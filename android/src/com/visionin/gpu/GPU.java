package com.visionin.gpu;

import android.content.res.AssetManager;
import android.provider.Settings;
import android.util.Log;
import android.view.Surface;

import com.rex.load.NativeLoad;
import com.visionin.Visionin;

public class GPU {
	static {
		long so = NativeLoad.loadSo("libvisionin.so");

        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "createTexture", "()I");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "destroyTexture", "(I)V");

        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "eglContext", "(Landroid/view/Surface;)J");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "makeCurrent", "(J)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "destroyEGL", "(J)V");

        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "processTexture", "(I)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "processBytes", "([BIII)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "getBytes", "([B)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "getTexture", "()I");

        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "setOutputSize", "(II)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "setOutputFormat", "(I)V");

        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "setInputSize", "(II)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "setInputRotation", "(I)V");

        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "setPreviewMirror", "(Z)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "setOutputMirror", "(Z)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "setOutputRotation", "(I)V");

        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "setSmoothStrength", "(F)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "setBrightenLevel", "(F)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "setToningLevel", "(F)V");

        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "setExtraFilter", "(Ljava/lang/String;)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "closeExtraFilter", "()V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "setExtraParameter", "(F)V");

        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "setOutputView", "()V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "removeOutputView", "()V");
	}

    /// SurfaceTexture相关
    protected static native int createTexture();
    protected static native void destroyTexture(int texture);
    /// EGLContext
    protected native long eglContext(Surface surface);
    protected native void makeCurrent(long context);
    protected native void destroyEGL(long context);
    /// c处理
    protected native void processTexture(int texture);
    protected native void processBytes(byte[] bytes, int width, int height, int format);
    public native void getBytes(byte[] bytes);
    public native int getTexture();
    /// 输出
    protected native void setOutputSize(int width, int height);
    protected native void setOutputFormat(int format);
    /// 输入
    protected native void setInputSize(int width, int height);
    protected native void setInputRotation(int rotation);
    /// 镜像
    public native void setPreviewMirror(boolean mirror);
    public native void setOutputMirror(boolean mirror);
    protected native void setOutputRotation(int rotation);

    /// 美颜
    public native void setSmoothStrength(float level);
    @Deprecated
    public native void setBrightenLevel(float level);
    @Deprecated
    public native void setToningLevel(float level);
    /// 滤镜
    public native void setExtraFilter(String filter);
    public native void closeExtraFilter();
    public native void setExtraParameter(float para);
    /// 预览
    public native void setOutputView();
    public native void removeOutputView();

    protected long  mEGLContext = 0;
    protected int   mProcessMode = 0;
    protected static boolean init = false;

    protected GPU(Surface surface) throws Exception {
        mEGLContext = eglContext(surface);
        if (mEGLContext==0){
            throw new Exception("GLContext create Error!");
        }
    }
    protected GPU(Surface surface, int mode){
        mProcessMode = mode;
        mEGLContext = eglContext(surface);
        if (mEGLContext==0){
        }
    }

    public void makeCurrent(){
        if (mEGLContext!=0) {
            makeCurrent(mEGLContext);
        }
    }
}