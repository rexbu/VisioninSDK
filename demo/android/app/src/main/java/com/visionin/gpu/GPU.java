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

        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "eglContext", "(Z)J");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "setRenderSurface", "(Landroid/view/Surface;)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "makeCurrent", "(J)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "destroyEGL", "(J)V");

        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "processTexture", "(II)V");
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

        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "setPreviewBlend", "(Ljava/lang/String;FFFFZ)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPU", "setVideoBlend", "(Ljava/lang/String;FFFFZ)V");
	}

    // processTexture函数要处理的texture类型，如果是surfaceTexture，则应该为OES类型
    public static final int GPU_TEXTURE_RGB = 0;
    public static final int GPU_TEXTURE_OES = 1;

    /// SurfaceTexture相关
    protected static native int createTexture();
    protected static native void destroyTexture(int texture);
    /// EGLContext
    protected native long eglContext(boolean active);
    public native void setRenderSurface(Surface surface);
    public native void makeCurrent(long context);
    public native void destroyEGL(long context);

    /// c处理
    public native void processTexture(int texture, int texture_type);
    public native void processBytes(byte[] bytes, int width, int height, int format);
    public native void getBytes(byte[] bytes);
    public native int getTexture();
    /// 输出
    public native void setOutputSize(int width, int height);
    public native void setOutputFormat(int format);
    /// 输入
    public native void setInputSize(int width, int height);
    public native void setInputRotation(int rotation);
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

    // blend可用于logo

    /**
     * 预览添加叠加渲染图片，可用于logo，仅用于预览，不会在视频流中生效
     * @param path  图片路径
     * @param x     叠加位置左上角x坐标，归一化坐标，0-1
     * @param y     叠加位置左上角y坐标，归一化坐标，0-1
     * @param w     叠加图片宽度，归一化坐标，0-1
     * @param h     叠加图片高度，归一化坐标，0-1
     * @param mirror 是否镜像
     */
    public native void setPreviewBlend(String path, float x, float y, float w, float h, boolean mirror);
    /**
     * 视频流添加叠加渲染图片，可用于logo，仅用于视频流，不会在预览窗口中生效
     * @param path  图片路径
     * @param x     叠加位置左上角x坐标，归一化坐标，0-1
     * @param y     叠加位置左上角y坐标，归一化坐标，0-1
     * @param w     叠加图片宽度，归一化坐标，0-1
     * @param h     叠加图片高度，归一化坐标，0-1
     * @param mirror 是否镜像
     */
    public native void setVideoBlend(String path, float x, float y, float w, float h, boolean mirror);
    public void removePreviewBlend(){
        setPreviewBlend(null, 0, 0, 0, 0, false);
    }
    public void removeVideoBlend(){
        setVideoBlend(null, 0, 0, 0, 0, false);
    }

    protected long  mEGLContext = 0;
    protected int   mProcessMode = 0;
    protected static boolean init = false;

    protected GPU(boolean glcontext){
        mEGLContext = eglContext(glcontext);
    }

    protected GPU(Surface surface) throws Exception {
        mEGLContext = eglContext(true);
        setRenderSurface(surface);
    }

    public void makeCurrent(){
        if (mEGLContext!=0) {
            makeCurrent(mEGLContext);
        }
    }
}