//
//  VSRexContext.m
//  SimpleImageFilter
//
//  Created by Rex on 15/11/28.
//  Copyright © 2015年 Cell Phone. All rights reserved.
//

#import "VSVideoCamera.h"

#pragma mark - "VSVideoCamera"
@interface VSVideoCamera(){
    dispatch_queue_t cameraProcessingQueue, audioProcessingQueue, sampleProcessingQueue;
    BOOL capturePaused;
    
    void(^sampleBufferBlock)();
}
//@property(nonatomic, retain) GPUImageVideoCamera* camera;
@property(nonatomic, assign) CMSampleBufferRef sampleBuffer;
@property (readonly, getter = isFrontFacingCameraPresent) BOOL frontFacingCameraPresent;
@property (readonly, getter = isBackFacingCameraPresent) BOOL backFacingCameraPresent;


@end

@implementation VSVideoCamera
//@synthesize camera = camera;

VSVideoCamera* shareInstance = nil;
+(VSVideoCamera*)shareInstance{
    if (shareInstance==nil) {
        shareInstance = [[VSVideoCamera alloc] initWithSessionPreset:AVCaptureSessionPreset640x480 position:AVCaptureDevicePositionFront view:nil];
    }
    
    return shareInstance;
}

-(id)initWithSessionPreset:(NSString *)sessionPreset position:(AVCaptureDevicePosition)position view:(UIView*)view{
    BOOL asYuv = YES;
    _videoOrientation = -1;
    self = [super initWithPosition:position pixelFormat:kCVPixelFormatType_420YpCbCr8BiPlanarFullRange view:view];
    if (self==nil) {
        return nil;
    }
    
    cameraProcessingQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH,0);
    audioProcessingQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW,0);
    sampleProcessingQueue = dispatch_queue_create("com.visionin.sampleProcessingQueue", NULL);
    
    shareInstance = self;

    _captureDevice = nil;
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for (AVCaptureDevice *device in devices)
    {
        if ([device position] == position)
        {
            _captureDevice = device;
        }
    }
    
    if (!_captureDevice) {
        return nil;
    }
    
    // Create the capture session
    _captureSession = [[AVCaptureSession alloc] init];
    
    [_captureSession beginConfiguration];
    
    // Add the video input
    NSError *error = nil;
    _videoInput = [[AVCaptureDeviceInput alloc] initWithDevice:_captureDevice error:&error];
    if ([_captureSession canAddInput:_videoInput])
    {
        [_captureSession addInput:_videoInput];
    }
    
    // Add the video frame output
    _videoOutput = [[AVCaptureVideoDataOutput alloc] init];
    [_videoOutput setAlwaysDiscardsLateVideoFrames:NO];
    if (asYuv) {
        [_videoOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_420YpCbCr8BiPlanarFullRange] forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
    }
    else{
        [_videoOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
    }
    
    [_videoOutput setSampleBufferDelegate:self queue:cameraProcessingQueue];
    
    
    if ([_captureSession canAddOutput:_videoOutput])
    {
        [_captureSession addOutput:_videoOutput];
    }
    else
    {
        NSLog(@"Couldn't add video output");
        return nil;
    }
    
    [_captureSession setSessionPreset:sessionPreset];
    // 设置视频帧方向
    if (_videoOrientation!=-1) {
        self.videoOrientation = _videoOrientation;
    }
    
    [_captureSession commitConfiguration];
    //camera = [[GPUImageVideoCamera alloc] initWithSessionPreset:sessionPreset cameraPosition:position];
    //[camera setDelegate:self];
    return self;
}

-(void)setVideoOrientation:(AVCaptureVideoOrientation)videoOrientation{
    AVCaptureConnection *videoConnection = [_videoOutput connectionWithMediaType:AVMediaTypeVideo];
    videoConnection.videoOrientation = videoOrientation;
    [_captureSession commitConfiguration];
    
    [super setFrontVideoOrientation:videoOrientation];
    [super setBackVideoOrientation:videoOrientation];
}

- (void)dealloc
{
    [self stopCameraCapture];
    [_videoOutput setSampleBufferDelegate:nil queue:dispatch_get_main_queue()];
    [_audioOutput setSampleBufferDelegate:nil queue:dispatch_get_main_queue()];
    
    [self removeInputsAndOutputs];
    
    // ARC forbids explicit message send of 'release'; since iOS 6 even for dispatch_release() calls: stripping it out in that case is required.
#if !OS_OBJECT_USE_OBJC
    if (frameRenderingSemaphore != NULL)
    {
        dispatch_release(frameRenderingSemaphore);
    }
#endif
}

