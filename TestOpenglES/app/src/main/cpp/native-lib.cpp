#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <string.h>

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG_TAG "jnidemo"

//顶点着色器
#define GET_STR(x) #x
static const char *vertexShader = GET_STR(
        attribute vec4 aPosition;  //顶点坐标
        attribute vec2 aTexCoord;  //材质顶点坐标
        varying   vec2 vTexCoord;  //输出的材质坐标
        void main(){
            vTexCoord = vec2(aTexCoord.x, 1.0-aTexCoord.y);
            gl_Position = aPosition;
        });

//片元着色器，软解码和部分x86硬解码
static const char *fragYUV420P = GET_STR(
        precision mediump float;        //精度
        varying vec2 vTexCoord;         //顶点着色器传递的坐标
        uniform sampler2D yTexture;     //输入的材质(不透明灰度，单像素)
        uniform sampler2D uTexture;
        uniform sampler2D vTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture, vTexCoord).r;
            yuv.g = texture2D(uTexture, vTexCoord).r - 0.5;
            yuv.b = texture2D(vTexture, vTexCoord).r - 0.5;
            rgb = mat3(1.0,      1.0,      1.0,
                       0.0,      -0.39465, 2.03211,
                       1.13983,  -0.5806,  0.0)*yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb, 1.0);
        }
);

