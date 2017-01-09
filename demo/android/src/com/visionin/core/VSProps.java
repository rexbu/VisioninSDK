package com.visionin.core;

import com.rex.load.NativeLoad;

/**
 * Created by Visionin on 16/11/25.
 */

public class VSProps {
    static {
        long so = NativeLoad.loadSo("libvisionin.so");
        NativeLoad.registJNIMethod(so, "com/visionin/core/VSProps", "startStProps", "(Ljava/lang/String;Z)Z");
        NativeLoad.registJNIMethod(so, "com/visionin/core/VSProps", "stopStProps", "()V");
        NativeLoad.registJNIMethod(so, "com/visionin/core/VSProps", "destroyProps", "()V");
    }

    public static native boolean startStProps(String propsZipPath, boolean mirror);
    public static native void stopStProps();
    public static native void destroyProps();
}