- (BOOL)addAudioInputsAndOutputs
{
    if (_audioOutput)
        return NO;
    
    [_captureSession beginConfiguration];
    
    _microphone = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeAudio];
    _audioInput = [AVCaptureDeviceInput deviceInputWithDevice:_microphone error:nil];
    if ([_captureSession canAddInput:_audioInput])
    {
        [_captureSession addInput:_audioInput];
    }
    _audioOutput = [[AVCaptureAudioDataOutput alloc] init];
    
    if ([_captureSession canAddOutput:_audioOutput])
    {
        [_captureSession addOutput:_audioOutput];
    }
    else
    {
        NSLog(@"Couldn't add audio output");
    }
    [_audioOutput setSampleBufferDelegate:self queue:audioProcessingQueue];
    
    [_captureSession commitConfiguration];
    return YES;
}

- (BOOL)removeAudioInputsAndOutputs
{
    if (!_audioOutput)
        return NO;
    
    [_captureSession beginConfiguration];
    [_captureSession removeInput:_audioInput];
    [_captureSession removeOutput:_audioOutput];
    _audioInput = nil;
    _audioOutput = nil;
    _microphone = nil;
    [_captureSession commitConfiguration];
    return YES;
}

- (void)removeInputsAndOutputs;
{
    [_captureSession beginConfiguration];
    if (_videoInput) {
        [_captureSession removeInput:_videoInput];
        [_captureSession removeOutput:_videoOutput];
        _videoInput = nil;
        _videoOutput = nil;
    }
    if (_microphone != nil)
    {
        [_captureSession removeInput:_audioInput];
        [_captureSession removeOutput:_audioOutput];
        _audioInput = nil;
        _audioOutput = nil;
        _microphone = nil;
    }
    [_captureSession commitConfiguration];
}

#pragma mark -
#pragma mark Manage the camera video stream

- (BOOL)isRunning;
{
    return [_captureSession isRunning];
}

- (void)startCameraCapture;
{
    if (![_captureSession isRunning])
    {
        [self startVideoFrame];
        [self addAudioInputsAndOutputs];
        [_captureSession startRunning];
    };
}

- (void)stopCameraCapture;
{
    if ([_captureSession isRunning])
    {
        [self removeAudioInputsAndOutputs];
        [_captureSession stopRunning];
    }
    [self stopVideoFrame];
}

- (void)pauseCameraCapture;
{
    capturePaused = YES;
}

- (void)resumeCameraCapture;
{
    capturePaused = NO;
}

- (void)rotateCamera
{
//    if (self.frontFacingCameraPresent == NO)
//        return;
    
    NSError *error;
    AVCaptureDeviceInput *newVideoInput;
    AVCaptureDevicePosition currentCameraPosition = [[_videoInput device] position];
    
    if (currentCameraPosition == AVCaptureDevicePositionBack)
    {
        currentCameraPosition = AVCaptureDevicePositionFront;
    }
    else
    {
        currentCameraPosition = AVCaptureDevicePositionBack;
    }
    
    AVCaptureDevice *backFacingCamera = nil;
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for (AVCaptureDevice *device in devices)
    {
        if ([device position] == currentCameraPosition)
        {
            backFacingCamera = device;
        }
    }
    newVideoInput = [[AVCaptureDeviceInput alloc] initWithDevice:backFacingCamera error:&error];
    
    if (newVideoInput != nil)
    {
        [_captureSession beginConfiguration];
        
        [_captureSession removeInput:_videoInput];
        if ([_captureSession canAddInput:newVideoInput])
        {
            [_captureSession addInput:newVideoInput];
            _videoInput = newVideoInput;
        }
        else
        {
            [_captureSession addInput:_videoInput];
        }
        //captureSession.sessionPreset = oriPreset;
        // 设置视频帧方向
        if (_videoOrientation!=-1) {
            self.videoOrientation = _videoOrientation;
        }
        [_captureSession commitConfiguration];
    }
    
    _captureDevice = backFacingCamera;
    
    [self setCameraPosition:[self cameraPosition]];
}

- (AVCaptureDevicePosition)cameraPosition
{
    return [[_videoInput device] position];
}

+ (BOOL)isBackFacingCameraPresent;
{
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    
    for (AVCaptureDevice *device in devices)
    {
        if ([device position] == AVCaptureDevicePositionBack)
            return YES;
    }
    
    return NO;
}

