//
//  Facer.h
//  Facer
//
//  Created by Visionin on 16/7/5.
//  Copyright © 2016年 Visionin. All rights reserved.
//

#import "GL.h"
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

@interface VSFacer : NSObject

@property(nonatomic, assign) BOOL faceTracking;
@property(nonatomic, assign) BOOL facerShaping;
// 是否显示人脸追踪的标记
@property(nonatomic, assign) BOOL  faceMarking;
@property(nonatomic, assign) gpu_orientation_t faceOrientation; // 人脸方向

+(VSFacer*)shareInstance;
+(void)destroyInstance;

// 开启人脸追踪
-(void)startFaceTracking;
-(void)stopFaceTracking;

// 返回第i个人脸68点关键点，无人脸返回NULL。
-(float*)getFacerMarks:(int)i;
// 返回第i个人脸的3维角度，分别用3个float表示，分别为水平转角度、俯仰角、旋转角，无人脸返回NULL
// float[0] 水平转角，左负右正
// float[1]，俯仰角，上负下正
// float[2]，旋转角，左负右正
-(float*)get3DAngle:(int)i;
// 第i个人当前帧是否在在做cmd对应的表情动作
-(BOOL)emotion:(int)cmd index:(int)i;

// 开启与关闭整形
-(void)startShaper;
-(void)stopShaper;
// 设置Marker
-(void)setMarder:(BOOL)marder;
/**
 * 整形强度，cmd为整形部位
 * stregnth: 0-1. 0-1为推荐区间，可大于1实现夸张效果
 */
-(void)setShapping:(int)cmd strength:(float)strength;

@end

// 整形指令
#define SHAPER_CMD_EYE  1
#define SHAPER_CMD_FACE 2
#define SHAPER_CMD_CHIN 3
#define SHAPER_CMD_NOSE 4
#define SAHPER_CMD_CHEEK    5

// 表情指令
#define VS_EMOTION_BLINK_EYE        0x00000002    //  眨眼
#define VS_EMOTION_OPEN_MOUTH       0x00000004    //  张嘴
#define VS_EMOTION_YAW_HEAD         0x00000008    //  摇头
#define VS_EMOTION_PITCH_HEAD       0x00000010    //  点头
#define VS_EMOTION_JUMP_BROW        0x00000020    //  挑眉
