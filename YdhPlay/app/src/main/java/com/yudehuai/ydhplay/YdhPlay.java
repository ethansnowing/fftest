package com.yudehuai.ydhplay;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.View;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class YdhPlay extends GLSurfaceView implements SurfaceHolder.Callback, GLSurfaceView.Renderer, View.OnClickListener {
    public YdhPlay(Context context, AttributeSet attrs){
        super(context, attrs);
        //android 8.0的手机需要做两件事：1，setRenderer(this);2.继承GLSurfaceView.Renderer接口，并实现3个函数
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O){
            setRenderer(this);
            setOnClickListener(this);
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder)
    {
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

    @Override
    public void onClick(View v) {
        playOrPause();
    }
    public native void playOrPause();
}
