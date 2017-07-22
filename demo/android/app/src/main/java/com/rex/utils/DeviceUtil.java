package com.rex.utils;

import android.content.Context;
import android.os.Build;
import android.provider.Settings;
import android.telephony.TelephonyManager;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * Created by Rex on 14-4-19.
 */
public class DeviceUtil {
    public static float screenDensity = 0;

    public static int getScreenWidth(Context context) {
        return context.getResources().getDisplayMetrics().widthPixels;
    }

    /**
     * get the height of the device screen
     *
     * @param context
     * @return
     */
    public static int getScreenHeight(Context context) {
        return context.getResources().getDisplayMetrics().heightPixels;
    }

    /**
     * get the density of the device screen
     *
     * @param context
     * @return
     */
    public static float getScreenDensity(Context context) {
        return context.getResources().getDisplayMetrics().density;
    }

    public static int getScreenDensityDpi(Context context){
        return context.getResources().getDisplayMetrics().densityDpi;
    }
    /**
     * dip to px
     *
     * @param context
     * @param dp
     * @return
     */
    public static int dip2px(Context context, float dp) {
        if(screenDensity==0) {
            screenDensity = getScreenDensity(context);
        }
        return (int) (dp * screenDensity + 0.5);
    }

    private static String deviceId = null;
    public static String deviceId(Context context){
        if(deviceId==null){
            String serial =  android.os.Build.SERIAL;
            String androidId =  Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID);
            String m_szDevIDShort = "35" + //we make this look like a valid IMEI
                    Build.BOARD.length()%10 +
                    Build.BRAND.length()%10 +
                    Build.CPU_ABI.length()%10 +
                    Build.DEVICE.length()%10 +
                    Build.DISPLAY.length()%10 +
                    Build.HOST.length()%10 +
                    Build.ID.length()%10 +
                    Build.MANUFACTURER.length()%10 +
                    Build.MODEL.length()%10 +
                    Build.PRODUCT.length()%10 +
                    Build.TAGS.length()%10 +
                    Build.TYPE.length()%10 +
                    Build.USER.length()%10 ;
            String m_szLongID = serial + m_szDevIDShort
                    + androidId;
            MessageDigest m = null;
            try {
                m = MessageDigest.getInstance("MD5");
            } catch (NoSuchAlgorithmException e) {
                e.printStackTrace();
            }
            m.update(m_szLongID.getBytes(),0,m_szLongID.length());
            // get md5 bytes
            byte p_md5Data[] = m.digest();
            // create a hex string
            String m_szUniqueID = new String();
            for (int i=0;i<p_md5Data.length;i++) {
                int b =  (0xFF & p_md5Data[i]);
                // if it is a single digit, make sure it have 0 in front (proper padding)
                if (b <= 0xF) {
                    m_szUniqueID += "0";
                }
                // add number to string
                m_szUniqueID+=Integer.toHexString(b);
            }
            // hex string to uppercase
            deviceId = m_szUniqueID.toUpperCase();

            return deviceId;
        }else
            return deviceId;
    }

    // imei
    public static String getDeviceId(Context context){
        try{
            TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
            return tm.getDeviceId();
        }catch (Exception e){
            return deviceId(context);
        }
    }

    // 手机号
    public static String getLine1Number(Context context){
        try {
            TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
            return tm.getLine1Number();
        }catch (Exception e){
            return "";
        }
    }
    // sim卡
    public static String getSimSerialNumber(Context context){
        try{
            TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
            return tm.getSimSerialNumber();
        }catch (Exception e){
            return "";
        }
    }

    public static String packageName;
    public static String getPackageName(Context context){
        packageName = context.getPackageName();
        return packageName;
    }

    public static String getDeviceModel(){
        return android.os.Build.MODEL;
    }

    public static String getSystemVersion(){
        return android.os.Build.VERSION.RELEASE;
    }

    public static String getManufacturer(){
        return android.os.Build.MANUFACTURER;
    }
}
