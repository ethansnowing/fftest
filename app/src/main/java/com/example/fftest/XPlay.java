package com.example.fftest;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.SurfaceHolder;

public class XPlay extends GLSurfaceView implements Runnable, SurfaceHolder.Callback {
    public XPlay(Context context){
        super(context);
    }

    @Override
    public void run(){

    }

    @Override
    public void surfaceCreated(SurfaceHolder var1){
        new Thread(this).start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder var1, int var2, int var3, int var4){

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder var1){

    }

    public native void Open(String url, Object surface)
}
