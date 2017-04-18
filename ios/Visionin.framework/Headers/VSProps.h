//
//  Props.h
//  Props
//
//  Created by Visionin on 16/7/5.
//  Copyright © 2016年 Visionin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

@interface VSProps : NSObject
+(VSProps*)shareInstance;
+(void)destroyInstance;

@property(nonatomic, weak)NSString* currentProps;
@property(nonatomic, readonly)BOOL  propsStatus;

// 加载本地道具，不需要带zip后缀
-(BOOL)startProps:(NSString*)propsName mirror:(BOOL)mirror;

// 道具本地全路径，需要带zip后缀
-(BOOL)startLocalProps:(NSString*)propsPath mirror:(BOOL)mirror;
-(void)stopProps;
// 第二个道具，用于直播中，主播已经有了礼物，观众又送了一个基于人脸的额礼物
-(BOOL)startLocalProps2:(NSString*)propsPath mirror:(BOOL)mirror;
-(void)stopProps2;

//// 获取所有的道具列表
//-(void)propses:(void (^)(NSArray* ))propesBlock;
//// 已经下载到本地的道具列表
//-(NSArray*)localPropses;
//// 获取本地某个道具
//-(NSDictionary*)localProps:(NSString*)props;
//// 本地是否存在该道具
//-(BOOL)localExist:(NSString*)name;
//// 开始道具，如果某个道具本地不存在，返回错误
// 下载道具包，progress：进度回调, 正常0-100，-1代表下载出错
//-(void)downloadProps:(NSString*)propsName progress:(void(^)(int percent))progress complete:(void (^)(id error))complete;

@end
