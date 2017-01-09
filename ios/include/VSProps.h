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

typedef void (^CallbackList)(NSArray* res, id err);

@interface VSProps : NSObject
+(VSProps*)shareInstance;

@property(nonatomic, weak)NSString* currentProps;
@property(nonatomic, readonly)BOOL  propsStatus;

// 获取所有的道具列表
-(void)propses:(CallbackList)callback;
// 已经下载到本地的道具列表
-(NSArray*)localPropses;
// 获取本地某个道具
-(NSDictionary*)localProps:(NSString*)props;
// 本地是否存在该道具
-(BOOL)localExist:(NSString*)name;
// 开始道具，如果某个道具本地不存在，返回错误
-(BOOL)startProps:(NSString*)propsName;
// 下载道具包，progress：进度回调, 正常0-100，-1代表下载出错
-(void)downloadProps:(NSString*)propsName progress:(void(^)(int percent))progress complete:(void (^)(id error))complete;
-(void)stopProps;
@end
