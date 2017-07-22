package com.rex.utils;

import android.app.Activity;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;

/**
 * Created by Rex on 14-5-18.
 * 获取Manifest中得meta-data配置
 */
public class ManifestUtil {
    public static String getVersionName(Context context){
        try {
            return getPackageInfo(context).versionName;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }

        return "";
    }
    public static int getVersionCode(Context context){
        try {
            return getPackageInfo(context).versionCode;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }

        return 0;
    }

    public static Bundle getActivityMeta(Activity activity) throws PackageManager.NameNotFoundException {
        return activity.getPackageManager().getActivityInfo(activity.getComponentName(), PackageManager.GET_META_DATA).metaData;
    }

    public static Bundle getApplicationMeta(Activity activity) throws PackageManager.NameNotFoundException {
        return activity.getPackageManager().getApplicationInfo(activity.getPackageName(), PackageManager.GET_META_DATA).metaData;
    }

    public static Bundle getServiceMeta(Service service) throws PackageManager.NameNotFoundException {
        return service.getPackageManager().getServiceInfo(new ComponentName(service, service.getClass()), PackageManager.GET_META_DATA).metaData;
    }

    public static Bundle getReceiverMeta(Context context, BroadcastReceiver receiver) throws PackageManager.NameNotFoundException {
        return context.getPackageManager().getReceiverInfo(new ComponentName(context, receiver.getClass()), PackageManager.GET_META_DATA).metaData;
    }

    public static PackageInfo getPackageInfo(Context context) throws PackageManager.NameNotFoundException {
        return context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
    }
}
