//
//  Visionin.h
//  Visionin
//
//  Created by Rex on 16/2/25.
//  Copyright © 2016年 Rex. All rights reserved.
//
#import <Foundation/Foundation.h>

@interface Visionin : NSObject
+(void)initialize:(NSString*)appId appKey:(NSString*)appKey;
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
