package com.rex.utils;

/**
 * Created by Rex on 14-5-19.
 */
public abstract class IOCallback {
    // 没个split调用一次callback
    public int length;
    public int split;
    public IOCallback(int length, int split){
        this.length = length;
        this.split = split;
    }

    // 设置参数
    public abstract void setParam(int param);
    public abstract void onRunning();
    public abstract void onSuccess();
    public abstract void onFailure(Exception e);
}
