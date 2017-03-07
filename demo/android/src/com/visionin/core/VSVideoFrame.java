package com.visionin.core;

import android.content.res.Configuration;
import android.graphics.SurfaceTexture;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.util.Log;
import android.view.Surface;
import com.visionin.Visionin;
import com.visionin.gpu.GPU;
import android.content.res.AssetManager;

/**
 * Created by Visionin on 16/7/18.
 */
public class VSVideoFrame extends GPU implements SurfaceTexture.OnFrameAvailableListener {
//    public static VSVideoFrame  shareInstance;

    public final static int CAMERA_FACING_BACK = 0;
    public final static int CAMERA_FACING_FRONT = 1;

    public final static int GPU_UNKNOWN = 0;
    public final static int GPU_RGBA = 1;
    public final static int GPU_NV21 = 2;       // yyyyvuvu
    public final static int GPU_YUV420P = 3;    // yyyyyuuvv
    public final static int GPU_YV12 = 4;       // yyyyyvvuu
    public final static int GPU_YUV444 = 5;     // yuvyuvyuv
    public final static int GPU_NV12 = 6;       // yyyyyuvuv
    public final static int GPU_BGRA = 7;

    // gpu处理模式，处理surfaceTexture模式或者bytes模式
    public final static int GPU_RPOCESS_TEXTURE = 0;
    public final static int GPU_PROCESS_BYTES = 1;

    protected boolean           running = false;
    protected int               textureId = -1;
    protected int               oldId=-1;
    protected SurfaceTexture    surfaceTexture;

    // 输出处理后像素
    protected byte[]            outputBytes = null;
    // 获取处理后像素回调
    protected VSRawBytesCallback    rawBytesCallback = null;

    private AssetManager        assetManager;

    private boolean isProcessing=false;
    private boolean isProcessingByte=false;

    public VSVideoFrame(Surface surface) throws Exception {
        super(surface);
    }

    /**
     * 开始处理
     * @return
     */
    public boolean start(/*AssetManager assetManager*/) {
        if (running){
            Log.e(Visionin.TAG, "Visionin Process is Running!");
            return false;
        }else{
            running = true;
            return true;
        }
    }

