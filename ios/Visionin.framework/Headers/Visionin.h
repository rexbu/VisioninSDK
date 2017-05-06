//
//  Visionin.h
//  Visionin
//
//  Created by Rex on 16/2/25.
//  Copyright © 2016年 Rex. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VSVideoFrame.h"
#import "VSFacer.h"
#import "VSProps.h"

//! Project version number for Visionin.
FOUNDATION_EXPORT double VisioninVersionNumber;

//! Project version string for Visionin.
FOUNDATION_EXPORT const unsigned char VisioninVersionString[];

@interface Visionin : NSObject
// 返回错误情况：1、证书错误；2、资源文件解压失败
+(BOOL)initialize:(NSString*)license;
@end

#ifdef __cplusplus
extern "C"{
#endif
    UIImage* pixelBuffer2Image(CVPixelBufferRef pixelBuffer);
    UIImage* rgbaBuffer2Image(unsigned char* bytes, int width, int heihgt);
    UIImage* grayBuffer2Image(unsigned char* bytes, int width, int height);
    UIImage* yuv420Buffer2Image(unsigned char* byte, int width, int height);
#ifdef __cplusplus
}
#endif
