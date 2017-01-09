package com.rex.utils;

import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by Rex on 14-8-18.
 */
public class LogUtil {
    public static void initialize(String path){
        LogUtil.path = path;
    }

    public static void log(String type, String s){
        try {
            Date date = new Date();
            SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
            SimpleDateFormat timeFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            FileWriter writer = new FileWriter(path+ dateFormat.format(date) +".log", true);
            writer.append("["+type+"]"+ timeFormat.format(date)+"\t\t"+ s+"\n");
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public static void debug(String s){
        log("debug", s);
    }
    public static void notice(String s){
        log("notice", s);
    }
    public static void err(String s){
        log("err", s);
    }
    private static String path = null;
}
