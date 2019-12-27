package com.example.fftest;

import android.opengl.GLSurfaceView;
import android.view.SurfaceHolder;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView.Renderer;

public class MyGLRenderer implements Renderer, Runnable {
    @Override
    public void run(){
        MyGLRenderer mgv = new MyGLRenderer();
        Open3("/storage/emulated/0/test.mp4", getHolder().getSurface());
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        // TODO Auto-generated method stub
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);//清空缓存
    }
    @Override
    public void onSurfaceChanged(GL10 gl,int width,int height) {
        // TODO Auto-generated method stub
    }
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        // TODO Auto-generated method stub
        new Thread(this).start();
    }

    public native void Open3(String url, Object surface);
}
