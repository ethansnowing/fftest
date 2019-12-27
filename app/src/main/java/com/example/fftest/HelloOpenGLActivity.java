package com.example.fftest;

import android.app.Activity;
import android.os.Bundle;

public class HelloOpenGLActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mGLSurfaceView = new myGLSurfaceView(this);
        setContentView(mGLSurfaceView);//这里我们用mGLSurfaceView来替换以前常用的R.layout.main
    }
    private myGLSurfaceView mGLSurfaceView;
}