+ (BOOL)isFrontFacingCameraPresent;
{
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    
    for (AVCaptureDevice *device in devices)
    {
        if ([device position] == AVCaptureDevicePositionFront)
            return YES;
    }
    
    return NO;
}

- (void)setCaptureSessionPreset:(NSString *)captureSessionPreset;
{
    [_captureSession beginConfiguration];
    
    [_captureSession setSessionPreset:captureSessionPreset];
    
    [_captureSession commitConfiguration];
}

- (void)setFrameRate:(int32_t)frameRate;
{
    _frameRate = frameRate;
    
    if (_frameRate > 0)
    {
        if ([_captureDevice respondsToSelector:@selector(setActiveVideoMinFrameDuration:)] &&
            [_captureDevice respondsToSelector:@selector(setActiveVideoMaxFrameDuration:)]) {
            
            NSError *error;
            [_captureDevice lockForConfiguration:&error];
            if (error == nil) {
#if defined(__IPHONE_7_0)
                [_captureDevice setActiveVideoMinFrameDuration:CMTimeMake(1, _frameRate)];
                [_captureDevice setActiveVideoMaxFrameDuration:CMTimeMake(1, _frameRate)];
#endif
            }
            [_captureDevice unlockForConfiguration];
            
        } else {
            
            for (AVCaptureConnection *connection in _videoOutput.connections)
            {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
                if ([connection respondsToSelector:@selector(setVideoMinFrameDuration:)])
                    connection.videoMinFrameDuration = CMTimeMake(1, _frameRate);
                
                if ([connection respondsToSelector:@selector(setVideoMaxFrameDuration:)])
                    connection.videoMaxFrameDuration = CMTimeMake(1, _frameRate);
#pragma clang diagnostic pop
            }
        }
        
    }
    else
    {
        if ([_captureDevice respondsToSelector:@selector(setActiveVideoMinFrameDuration:)] &&
            [_captureDevice respondsToSelector:@selector(setActiveVideoMaxFrameDuration:)]) {
            
            NSError *error;
            [_captureDevice lockForConfiguration:&error];
            if (error == nil) {
#if defined(__IPHONE_7_0)
                [_captureDevice setActiveVideoMinFrameDuration:kCMTimeInvalid];
                [_captureDevice setActiveVideoMaxFrameDuration:kCMTimeInvalid];
#endif
            }
            [_captureDevice unlockForConfiguration];
            
        } else {
            
            for (AVCaptureConnection *connection in _videoOutput.connections)
            {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
                if ([connection respondsToSelector:@selector(setVideoMinFrameDuration:)])
                    connection.videoMinFrameDuration = kCMTimeInvalid; // This sets videoMinFrameDuration back to default
                
                if ([connection respondsToSelector:@selector(setVideoMaxFrameDuration:)])
                    connection.videoMaxFrameDuration = kCMTimeInvalid; // This sets videoMaxFrameDuration back to default
#pragma clang diagnostic pop
            }
        }
        
    }
}

- (AVCaptureConnection *)videoCaptureConnection {
    for (AVCaptureConnection *connection in [_videoOutput connections] ) {
        for ( AVCaptureInputPort *port in [connection inputPorts] ) {
            if ( [[port mediaType] isEqual:AVMediaTypeVideo] ) {
                return connection;
            }
        }
    }
    
    return nil;
}


/*- (CGFloat)averageFrameDurationDuringCapture;
{
    return (totalFrameTimeDuringCapture / (CGFloat)(numberOfFramesCaptured - INITIALFRAMESTOIGNOREFORBENCHMARK)) * 1000.0;
}

- (void)resetBenchmarkAverage;
{
    numberOfFramesCaptured = 0;
    totalFrameTimeDuringCapture = 0.0;
}
*/

#pragma mark -
#pragma mark AVCaptureVideoDataOutputSampleBufferDelegate

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    if (!self.captureSession.isRunning)
    {
        return;
    }
    else if (captureOutput == _audioOutput)
    {
        if (self.audioSampleBufferBlock)
        {
            self.audioSampleBufferBlock(sampleBuffer);
        }
        //[self processAudioSampleBuffer:sampleBuffer];
    }
    else
    {
        if (self.videoSampleBufferBlock)
        {
            self.videoSampleBufferBlock(sampleBuffer);
        }
        
        CFRetain(sampleBuffer);
        dispatch_async(sampleProcessingQueue, ^(){
            [self processVideoSampleBuffer:sampleBuffer];
            CFRelease(sampleBuffer);
        });
    }
}

@end
