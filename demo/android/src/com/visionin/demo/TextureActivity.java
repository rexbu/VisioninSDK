package com.visionin.demo;

import android.app.Activity;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
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

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class TextureActivity extends Activity implements SurfaceHolder.Callback{

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
    protected SeekBar shaperValueSeek;
    protected boolean isProps = true;
    protected boolean isShaper = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_texture);

        try {
            InputStream is = this.getResources().getAssets().open("cat.zip");
            OutputStream os = FileUtil.fileOutputStream("/data/data/"+ DeviceUtil.getPackageName(this) +"/cat.zip");
            FileUtil.write(os, is);
        } catch (IOException e) {
            e.printStackTrace();
        }

        surfaceView = (SurfaceView) findViewById(R.id.camera_surfaceView);
        surfaceHolder = surfaceView.getHolder();
        imageView = (ImageView)findViewById(R.id.cmaera_iamgeView);

        isFront = true;
        initView();
    }

    @Override
    protected void onResume() {
        super.onResume();
        videoSize = CameraUtil.openCamera(1280, 720, isFront);
        surfaceHolder.addCallback(this);
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        try {
            videoFrame = new VSVideoFrame(surfaceHolder.getSurface());
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
        videoFrame.setMirrorFrontVideo(false);
        //设置预览镜像，true时预览为镜像（正常画面），false时为非镜像(左右颠倒)
        videoFrame.setMirrorFrontPreview(true);
        //设置推流视频镜像
        videoFrame.setMirrorBackVideo(false);
        //设置预览镜像，true时预览为镜像（左右颠倒），false时为非镜像(正常画面)
        videoFrame.setMirrorBackPreview(true);
        videoFrame.setOutputSize(360, 640);
        videoFrame.setVideoSize(videoSize.width, videoSize.height);

        videoFrame.setSmoothStrength(0.9f);
        videoFrame.setYuv420PCallback(new VSRawBytesCallback() {
            @Override
            public void outputBytes(byte[] bytes) {
                // 获取人脸坐标点，偶数为x坐标，奇数为y坐标
                float[] point = VSFacer.getFacerMarks(0);
                if (point!=null){
                    //Log.e("Facer", "face第一个点  x:"+point[0]+" y:"+point[1]);
                }
                imageView.setImageBitmap(yuv420p2RGBABitmap(bytes, 360, 640));
            }
        });

        VSFacer.initialize(this);
        VSFacer.startFacerTracking();

        VSFacer.startShaper();
        boolean st = VSProps.startProps("/data/data/" + DeviceUtil.getPackageName(this) + "/cat.zip", false);
        if (!st){
            Log.e("Visionin", "Set Props Error!");
        }
        videoFrame.start();
        try {
            com.rex.utils.CameraUtil.mCamera.setPreviewTexture(videoFrame.surfaceTexture());
            com.rex.utils.CameraUtil.mCamera.startPreview();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        if (videoFrame!=null){
            videoFrame.stop();
        }
        VSProps.destroyProps();
        VSFacer.destroyFacer();
        videoFrame.destroy();
        com.rex.utils.CameraUtil.releaseCamera();
        videoFrame = null;
    }

    // 切换摄像头做的操作
    protected void rotateCamera(){
        if(isFront){
            isFront=false;
            videoFrame.setCameraPosition(VSVideoFrame.CAMERA_FACING_BACK);
        }
        else{
            isFront=true;
            videoFrame.setCameraPosition(VSVideoFrame.CAMERA_FACING_FRONT);
        }
        com.rex.utils.CameraUtil.releaseCamera();
        videoSize = com.rex.utils.CameraUtil.openCamera(1280, 720, isFront);
        videoFrame.setVideoSize(videoSize.width, videoSize.height);

        try {
            com.rex.utils.CameraUtil.mCamera.setPreviewTexture(videoFrame.surfaceTexture());
            com.rex.utils.CameraUtil.mCamera.startPreview();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    protected void initView(){
        swtichButton = (Button)findViewById(R.id.switchCamera);
        propsButton = (Button)findViewById(R.id.propsButton);
        shaperButton = (Button)findViewById(R.id.shaperButton);
        filterSpinner = (Spinner)findViewById(R.id.filterSpinner);
        smoothText = (TextView)findViewById(R.id.smoothText);
        smoothValueSeek = (SeekBar)findViewById(R.id.smoothValueBar);
        smoothText.setText("磨皮：0.9");
        smoothValueSeek.setProgress(90);
        shaperValueSeek = (SeekBar)findViewById(R.id.shaperValueBar);

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
                    shaperButton.setText("开整形");
                    VSFacer.stopShaper();
                    isShaper = false;
                }
                else{
                    shaperButton.setText("关整形");
                    VSFacer.startShaper();
                    isShaper = true;
                }
            }
        });

        propsButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (isProps){
                    VSProps.stopProps();
                    isProps = false;
                }
                else{
                    VSProps.startProps("/data/data/" + DeviceUtil.getPackageName(TextureActivity.this) + "/cat.zip", false);
                    isProps = true;
                }
            }
        });
        // 设置滤镜
        filterSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                if (videoFrame!=null){
                    String filter = getResources().getStringArray(R.array.filter_name)[i];
                    videoFrame.setExtraFilter(filter);
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
        shaperValueSeek.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
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
                    VSFacer.setShapping(VSFacer.SHAPER_CMD_EYE, strength);
                    VSFacer.setShapping(VSFacer.SHAPER_CMD_FACE, strength);
                    VSFacer.setShapping(VSFacer.SHAPER_CMD_CHIN, strength);
                    VSFacer.setShapping(VSFacer.SAHPER_CMD_CHEEK, strength);
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
