package com.rex.utils;

import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.util.Log;

/**
 * Created by Visionin on 16/11/26.
 */
public class CameraUtil {
    public static Camera mCamera = null;

    public static Camera.Size openCamera(boolean isFront) {
        if (mCamera != null) {
            throw new RuntimeException("camera already initialized");
        }
        int mPosition;
        if(isFront){
            mPosition= Camera.CameraInfo.CAMERA_FACING_FRONT;
        }else {
            mPosition=Camera.CameraInfo.CAMERA_FACING_BACK;
        }
        mCamera=Camera.open(mPosition);

        if (mCamera == null) {
            Log.e("Visionin", "No front-facing camera found; opening default");
            mCamera = Camera.open();    // opens first back-facing camera
        }

        Camera.Parameters parms = mCamera.getParameters();
        return parms.getPreviewSize();
    }

    public static Camera.Size openCamera(int desiredWidth, int desiredHeight, boolean isFront) {
        if (mCamera != null) {
            throw new RuntimeException("camera already initialized");
        }
        int mPosition;
        if(isFront){
            mPosition= Camera.CameraInfo.CAMERA_FACING_FRONT;
            mCamera = Camera.open(mPosition);
        }else {
            mPosition=Camera.CameraInfo.CAMERA_FACING_BACK;
            mCamera=Camera.open(mPosition);
        }

        if (mCamera == null) {
            Log.e("Visionin", "No front-facing camera found; opening default");
            mCamera = Camera.open();    // opens first back-facing camera
        }

        Camera.Parameters parms = mCamera.getParameters();
        Camera.Size ppsfv = parms.getPreferredPreviewSizeForVideo();
        if (ppsfv != null) {
            Log.e("Visionin", "Camera preferred preview size for video is " +
                    ppsfv.width + "x" + ppsfv.height);
            parms.setPreviewSize(ppsfv.width, ppsfv.height);
        }

        for (Camera.Size size : parms.getSupportedPreviewSizes()) {
            if (size.width == desiredWidth && size.height == desiredHeight) {
                parms.setPreviewSize(desiredWidth, desiredHeight);
                break;
            }
        }

        parms.setRecordingHint(true);
        parms.setPreviewFormat(ImageFormat.NV21);
        mCamera.setParameters(parms);

        int[] fpsRange = new int[2];
        parms.getPreviewFpsRange(fpsRange);
        return parms.getPreviewSize();
    }

    public static void releaseCamera() {
        if (mCamera != null) {
            mCamera.stopPreview();
            mCamera.release();
            mCamera = null;
            Log.d("Visionin", "releaseCamera -- done");
        }
    }
}
