package com.rex.load;

import android.content.Context;
import android.provider.Settings;

import com.rex.utils.DeviceUtil;

/**
 * Created by Visionin on 16/8/12.
 */
public class NativeLoad {
    static {
        System.loadLibrary("zr");
        System.loadLibrary("native");
    }

    public static long so;
    static public native long loadSo(String so);
    static public native int registJNIMethod(long so, String className, String funcName, String signature);

    /*
类型           相应的签名
boolean        Z
byte           B
char           C
short          S
int            I
long           J
float          F
double         D
void           V
object         L用/分隔包的完整类名：   Ljava/lang/String;
Array          [签名          [I      [Ljava/lang/Object;
Method         (参数1类型签名 参数2类型签名···)返回值类型签名
复制代码
特别注意：Object后面一定有分号（；）结束的,多个对象参数中间也用分号(;)来分隔
例子：
方法签名
void f1()                         ()V
int f2(int, long)                 (IJ)I
boolean f3(int[])                 ([I)B
double f4(String, int)            (Ljava/lang/String;I)D
void f5(int, String [], char)    (I[Ljava/lang/String;C)V
*/

}
