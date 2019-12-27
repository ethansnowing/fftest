package com.example.fftest;

import android.content.Context;
import android.opengl.GLSurfaceView;

public class myGLSurfaceView extends GLSurfaceView {
    public myGLSurfaceView(Context context) {
        super(context);
        // TODO Auto-generated constructor stub
        mrender = new MyGLRenderer();
        setRenderer(mrender);
    }
    private MyGLRenderer mrender;
}
