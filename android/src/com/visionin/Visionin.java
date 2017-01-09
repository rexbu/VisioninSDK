package com.visionin;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;

import com.rex.load.NativeLoad;
import com.rex.utils.DeviceUtil;
import com.rex.utils.FileUtil;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Created by Rex on 16/6/2.
 */
public class Visionin {
    public static String TAG = "visionin";

    static{
        long so = NativeLoad.loadSo("libvisionin.so");
        NativeLoad.registJNIMethod(so, "com/visionin/Visionin", "authorization", "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;)V");
        NativeLoad.registJNIMethod(so, "com/visionin/Visionin", "resource", "(Landroid/content/res/AssetManager;)V");
    }

    public static void initialize(Context context, String appId, String appKey){
        File file = new File("/data/data/"+ DeviceUtil.getPackageName(context) +"/__resource");
        if (!file.exists()){
            file.mkdir();
        }

        try {
            InputStream is = context.getResources().getAssets().open("visionin.res");
            OutputStream os = FileUtil.fileOutputStream("/data/data/"+ DeviceUtil.getPackageName(context) +"/__resource/visionin.res");
            FileUtil.write(os, is);

            is = context.getResources().getAssets().open("visionin.dat");
            os = FileUtil.fileOutputStream("/data/data/"+ DeviceUtil.getPackageName(context) +"/__resource/visionin.dat");
            FileUtil.write(os, is);
        } catch (IOException e) {
            e.printStackTrace();
        }

        resource(context.getAssets());
        authorization(context, appId, appKey);
    }

    public static native void authorization(Context context, String appId, String appKey);
    public static native void resource(AssetManager asset);
}
