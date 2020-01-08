package com.yudehuai.ydhplay;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.util.AttributeSet;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class YdhPlay extends GLSurfaceView implements SurfaceHolder.Callback, GLSurfaceView.Renderer {
    public YdhPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder)
    {
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O){
            setRenderer(this);
        }
//        new Thread().start();      //使用网上的GLSurfaceView显示方法没有这个开启线程，但是我这里不开启就不行
        //初始化opengl显示
        InitView(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder var1, int var2, int var3, int var4)
    {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder var1)
    {

    }

    @Override
    public void onSurfaceCreated(GL10 var1, EGLConfig var2)
    {

    }

    @Override
    public void onSurfaceChanged(GL10 var1, int var2, int var3)
    {

    }

    @Override
    public void onDrawFrame(GL10 var1)
    {

    }

    public native void InitView(Object surface);
}
