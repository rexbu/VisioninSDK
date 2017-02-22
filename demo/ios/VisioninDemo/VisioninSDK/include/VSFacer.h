//
//  Facer.h
//  Facer
//
//  Created by Visionin on 16/7/5.
//  Copyright © 2016年 Visionin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

@interface VSFacer : NSObject

@property(nonatomic, assign) BOOL faceTracking;
@property(nonatomic, assign) BOOL facerShaping;
// 是否显示人脸追踪的标记
@property(nonatomic, assign) BOOL  faceMarking;

+(VSFacer*)shareInstance;

// 开启人脸追踪
-(void)startFaceTracking;
-(void)stopFaceTracking;
// 返回人脸68点关键点
-(float*)getFacerMarks;

-(void)startShaper;
-(void)stopShaper;
/**
 * 整形强度，cmd为整形部位
 * stregnth: 0-1. 0-1为推荐区间，可大于1实现夸张效果
 */
-(void)setShapping:(int)cmd strength:(float)strength;

@end

#define SHAPER_CMD_EYE  1
#define SHAPER_CMD_FACE 2
