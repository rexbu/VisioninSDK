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
+(void)setThirdPort:(BOOL)set;

///////////////////////////////////////////////////////////
///	n: 要求的人脸关键点个数，2/3/4/16/32/75
///	返回: 人脸关键点【xy,xy,...】，NULL代表人脸丢失
/*!	return key marks[][x,y]:
 [ 0,1 ]: eyeballs
 [  2  ]: mouth
 --------
 [  3  ]: nosetip
 --------
 [ 4,5 ]: inner eye corners
 [ 6,7 ]: outer eye corners
 [ 8,9 ]: eye line high points
 [10,11]: eyebow high points
 [ 12  ]: upper-lip
 [ 13  ]: lower-lip
 [14,15]: lip corners
 --------
 [16,17]: eye line low points
 [18,19]: inner eyebows
 [20,21]: outer eyebows
 [22,23]: lip line low points from center
 [24,25]: nose side points
 [ 26  ]: nose base
 [ 27  ]: jawtip
 [28,29]: cheek points
 [30,31]: temple points
 --------
 [32,33][34,35]: temple to cheek line points
 [36,37][38,39][40,41][42,43]: cheek to jaw line points
 [44,45]: eye line high points near inner corners
 [46,47]: eye line high points near outer corners
 [48,49]: eye line low points near inner corners
 [50,51]: eye line low points near outer corners
 [52,53]: eyebow points near inner corners
 [54,55]: eyebow points near outer corners
 [56,57][58,59]: lip line high points from corners
 [60,61]: lip line low points from corners
 [ 62  ]: upper-gum
 [ 63  ]: lower-gum
 [64,65]: gum corners
 [66,67]: gum high points from corners
 [68,69]: gum low points from corners
 [70,71]: nose bridge from root to tip
 [72,73]: nostrils
 [ 74  ]: nose root
 !*/
-(float*)getFacerMarks;
///////////////////////////////////////////////////////////
///	R9[9]: 非nil返回3x3旋转矩阵
///	O3[3]: 非nil返回两眼中心3维坐标（归一化到[-1,1]x[-1,1]）
///	P3[3]: 非nil返回围绕[X,Y,Z]轴的3维朝向（右手螺旋）
///	返回: 两眼间距（归一化到[-1,1]x[-1,1]）
-(float)getFacerPosingMatrix:(float*)R9 center:(float*)O3 angles:(float*)P3;

///////////////////////////////////////////////////////////
-(void)startShaper;
-(void)stopShaper;

///////////////////////////////////////////////////////////
///	emo: 人脸表情类型，见下定义"FacerEmo***"
///	返回: 表情程度，0代表无此表情，<0代表尚不支持的表情类型
-(float)emotion:(int)emo;

@end
