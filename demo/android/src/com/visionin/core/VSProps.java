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
        NativeLoad.registJNIMethod(so, "com/visionin/core/VSProps", "destroyProps", "()V");
    }

    public static native boolean startProps(String propsZipPath, boolean mirror);
    public static native void stopProps();
    public static native void destroyProps();
}