    /**
     * 结束处理
     * @return
     */
    public boolean stop(){
        if (!running){
            Log.e(Visionin.TAG, "Visionin Process isn't Running!");
            return false;
        }
        running = false;

        while(isProcessing || isProcessingByte){
            try {
                Thread.sleep(1);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        return true;
    }

    public void processBytes(byte[] bytes, int width, int height, int format){
        if (!running){
            Log.e(Visionin.TAG, "Visionin Process is not Running! Please call start first");
            return;
        }
        if(isProcessingByte){
            return;
        }
        isProcessingByte=true;
        super.processBytes(bytes, width, height, format);
        if (rawBytesCallback!=null){
            getBytes(outputBytes);
            rawBytesCallback.outputBytes(outputBytes);
        }

        isProcessingByte=false;
    }

    public SurfaceTexture surfaceTexture(){
        makeCurrent();
        if(textureId>0){
            oldId=textureId;
            surfaceTexture=null;
        }
        textureId = createTexture();
        surfaceTexture = new SurfaceTexture(textureId);
        surfaceTexture.setOnFrameAvailableListener(this);
        if(oldId>0){
            destroySurfaceTexture(oldId);
        }

        return surfaceTexture;
    }

    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {

        if (!running){
            return;
        }
        if(isProcessing){

            return;
        }

        isProcessing=true;
        this.makeCurrent();
        surfaceTexture.updateTexImage();
        super.processTexture(textureId, GPU_TEXTURE_OES);
        if (rawBytesCallback!=null) {
            getBytes(outputBytes);
            rawBytesCallback.outputBytes(outputBytes);
        }
        isProcessing=false;
    }

    protected int cameraPosition = CAMERA_FACING_BACK;
    /**
     * 设置前后相机
     * @param position
     */
    public void setCameraPosition(int position){
        cameraPosition = position;
        if (position == CAMERA_FACING_FRONT){
            super.setOutputMirror(this.mirrorFrontVideo);
            super.setPreviewMirror(this.mirrorFrontPreview);
        }
        else{
            super.setOutputMirror(this.mirrorBackVideo);
            super.setPreviewMirror(this.mirrorBackPreview);
        }
        setOutputImageOritation(outputImageOritation);
    }
    protected int outputImageOritation = Configuration.ORIENTATION_PORTRAIT;
    /**
     * 设置Surface显示方向
     * @param outputImageOritation
     */
    public void setOutputImageOritation(int outputImageOritation){
        this.outputImageOritation = outputImageOritation;
        //TODO: 前摄像头默认就是带镜像的，后摄像头默认不带镜像，但是经过left旋转后带了镜像，暂时没查出原因
        if (outputImageOritation==Configuration.ORIENTATION_PORTRAIT){
            if (cameraPosition==CAMERA_FACING_FRONT) {
                super.setInputRotation(GPURotateRightFlipVertical);
            }
            else{
                super.setInputRotation(GPURotateRightFlipHorizontal);
            }
        }
        else{
            //super.setInputRotation(GPUFlipVertical);
            super.setInputRotation(GPUNoRotation);

        }
        setVideoSize(videoWidth, videoHeight);
    }

    protected int videoWidth = 0;
    protected int videoHeight = 0;
    /**
     * 设置视频帧尺寸，VisioninSDK会根据图片旋转做调整，Surface方向显示放在Size设置之前
     * @param width
     * @param height
     */
    public void setVideoSize(int width, int height){
        videoWidth = width;
        videoHeight = height;
        if (outputImageOritation==Configuration.ORIENTATION_PORTRAIT) {
            super.setInputSize(height, width);
        }
        else{
            super.setInputSize(width, height);
        }
    }

    /**
     * 设置输出尺寸及输出格式
     */
    protected int outputWidth = 0;
    protected int outputHeight = 0 ;
    protected int outputFormat = GPU_UNKNOWN;
    public void setOutputSize(int width, int height){
        outputWidth = width;
        outputHeight = height;
        super.setOutputSize(width, height);
        setOutputFormat(outputFormat);
    }
    protected void setOutputFormat(int format){
        int size = 0;
        outputFormat = format;
        if (outputFormat!=GPU_UNKNOWN){
            super.setOutputFormat(format);
        }

        if (format==GPU_YUV420P || format==GPU_YV12 || format== GPU_NV21){
            if (outputWidth==0 || outputHeight==0){
                size = videoHeight*videoWidth*3/2;
            }
            else{
                size = outputWidth*outputHeight*3/2;
            }
        }
        else if (format == GPU_RGBA){
            if (outputWidth==0 || outputHeight==0){
                size = videoHeight*videoWidth*4;
            }
            else{
                size = outputWidth*outputHeight*4;
            }
        }
        if (outputFormat!= GPU_UNKNOWN && (outputBytes==null || outputBytes.length<size)){
            outputBytes = new byte[size];
        }
    }
    public void setRGBACallback(VSRawBytesCallback rgbaCallback){
        this.rawBytesCallback = rgbaCallback;
        setOutputFormat(GPU_RGBA);
    }

    /**
     * 获取处理后的I420格式视频帧数据
     * @param yuv420Callback
     */
    public void setYuv420PCallback(VSRawBytesCallback yuv420Callback){
        this.rawBytesCallback = yuv420Callback;
        setOutputFormat(GPU_YUV420P);
    }

    /**
     * 获取处理后的NV21格式视频帧数据
     * @param nv21Callback
     */
    public void setNV21Callback(VSRawBytesCallback nv21Callback){
        this.rawBytesCallback = nv21Callback;
        setOutputFormat(GPU_NV21);
    }

    /**
     * 获取处理后的NV12格式视频帧数据
     * @param nv12Callback
     */
    public void setNV12Callback(VSRawBytesCallback nv12Callback){
        this.rawBytesCallback = nv12Callback;
        setOutputFormat(GPU_NV12);
    }

    /**
     * 设置镜像
     */
    protected boolean mirrorFrontVideo = false;
    protected boolean mirrorBackVideo = false;
    protected boolean mirrorFrontPreview = false;
    protected boolean mirrorBackPreview = false;
    //TODO: 前摄像头默认就是带镜像的，后摄像头默认不带镜像，但是经过left旋转后带了镜像，暂时没查出原因
    public void setMirrorFrontVideo(boolean mirrorFrontVideo) {
        this.mirrorFrontVideo = mirrorFrontVideo;
        if(this.cameraPosition == CAMERA_FACING_FRONT) {
            super.setOutputMirror(this.mirrorFrontVideo);
        }
    }
    public void setMirrorFrontPreview(boolean mirrorFrontPreview) {
        this.mirrorFrontPreview = mirrorFrontPreview;
        if(this.cameraPosition == CAMERA_FACING_FRONT) {
            super.setPreviewMirror(this.mirrorFrontPreview);
        }
    }
    public void setMirrorBackVideo(boolean mirrorBackVideo) {
        this.mirrorBackVideo = mirrorBackVideo;
        if(this.cameraPosition == CAMERA_FACING_BACK) {
            super.setOutputMirror(this.mirrorBackVideo);
        }
    }

    public void setMirrorBackPreview(boolean mirrorBackPreview) {
        this.mirrorBackPreview = mirrorBackPreview;
        if(this.cameraPosition == CAMERA_FACING_BACK) {
            super.setPreviewMirror(this.mirrorBackPreview);
        }
    }

    public void destroySurfaceTexture(int texture){
        if(texture>0){
            destroyTexture(texture);
        }
    }

    public void destroy(){
        makeCurrent();
        destroySurfaceTexture(textureId);
        if(surfaceTexture!=null){
            surfaceTexture=null;
        }
        destroyEGL(mEGLContext);
    }


    public static final int GPUNoRotation = 0;
    public static final int GPURotateLeft = 1;
    public static final int GPURotateRight = 2;
    public static final int GPUFlipVertical = 3;
    public static final int GPUFlipHorizonal = 4;
    public static final int GPURotateRightFlipVertical = 5;
    public static final int GPURotateRightFlipHorizontal = 6;
    public static final int GPURotate180 = 7;

    // 滤镜
    public static final String VS_GAUSSIAN_BLUR_FILTER = "GaussianBlur";    // 高斯模糊
    public static final String VS_MEDIAN_BLUR_FILTER = "MedianBlur";        // 中值滤波
    public static final String VS_FROSTED_BLUR_FILTER = "FrostedBlur";      // 毛玻璃效果
    public static final String VS_SATURATION_FILTER = "Saturation";         // 饱和度
}
