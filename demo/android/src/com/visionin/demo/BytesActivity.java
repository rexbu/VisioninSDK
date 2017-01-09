package com.visionin.demo;

import android.app.Activity;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.YuvImage;
import android.hardware.Camera;
import android.media.Image;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.TextView;

import com.rex.utils.CameraUtil;
import com.rex.utils.DeviceUtil;
import com.rex.utils.FileUtil;
import com.visionin.core.VSFacer;
import com.visionin.core.VSProps;
import com.visionin.core.VSRawBytesCallback;
import com.visionin.core.VSVideoFrame;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class BytesActivity extends Activity implements Camera.PreviewCallback{

    protected SurfaceView surfaceView = null;
    protected ImageView imageView = null;
    protected SurfaceHolder surfaceHolder = null;
    protected VSVideoFrame videoFrame = null;
    protected boolean isFront = true;
    Camera.Size videoSize;

    protected Button swtichButton;
    protected Button propsButton;
    protected Button shaperButton;
    protected Spinner filterSpinner;
    protected TextView smoothText;
    protected SeekBar smoothValueSeek;
    protected boolean isProps = true;
    protected boolean isShaper = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bytes);

        try {
            InputStream is = this.getResources().getAssets().open("rabbit.zip");
            OutputStream os = FileUtil.fileOutputStream("/data/data/"+ DeviceUtil.getPackageName(this) +"/rabbit.zip");
            FileUtil.write(os, is);
        } catch (IOException e) {
            e.printStackTrace();
        }

        isFront = true;
        initView();

        // 摄像头
        videoSize = CameraUtil.openCamera(1280, 720, isFront);
        CameraUtil.mCamera.setPreviewCallback(this);
        CameraUtil.mCamera.startPreview();

        // VSVideoFrame
        try {
            videoFrame = new VSVideoFrame(null);
        } catch (Exception e) {
            e.printStackTrace();
            return;
        }

        if (isFront){
            videoFrame.setCameraPosition(VSVideoFrame.CAMERA_FACING_FRONT);
        }
        else {
            videoFrame.setCameraPosition(VSVideoFrame.CAMERA_FACING_BACK);
        }

        videoFrame.setOutputImageOritation(Configuration.ORIENTATION_PORTRAIT);
        //设置推流视频镜像
        videoFrame.setMirrorFrontVideo(true);
        //设置预览镜像，true时预览为镜像（正常画面），false时为非镜像(左右颠倒)
        videoFrame.setMirrorFrontPreview(true);
        //设置推流视频镜像
        videoFrame.setMirrorBackVideo(false);
        //设置预览镜像，true时预览为镜像（左右颠倒），false时为非镜像(正常画面)
        videoFrame.setMirrorBackPreview(false);
        videoFrame.setOutputSize(360, 640);
        videoFrame.setVideoSize(videoSize.width, videoSize.height);

        videoFrame.setSmoothStrength(0.9f);
        videoFrame.setYuv420PCallback(new VSRawBytesCallback() {
            @Override
            public void outputBytes(byte[] bytes) {
                // 获取人脸坐标点
                float[] point = VSFacer.getFacerMarks(0);
                if (point!=null){
                    Log.e("Facer", "facer:"+point[0]+"/"+point[1]);
                }
                imageView.setImageBitmap(yuv420p2RGBABitmap(bytes, 360, 640));
            }
        });

        VSFacer.initialize(this);
        VSFacer.startFacerTracking();
        VSFacer.startFacerShaper();
        VSProps.startStProps("/data/data/"+ DeviceUtil.getPackageName(this) +"/rabbit.zip", false);
        videoFrame.start();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        if (videoFrame!=null){
            videoFrame.stop();
        }
        com.rex.utils.CameraUtil.releaseCamera();
        videoFrame.stop();
        VSFacer.destroyFacer();
        VSProps.destroyProps();
        videoFrame.destroy();
        videoFrame = null;

        super.onPause();
    }

    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {
        videoFrame.processBytes(data, videoSize.width, videoSize.height, VSVideoFrame.GPU_NV21);
    }

    // 切换摄像头做的操作
    protected void rotateCamera(){
        CameraUtil.releaseCamera();

        videoSize = CameraUtil.openCamera(1280, 720, isFront);
        if(isFront){
            isFront=false;
            videoFrame.setCameraPosition(VSVideoFrame.CAMERA_FACING_BACK);
        }
        else{
            isFront=true;
            videoFrame.setCameraPosition(VSVideoFrame.CAMERA_FACING_FRONT);
        }
        videoFrame.setVideoSize(videoSize.width, videoSize.height);

        CameraUtil.mCamera.setPreviewCallback(this);
        CameraUtil.mCamera.startPreview();
    }

    protected void initView(){
        imageView = (ImageView)findViewById(R.id.imageView);
        swtichButton = (Button)findViewById(R.id.switchCamera);
        propsButton = (Button)findViewById(R.id.propsButton);
        shaperButton = (Button)findViewById(R.id.shaperButton);
        filterSpinner = (Spinner)findViewById(R.id.filterSpinner);
        smoothText = (TextView)findViewById(R.id.smoothText);
        smoothValueSeek = (SeekBar)findViewById(R.id.smoothValueBar);
        smoothText.setText("磨皮：0.9");
        smoothValueSeek.setProgress(90);

        swtichButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                rotateCamera();
            }
        });
        shaperButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (isShaper){
                    VSFacer.stopFacerShaper();
                    isShaper = false;
                }
                else{
                    VSFacer.startFacerShaper();
                    isShaper = true;
                }
            }
        });
        propsButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (isProps){
                    VSProps.stopStProps();
                    isProps = false;
                }
                else{
                    VSProps.startStProps("/data/data/"+ DeviceUtil.getPackageName(BytesActivity.this) +"/rabbit.zip", false);
                    isProps = true;
                }
            }
        });
        // 设置滤镜
        filterSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                if (videoFrame!=null){
                    videoFrame.setExtraFilter(getResources().getStringArray(R.array.filter_name)[i]);
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });
        smoothValueSeek.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            public int value;
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                value = i;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                if (videoFrame!=null){
                    float strength = value*(float)1.0/100;
                    videoFrame.setSmoothStrength(strength);
                    smoothText.setText("磨皮："+ strength);
                }
            }
        });
    }
    public Bitmap yuv420p2RGBABitmap(byte[] data, int width, int height) {
        int frameSize = width * height;
        int[] rgba = new int[frameSize];
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                int y = (0xff & ((int) data[i * width + j]));
                int u = (0xff & ((int) data[frameSize + (i >> 1) * width / 2 + (j >> 1)]));
                int v = (0xff & ((int) data[frameSize + width * height / 4 + (i >> 1) * width / 2 + (j >> 1)]));
                y = y < 16 ? 16 : y;
                int r = Math.round(1.164f * (y - 16) + 1.596f * (v - 128));
                int g = Math.round(1.164f * (y - 16) - 0.813f * (v - 128) - 0.391f * (u - 128));
                int b = Math.round(1.164f * (y - 16) + 2.018f * (u - 128));
                r = r < 0 ? 0 : (r > 255 ? 255 : r);
                g = g < 0 ? 0 : (g > 255 ? 255 : g);
                b = b < 0 ? 0 : (b > 255 ? 255 : b);
                rgba[i * width + j] = 0xff000000 + (r << 16) + (g << 8) + b;
            }
        Bitmap bmp = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        bmp.setPixels(rgba, 0, width, 0, 0, width, height);
        return bmp;
    }

    public Bitmap nv212RGBABitmap(byte[] data, int width, int height) {
        int frameSize = width * height;
        int[] rgba = new int[frameSize];
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                int y = (0xff & ((int) data[i * width + j]));
                int v = (0xff & ((int) data[frameSize + (i >> 1) * width + (j & ~1) + 0]));
                int u = (0xff & ((int) data[frameSize + (i >> 1) * width + (j & ~1) + 1]));
                y = y < 16 ? 16 : y;
                int r = Math.round(1.164f * (y - 16) + 1.596f * (v - 128));
                int g = Math.round(1.164f * (y - 16) - 0.813f * (v - 128) - 0.391f * (u - 128));
                int b = Math.round(1.164f * (y - 16) + 2.018f * (u - 128));
                r = r < 0 ? 0 : (r > 255 ? 255 : r);
                g = g < 0 ? 0 : (g > 255 ? 255 : g);
                b = b < 0 ? 0 : (b > 255 ? 255 : b);
                rgba[i * width + j] = 0xff000000 + (r << 16) + (g << 8) + b;
            }
        Bitmap bmp = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        bmp.setPixels(rgba, 0, width, 0, 0, width, height);
        return bmp;
    }
}