GLint InitShader(const char *code, GLint type)
{
    GLint sh = glCreateShader(type);
    if(sh == 0)
    {
        LOGE("ydh--glCreateShader %d failed!");
        return 0;
    }
    //加载shader
    glShaderSource(sh,
                    1,      //shader数量
                    &code,   //shader代码
                    0);       //代码长度

     //编译shader
     glCompileShader(sh);
     //获取编译情况
     GLint status;
     glGetShaderiv(sh, GL_COMPILE_STATUS, &status);     //获取编译状态
     if(status ==0)
     {
         LOGE("ydh--glCompileShader failed!");
         return 0;
     }
    LOGE("ydh--glCompileShader success!");
    return sh;
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_yudehuai_opengles_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_yudehuai_opengles_XPlay_Open(JNIEnv *env, jobject instance, jstring url_,
                                      jobject surface) {
    const char *url = env->GetStringUTFChars(url_, 0);
    LOGE("ydh--open url is %s", url);
    FILE *fp = fopen(url, "rb");
    if(!fp)
    {
        LOGE("ydh--open file %s failed!", url);
        return;
    }

    // TODO
    //1 获取原始窗口
    ANativeWindow *nwin = ANativeWindow_fromSurface(env, surface);
    //2 EGL
    // a.EGL display创建和初始化
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(display== EGL_NO_DISPLAY){
        LOGE("ydh--eglGetDisplay failed!");
        return;
    }
    if(EGL_TRUE != eglInitialize(display, 0, 0)){
        LOGE("ydh--eglInitialize failed!");
    }
    // b.surface
    //b-1 surface窗口配置
    EGLConfig config;   //输出项
    EGLint configNum;   //输出项
    EGLint configSpec[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE,8,
            EGL_BLUE_SIZE,8,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE
    };
    if(EGL_TRUE !=eglChooseConfig(display, configSpec, &config, 1, &configNum)){
        LOGE("ydh--eglChooseConfig failed!");
        return;
    }
    // 创建surface
    EGLSurface  winsurface = eglCreateWindowSurface(display, config, nwin, 0);
    if(winsurface == EGL_NO_SURFACE){
        LOGE("ydh--eglCreateWindowSurface failed!");
        return;
    }
    //c context 创建关联的上下文
    const EGLint ctxAttr[] = {
            EGL_CONTEXT_CLIENT_VERSION,2,EGL_NONE
    };
    EGLContext  context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);
    if(context ==EGL_NO_CONTEXT)
    {
        LOGE("ydh--eglCreateContext failed!");
        return;
    }
    if(EGL_TRUE != eglMakeCurrent(display, winsurface, winsurface, context))
    {
        LOGE("ydh--eglMakeCurrent failed!");
        return;
    }
    LOGE("ydh--EGL init success!");

    //顶点和片元shader初始化
    //顶点shader初始化
    GLint vsh = InitShader(vertexShader, GL_VERTEX_SHADER);
    //片元yuv420 shader初始化
    GLint fsh = InitShader(fragYUV420P, GL_FRAGMENT_SHADER);

    ///////////////////////////////////////////////
    //创建渲染程序
    GLint program = glCreateProgram();
    if(program == 0)
    {
        LOGE("ydh--glCreateProgram failed!");
        return;
    }
    //渲染程序中加入着色器代码
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);
    //链接程序
    glLinkProgram(program);
    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);       //获取链接程序后的状态，判断是否链接成功
    if(status != GL_TRUE)
    {
        LOGE("ydh--glLinkProgram failed!");
        return;
    }
    glUseProgram(program);      //激活渲染程序
    LOGE("ydh--glLinkProgram success!");
    ///////////////////////////////////////////////
    //加入三维顶点数据，两个三角形组成正方形
    static float vers[] = {
            1.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f
    };
    GLuint apos = (GLuint)glGetAttribLocation(program, "aPosition");
    glEnableVertexAttribArray(apos);
    //传递顶点
    glVertexAttribPointer(apos, 3, GL_FLOAT, GL_FALSE, 12, vers);
    //传递材质坐标数据
    static float txts[] = {
            1.0f,  0.0f,
            0.0f,  0.0f,
            1.0f,  1.0f,
            0.0f,  1.0f
    };
    GLuint atex = (GLuint)glGetAttribLocation(program, "aTexCoord");
    glEnableVertexAttribArray(atex);
    //传递材质坐标
    glVertexAttribPointer(atex, 2, GL_FLOAT, GL_FALSE, 8, txts);

    int width = 424;
    int height = 240;

    //材质纹理初始化
    //设置纹理层
    glUniform1i( glGetUniformLocation(program, "yTexture"), 0);     //对于纹理第1层
    glUniform1i( glGetUniformLocation(program, "uTexture"), 1);     //对于纹理第2层
    glUniform1i( glGetUniformLocation(program, "vTexture"), 2);     //对于纹理第3层
    GLuint texts[3] = {0};      //创建opengl纹理
    glGenTextures(3, texts);        //创建三个纹理
    glBindTexture(GL_TEXTURE_2D, texts[0]);     //设置纹理属性
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);     //缩小的过滤器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);     //放大的过滤器
    //设置纹理的格式和大小
    glTexImage2D(GL_TEXTURE_2D,
                 0,                  //细节基本 0默认
                 GL_LUMINANCE,       //gpu内部格式亮度，灰度图
                 width, height,      //拉升到全屏
                 0,                  //边框
                 GL_LUMINANCE,       //数据的像素格式亮度，灰度图要与上面一致
                 GL_UNSIGNED_BYTE,   //像素的数据类型
                 NULL);              //纹理的数据

    //设置纹理层
    glBindTexture(GL_TEXTURE_2D, texts[1]);     //设置纹理属性
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);     //缩小的过滤器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);     //放大的过滤器
    //设置纹理的格式和大小
    glTexImage2D(GL_TEXTURE_2D,
                 0,                  //细节基本 0默认
                 GL_LUMINANCE,       //gpu内部格式亮度，灰度图
                 width/2, height/2,      //拉升到全屏
                 0,                  //边框
                 GL_LUMINANCE,       //数据的像素格式亮度，灰度图要与上面一致
                 GL_UNSIGNED_BYTE,   //像素的数据类型
                 NULL);              //纹理的数据

    //设置纹理层
    glBindTexture(GL_TEXTURE_2D, texts[2]);     //设置纹理属性
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);     //缩小的过滤器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);     //放大的过滤器
    //设置纹理的格式和大小
    glTexImage2D(GL_TEXTURE_2D,
                 0,                  //细节基本 0默认
                 GL_LUMINANCE,       //gpu内部格式亮度，灰度图
                 width/2, height/2,      //拉升到全屏
                 0,                  //边框
                 GL_LUMINANCE,       //数据的像素格式亮度，灰度图要与上面一致
                 GL_UNSIGNED_BYTE,   //像素的数据类型
                 NULL);              //纹理的数据

    //////////////////////////////////////////////
    //纹理的修改和显示
    unsigned char *buf[3] = {0};
    buf[0] = new unsigned char[width*height];
    buf[1] = new unsigned char[width/2 * height/2];     //初始化第二层纹理，相当于u，也可以不用除4，因为会自动化缩小纹理
    buf[2] = new unsigned char[width/2 * height/2];

    for(int i=0; i<10000;i++) {
//        LOGI("%d",i);
//        memset(buf[0], i, width*height);
//        memset(buf[1], i, width*height/4);
//        memset(buf[2], i, width*height/4);

        //显示out.yuv视频  420p   yyyyyyyy  uu  vv
        if(feof(fp) == 0)
        {
            fread(buf[0], 1, width*height, fp);
            fread(buf[1], 1, width*height/4, fp);
            fread(buf[2], 1, width*height/4, fp);

        }

        //激活第1层纹理，绑定到创建的opengl纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texts[0]);
        //替换纹理内容
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                        buf[0]);

        //激活第2层纹理，绑定到创建的opengl纹理
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, texts[1]);
        //替换纹理内容
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width / 2, height / 2, GL_LUMINANCE,
                        GL_UNSIGNED_BYTE, buf[1]);

        //激活第1层纹理，绑定到创建的opengl纹理
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, texts[2]);
        //替换纹理内容
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width / 2, height / 2, GL_LUMINANCE,
                        GL_UNSIGNED_BYTE, buf[2]);

        //三维绘制
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        //窗口显示
        eglSwapBuffers(display, winsurface);
    }






    env->ReleaseStringUTFChars(url_, url);
}