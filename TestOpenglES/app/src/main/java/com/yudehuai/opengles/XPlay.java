package com.yudehuai.opengles;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.util.AttributeSet;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class XPlay extends GLSurfaceView implements Runnable, SurfaceHolder.Callback,GLSurfaceView.Renderer {

    public XPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public void run() {
        Open("/sdcard/out.yuv", getHolder().getSurface());

    }

    public void startPlay(){
        new Thread(this).start();
    }   //使用网上的GLSurfaceView显示方法有这个线程，但是没有调用
    @Override
    public void surfaceCreated(SurfaceHolder holder){
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O){
            setRenderer(this);
        }
        new Thread(this).start();       //使用网上的GLSurfaceView显示方法没有这个开启线程，但是我这里不开启就不行
    }

    @Override
    public void surfaceChanged(SurfaceHolder var1, int var2, int var3, int var4){

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder var1){

    }

    public native void Open(String url, Object surface);

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int i, int i1) {

    }

    @Override
    public void onDrawFrame(GL10 gl10) {

    }
}
