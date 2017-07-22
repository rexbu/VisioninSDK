package com.visionin.core;

import com.rex.load.NativeLoad;

/**
 * Created by Visionin on 16/11/25.
 */

public class VSProps {
    static {
        long so = NativeLoad.loadSo("libvisionin.so");
        NativeLoad.registJNIMethod(so, "com/visionin/core/VSProps", "startProps", "(Ljava/lang/String;Z)Z");
        NativeLoad.registJNIMethod(so, "com/visionin/core/VSProps", "stopProps", "()V");
        NativeLoad.registJNIMethod(so, "com/visionin/core/VSProps", "startProps2", "(Ljava/lang/String;Z)Z");
        NativeLoad.registJNIMethod(so, "com/visionin/core/VSProps", "stopProps2", "()V");

        NativeLoad.registJNIMethod(so, "com/visionin/core/VSProps", "setTextureProps", "(Ljava/lang/String;)Z");
        NativeLoad.registJNIMethod(so, "com/visionin/core/VSProps", "renderTextureProps", "(IIZ)Z");
        NativeLoad.registJNIMethod(so, "com/visionin/core/VSProps", "destroyProps", "()V");
    }

    /// 支持同时加载2个道具，用于直播过程中主播有一个道具，观众再送一个道具礼物
    public static native boolean startProps(String propsZipPath, boolean mirror);
    public static native void stopProps();
    public static native boolean startProps2(String propsZipPath, boolean mirror);
    public static native void stopProps2();

    public static native boolean setTextureProps(String jpath);
    public static native boolean renderTextureProps(int in_texture, int out_texture, boolean mirror);
    public static native void destroyProps();
}
