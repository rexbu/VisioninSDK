package com.visionin.gpu;

import com.rex.load.NativeLoad;

/**
 * Created by Visionin on 17/3/29.
 */
public class GPUFilter {
    protected long p;
    public GPUFilter(){
        p = FCreateFilter(VERTEX_SHADER, FRAGMENT_SHADER);
    }
    public GPUFilter(String vertex, String fragment){
        p = FCreateFilter(vertex, fragment);
    }

    public void destroy(){
        FDestroyFilter(p);
    }
    public void setInputTexture(int texture, int width, int height){
        FSetInputTexture(p, texture, width, height);
    }
    public void setOutputRotation(int rotation){
        FSetOutputRotation(p, rotation);
    }
    public void setOutputSize(int width, int height){
        FSetOutputSize(p, width, height);
    }
    public int getOutputTexture(){
        return FGetOutputTexture(p);
    }

    /**
     * 暂不可用，在setInputTexture执行newFrame
     */
    public void newFrame(){
        FNewFrame(p);
    }

    /**
     * 析构函数
     */
    @Override
    protected void finalize(){
        FDestroyFilter(p);
    }
    /**
     * Fitler接口
     */
    public native long FCreateFilter(String vertex, String fragment);
    public native void FDestroyFilter(long p);
    public native void FSetInputTexture(long p, int texture, int width, int height);
    public native void FSetOutputRotation(long p, int rotation);
    public native void FSetOutputSize(long p, int width, int height);
    public native int FGetOutputTexture(long p);
    public native void FNewFrame(long p);

    static{
        long so = NativeLoad.loadSo("libvisionin.so");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPUFilter", "FCreateFilter", "(Ljava/lang/String;Ljava/lang/String;)J");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPUFilter", "FDestroyFilter", "(J)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPUFilter", "FSetInputTexture", "(JIII)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPUFilter", "FSetOutputRotation", "(JI)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPUFilter", "FSetOutputSize", "(JII)V");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPUFilter", "FGetOutputTexture", "(J)I");
        NativeLoad.registJNIMethod(so, "com/visionin/gpu/GPUFilter", "FNewFrame", "(J)V");
    }

    protected static final String VERTEX_SHADER = "precision mediump float; attribute vec4 position;\n" +
            "    attribute vec4 inputTextureCoordinate;\n" +
            "    varying vec2 textureCoordinate;\n" +
            "    void main()\n" +
            "    {\n" +
            "        gl_Position = position;\n" +
            "        textureCoordinate = inputTextureCoordinate.xy;\n" +
            "    }";
    protected static final String FRAGMENT_SHADER = "precision mediump float; varying vec2 textureCoordinate;\n" +
            "    uniform sampler2D inputImageTexture;\n" +
            "\n" +
            "    void main()\n" +
            "    {\n" +
            "        gl_FragColor = texture2D(inputImageTexture, textureCoordinate);\n" +
            "    }";
}
