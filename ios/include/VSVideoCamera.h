//
//  VSRexContext.h
//  CaptureDemo
//
//  Created by Rex on 15/11/30.
//  Copyright © 2015年 Fantasist. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>
#import "VSVideoFrame.h"

#pragma mark - "Camera管理，获取原始视频流"
@protocol VSCameraSampleDelegate<NSObject>
-(void)willOutputAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer;
-(void)willOutputVideoSampleBuffer:(CMSampleBufferRef)sampleBuffer;
@end

@interface VSVideoCamera : VSVideoFrame<AVCaptureVideoDataOutputSampleBufferDelegate>
@property(nonatomic, copy)void (^videoSampleBufferBlock)(CMSampleBufferRef sampleBufer);
@property(nonatomic, copy)void (^audioSampleBufferBlock)(CMSampleBufferRef sampleBufer);

-(id)initWithSessionPreset:(NSString *)sessionPreset position:(AVCaptureDevicePosition)position view:(UIView*)view;
// 启动摄像头
-(void)startCameraCapture;
// 关闭摄像头
-(void)stopCameraCapture;

// 切换摄像头
-(void)rotateCamera;
-(AVCaptureDevicePosition)cameraPosition;

@property (nonatomic, assign) int32_t frameRate;

@property(nonatomic, retain) AVCaptureSession* captureSession;
@property(nonatomic, retain) AVCaptureDevice* captureDevice;
@property(nonatomic, retain) AVCaptureDevice* microphone;
@property(nonatomic, retain) AVCaptureDeviceInput* videoInput;
@property(nonatomic, retain) AVCaptureVideoDataOutput* videoOutput;
@property(nonatomic, retain) AVCaptureDeviceInput* audioInput;
@property(nonatomic, retain) AVCaptureAudioDataOutput* audioOutput;
@property(nonatomic, assign) AVCaptureVideoOrientation videoOrientation;
@end
