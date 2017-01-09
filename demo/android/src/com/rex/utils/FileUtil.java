package com.rex.utils;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import java.io.*;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

/**
 * Created by Rex on 14-5-5.
 */
public class FileUtil {
    public static InputStream fileInputStream(String filePath) throws IOException{
        File file = new File(filePath);
        if(file.exists()){
            return new FileInputStream(file);
        }
        else{
            return null;
        }
    }

    public static OutputStream fileOutputStream(String filePath) throws IOException{
        File file = new File(filePath);
        String tempPath = file.getAbsolutePath();
        return new FileOutputStream(tempPath);
    }

    public static String read(InputStream inputStream) throws IOException{
        ByteArrayOutputStream outStream = new ByteArrayOutputStream();
        byte[] buf = new byte[1024];
        int len = -1;
        while((len = inputStream.read(buf)) != -1){
            outStream.write(buf, 0, len);
        }

        byte[] data = outStream.toByteArray();
        outStream.close();
        inputStream.close();
        return new String(data, "UTF-8");
    }

    public static boolean write(OutputStream outputStream, byte[] content, int offset, int size) throws IOException{
        outputStream.write(content, offset, size);
        outputStream.close();
        return true;
    }

    public static boolean write(OutputStream outputStream, String content) throws IOException{
        outputStream.write(content.getBytes("UTF-8"));
        outputStream.close();
        return true;
    }

    public static boolean write(OutputStream os, InputStream is) throws IOException {
        byte buf[] = new byte[1024];
        int len = -1;
        while((len = is.read(buf)) != -1){
            os.write(buf, 0, len);
        }
        is.close();
        os.close();

        return true;
    }

    public static boolean write(OutputStream os, InputStream is, IOCallback callback){
        byte buf[] = new byte[callback.split];
        int len = -1;
        int total = 0;
        try {
            while((len = is.read(buf)) != -1){
                os.write(buf, 0, len);
                total += len;
                callback.setParam(total*100/callback.length);
                callback.onRunning();
            }
            is.close();
            os.close();
            callback.onSuccess();
        } catch (IOException e) {
            e.printStackTrace();
            callback.onFailure(e);
        }

        return true;
    }

    public static boolean write(OutputStream os, InputStream is, TimerIOCallback callback){
        byte buf[] = new byte[1024];
        int len = -1;
        int total = 0;
        try {
            while((len = is.read(buf)) != -1){
                os.write(buf, 0, len);
                total += len;
                callback.setParam(total * 100 / callback.length);
            }
            is.close();
            os.close();
            callback.onSuccess();
        } catch (IOException e) {
            e.printStackTrace();
            callback.onFailure(e);
        }

        return true;
    }

    public static boolean exists(String pathString){
        File f = new File(pathString);
        if(!f.exists()){
            return false;
        }
        else{
            return true;
        }
    }

    public static void writeImage(Bitmap bitmap, OutputStream fileOutputStream){
        bitmap.compress(Bitmap.CompressFormat.JPEG, 50, fileOutputStream);
        try {
            fileOutputStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static Bitmap readPNG(String path){
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inScaled = false;
        return BitmapFactory.decodeFile(path, options);
    }

    public static Bitmap readPNG(byte[] bytes){
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inScaled = false;
        return BitmapFactory.decodeByteArray(bytes, 0, bytes.length, options);
    }

    public static boolean unzip(String src, String dest){
        ZipInputStream inZip = null;
        try {
            inZip = new ZipInputStream(new FileInputStream(src));
            ZipEntry zipEntry;
            String szName = "";
            while ((zipEntry = inZip.getNextEntry()) != null) {
                szName = zipEntry.getName();
                if (zipEntry.isDirectory()) {
                    szName = szName.substring(0, szName.length() - 1);
                    File folder = new File(dest + "/" + szName);
                    folder.mkdirs();
                } else {

                    File file = new File(dest + "/" + szName);
                    file.createNewFile();

                    FileOutputStream out = new FileOutputStream(file);
                    int len;
                    byte[] buffer = new byte[1024];

                    while ((len = inZip.read(buffer)) != -1) {
                        out.write(buffer, 0, len);
                        out.flush();
                    }

                    out.close();
                }
            }
            inZip.close();
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }
}
