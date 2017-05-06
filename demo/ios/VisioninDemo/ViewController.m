//
//  ViewController.m
//  VisioninDemo
//
//  Created by Visionin on 16/9/24.
//  Copyright © 2016年 Visionin. All rights reserved.
//

#import "ViewController.h"
#import "Visionin.h"
#import "VSVideoCamera.h"
#import "VSProps.h"
#import "VSFacer.h"

static bool initViewFlag = true;

@interface ViewController (){
    VSVideoCamera*  videoCamera;
    UITextView*     smoothView;
    UITextView*     shaperView;
    UITextView*     brightView;
    UITextView*     toneView;
    UIImageView*    videoView;
    UIButton*       shaper;
    NSArray*        propses;
    
    // 滤镜滑动框
    UICollectionView*   filterScrollView;
    NSMutableArray*     filterButtonArray;
    NSArray*            filterNameArray;
    
    UIInterfaceOrientation old_orientation;
}

@end

@implementation ViewController
BOOL canRotateToAllOrientations;

-(id) init{
    self = [super init];
    old_orientation = UIInterfaceOrientationUnknown;
    return self;
}
- (void)viewDidLoad {
    [super viewDidLoad];
    [self initVideoFrame];
}

-(void)initVideoFrame{
    [Visionin initialize:@"0x75,0x1,0x0,0x0,0x78,0x9c,0x7d,0x8e,0xc1,0x6f,0xc3,0x31,0xd,0x87,0xf0,0x3d,0x46,0xd6,0x74,0x87,0x8a,0xd5,0xd4,0x96,0x39,0x42,0xde,0xbb,0x4e,0x1f,0x8f,0xe6,0x8f,0xb,0x9d,0x47,0x70,0x8d,0x89,0x14,0xf3,0xc2,0xb8,0x34,0xba,0x47,0x39,0x23,0xc3,0x4,0x4,0x71,0xe4,0x1c,0x5e,0x9f,0xe1,0x66,0xd0,0xff,0x2c,0xd1,0xfd,0xbc,0xbd,0xe4,0x5f,0x9d,0x1d,0x56,0x95,0x28,0x73,0xfc,0x1f,0x1f,0x88,0xb1,0x2d,0xb6,0x98,0xb3,0xe2,0x10,0x8e,0x5a,0x9d,0xa8,0x6c,0x32,0x23,0x9b,0x75,0xa9,0xb7,0x8,0x11,0x44,0xc7,0x97,0x10,0x15,0xbd,0xa0,0x94,0x73,0xaf,0x35,0x4a,0x75,0xe1,0xd2,0x95,0xb0,0x2c,0x10,0xd4,0x1e,0x5d,0xa7,0xc3,0x7d,0x28,0xa8,0xca,0xfc,0xff,0x16,0x5,0x83,0x86,0x8a,0xca,0xbd,0xe1,0xe9,0x7,0xac,0x5f,0x4e,0x48,0xf1,0x35,0xd,0xe9,0xef,0xbd,0x6e,0x49,0x2d,0x6,0x8b,0xcf,0xb9,0x78,0x99,0x5b,0x4f,0xa0,0x11,0x2c,0x6e,0xc1,0x29,0x2c,0xd5,0x74,0xb5,0xc,0x4f,0xa2,0x9a,0x7,0xbe,0x8,0x2,0xa5,0x69,0xfd,0x18,0x40,0x90,0xcb,0xc3,0xa9,0xda,0xef,0x1e,0x59,0xad,0xd6,0x10,0x1c,0xd6,0x3f,0xa2,0xa9,0xe1,0x2,0x74,0xf6,0xa3,0xd9,0xf7,0x2f,0xb3,0x31,0x76,0xd1,"];
    videoCamera = [[VSVideoCamera alloc] initWithSessionPreset:AVCaptureSessionPresetHigh position:AVCaptureDevicePositionFront view:self.view];
    [videoCamera setMirrorFrontFacingCamera:TRUE];
    [videoCamera setMirrorFrontPreview:TRUE];
    [videoCamera setOutputSize:CGSizeMake(480, 640)];
    [videoCamera setOutputImageOrientation:UIInterfaceOrientationPortrait];
    // 如果设置了videoConnection.videoOrientation，则调用以下这句
    // [videoCamera setVideoOrientation:AVCaptureVideoOrientationPortrait];
    
    [videoCamera setSmoothStrength:0.9];
    // 设置滤镜，类型有origin、valencia、brannan、inkwell、pink、grass、beach、nature、sweety、clean、fresh、coral、sunset、vivid、lolita、crisp、rosy、urban、vintage
    [videoCamera setExtraFilter:@"origin"];
    
    // 开启人脸追踪
    VSFacer* facer = [VSFacer shareInstance];
    [facer startFaceTracking];
    // 开启整形，瘦脸大眼睛
    [facer startShaper];
    
    // 加载贴纸
    NSString* props = [[NSBundle mainBundle] pathForResource:@"cat" ofType:@"zip"];
    [[VSProps shareInstance] startLocalProps:props mirror:true];
    
    //    __block typeof(self) parent = self;
    //    [videoCamera setBgraPixelBlock:^(CVPixelBufferRef pixelBuffer, CMTime time) {
    //        // 获取处理后视频帧
    //        // NSLog(@"size: %d/%d", CVPixelBufferGetWidth(pixelBuffer), CVPixelBufferGetHeight(pixelBuffer));
    //        // 视频流预览
    //        if (parent->videoView!=nil) {
    //            // 此处对性能影响比较大，如果不用测试视频流是否正确，可以把以下代码关闭
    //            UIImage* image = pixelBuffer2Image(pixelBuffer);
    //            dispatch_async(dispatch_get_main_queue(), ^(){
    //                [parent->videoView setImage:image];
    //            });
    //        }
    //    }];
    
    // 以下是查看nv12格式的视频流
    //    [videoCamera setYuv420pPixelBlock:^(unsigned char * buffer, CMTime time) {
    //        if (parent->videoView!=nil && buffer!=NULL) {
    //            UIImage* image = yuv420Buffer2Image(buffer, 480, 640);
    //            dispatch_async(dispatch_get_main_queue(), ^(){
    //                [parent->videoView setImage:image];
    //            });
    //        }
    //    }];
    
}

- (void)initView{
    // 注意所有view的初始化都要放在VSVideoFrame的初始化之后，否则会被preview覆盖
    // 视频流预览
    videoView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 150, 135, 240)];
    [videoView setBackgroundColor:[UIColor whiteColor]];
    // [self.view addSubview:videoView];
    
    UIButton* record = [[UIButton alloc]initWithFrame:CGRectMake(10, 20, 60, 40)];
    [record setTitle:@"返回" forState:UIControlStateNormal];
    [record setBackgroundColor:[UIColor orangeColor]];
    [record addTarget:self action:@selector(back) forControlEvents:UIControlEventTouchUpInside];
    
    UIButton* stop = [[UIButton alloc]initWithFrame:CGRectMake(80, 20, 60, 40)];
    [stop setTitle:@"停止" forState:UIControlStateNormal];
    [stop setBackgroundColor:[UIColor orangeColor]];
    [stop addTarget:self action:@selector(stopVideo) forControlEvents:UIControlEventTouchUpInside];
    UIButton* rotate = [[UIButton alloc]initWithFrame:CGRectMake(150, 20, 60, 40)];
    [rotate setTitle:@"切换" forState:UIControlStateNormal];
    [rotate setBackgroundColor:[UIColor orangeColor]];
    [rotate addTarget:videoCamera action:@selector(rotateCamera) forControlEvents:UIControlEventTouchUpInside];
    
    shaper = [[UIButton alloc]initWithFrame:CGRectMake(220, 20, 60, 40)];
    [shaper setTitle:@"关整形" forState:UIControlStateNormal];
    [shaper setBackgroundColor:[UIColor orangeColor]];
    [shaper addTarget:self action:@selector(faceShaper) forControlEvents:UIControlEventTouchUpInside];
    
    UIButton* props = [[UIButton alloc]initWithFrame:CGRectMake(290, 20, 60, 40)];
    [props setTitle:@"贴纸" forState:UIControlStateNormal];
    [props setBackgroundColor:[UIColor orangeColor]];
    [props addTarget:self action:@selector(rotateProps) forControlEvents:UIControlEventTouchUpInside];
    
    [self.view addSubview:record];
    [self.view addSubview:stop];
    [self.view addSubview:rotate];
    [self.view addSubview:shaper];
    [self.view addSubview:props];
    
    UILabel* smoothLabel = [[UILabel alloc] initWithFrame:CGRectMake(10, 480, 40, 20)];
    [smoothLabel setText:@"美颜"];
    [self.view addSubview:smoothLabel];
    UISlider* smooth = [[UISlider alloc] initWithFrame:CGRectMake(60, 480, 300, 20)];
    smooth.minimumValue = 0.0;
    smooth.maximumValue = 1.0;
    smooth.value = 0.9;
    smooth.continuous = NO;
    [smooth addTarget:self action:@selector(smoothValueChanged:) forControlEvents:UIControlEventValueChanged];
    [self.view addSubview:smooth];
    
    UILabel* shaperLabel = [[UILabel alloc] initWithFrame:CGRectMake(10, 510, 40, 20)];
    [shaperLabel setText:@"整形"];
    [self.view addSubview:shaperLabel];
    UISlider* shaperSlider = [[UISlider alloc] initWithFrame:CGRectMake(60, 510, 300, 20)];
    shaperSlider.minimumValue = 0.0;
    shaperSlider.maximumValue = 1.0;
    shaperSlider.value = 0.5;
    shaperSlider.continuous = NO;
    [shaperSlider addTarget:self action:@selector(shaperValueChanged:) forControlEvents:UIControlEventValueChanged];
    [self.view addSubview:shaperSlider];
    
    smoothView = [[UITextView alloc] initWithFrame:CGRectMake(10, 80, 100, 30)];
    [smoothView setBackgroundColor:[UIColor clearColor]];
    NSString* text = [[NSString alloc] initWithFormat:@"磨皮:%0.2f", 0.5];
    [smoothView setText:text];
    [self.view addSubview:smoothView];
    
    shaperView = [[UITextView alloc] initWithFrame:CGRectMake(100, 80, 100, 30)];
    [shaperView setBackgroundColor:[UIColor clearColor]];
    text = [[NSString alloc] initWithFormat:@"整形:%0.2f", 0.9];
    [shaperView setText:text];
    [self.view addSubview:shaperView];
    
    // 滤镜滑动窗口
    filterNameArray = [NSArray arrayWithObjects:@"origin", @"nature", @"clean", @"vivid", @"fresh", @"sweety", @"rosy", @"lolita", @"sunset", @"grass", @"coral",
                       @"pink",@"urban", @"crisp", @"valencia", @"beach", @"vintage", @"rococo", @"walden", @"brannan", @"inkwell", nil];
    filterButtonArray = [[NSMutableArray alloc] init];
    UICollectionViewFlowLayout *flowLayout = [[UICollectionViewFlowLayout alloc] init];
    flowLayout.scrollDirection = UICollectionViewScrollDirectionHorizontal;
    filterScrollView = [[UICollectionView alloc] initWithFrame:CGRectMake(10, 530, 360, 40) collectionViewLayout:flowLayout];
    filterScrollView.backgroundColor = [UIColor clearColor];
    [self.view addSubview:filterScrollView];
    filterScrollView.delegate = self;
    filterScrollView.dataSource = self;
    
    [filterScrollView registerClass:[UICollectionViewCell class] forCellWithReuseIdentifier:@"myCell"];
    
    // 屏幕旋转
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(orientationChanged:) name:UIDeviceOrientationDidChangeNotification object:nil];
}

-(void)back{
    [videoCamera stopCameraCapture];
    [VSProps destroyInstance];
    [VSFacer destroyInstance];
    videoCamera = nil;
    initViewFlag = true;
    [self dismissViewControllerAnimated:YES completion:nil];
}

-(void)stopVideo{
    [videoCamera stopCameraCapture];
    videoCamera = nil;
}

- (void)viewDidLayoutSubviews {
    //self.view setBounds:CGRectMake(0, 0, self.view, CGFloat height)
    if (initViewFlag) {
        [self initView];
        initViewFlag = false;
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)smoothValueChanged:(id)sender{
    NSString* text = [[NSString alloc] initWithFormat:@"磨皮:%0.2f", [(UISlider*)sender value]];
    [smoothView setText:text];
    [videoCamera setSmoothStrength:[(UISlider*)sender value]];
}
-(void)shaperValueChanged:(id)sender{
    NSString* text = [[NSString alloc] initWithFormat:@"整形:%0.2f", [(UISlider*)sender value]];
    [shaperView setText:text];
    float strength = [(UISlider*)sender value];
    [[VSFacer shareInstance] setShapping:SHAPER_CMD_EYE strength:strength];
    [[VSFacer shareInstance] setShapping:SHAPER_CMD_FACE strength:strength];
    // [[VSFacer shareInstance] setShapping:SHAPER_CMD_CHIN strength:strength];
}
-(void)setNoneSmooth{
    [videoCamera setSmoothStrength:0];
}
-(void)rotateCamera{
    [videoCamera rotateCamera];
}
-(void)faceShaper{
    VSFacer* facer = [VSFacer shareInstance];
    if (facer.facerShaping ) {
        [facer stopShaper];
        [shaper setTitle:@"开整形" forState:UIControlStateNormal];
    }
    else{
        [facer startShaper];
        [shaper setTitle:@"关整形" forState:UIControlStateNormal];
    }
}

-(void)rotateProps{
    if ([VSProps shareInstance].currentProps!=nil) {
        [[VSProps shareInstance] stopProps];
    }
    else{
        [[VSProps shareInstance] startProps:@"cat" mirror:false];
    }
}

- (void) viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    //	We're going onto the screen, enable auto rotations
    canRotateToAllOrientations = YES;
    [videoCamera startCameraCapture];
}

- (void) viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    //	We're now on the screen, disable auto rotations
    canRotateToAllOrientations = YES;
}

-(void) viewWillDisappear:(BOOL)animated{
    [super viewWillDisappear:animated];
    old_orientation = UIInterfaceOrientationUnknown;
}

-(void) viewDidDisappear:(BOOL)animated{
    [super viewDidDisappear:animated];
    [videoCamera stopCameraCapture];
}

- (BOOL) shouldAutorotate
{
    return YES;
}
- (UIInterfaceOrientationMask)supportedInterfaceOrientations{
    return UIInterfaceOrientationMaskLandscapeRight|UIInterfaceOrientationMaskLandscapeLeft|UIInterfaceOrientationMaskPortrait|UIInterfaceOrientationMaskPortraitUpsideDown;
    //return UIInterfaceOrientationMaskPortrait;
}

- (void)orientationChanged:(NSNotification*)nofication{
    //UIDeviceOrientation orientation=[[UIDevice currentDevice] orientation];
    UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
    if (old_orientation == orientation) {
        return;
    }
    
    [videoCamera setViewFrame:self.view.bounds];
    
    old_orientation = orientation;
    
    switch (orientation) {
        case UIInterfaceOrientationPortraitUpsideDown:
            [videoCamera setOutputImageOrientation:UIInterfaceOrientationPortraitUpsideDown];
            break;
        case UIInterfaceOrientationLandscapeRight:
            [videoCamera setOutputImageOrientation:UIInterfaceOrientationLandscapeRight];
            break;
        case UIInterfaceOrientationLandscapeLeft:
            [videoCamera setOutputImageOrientation:UIInterfaceOrientationLandscapeLeft];
            break;
        case UIInterfaceOrientationPortrait:
            [videoCamera setOutputImageOrientation:UIInterfaceOrientationPortrait];
            break;
        default:
            break;
    }
    [self initView];
}

// 列数
- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    return filterNameArray.count;
}
// 行数
- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView
{
    return 1;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *cellID = @"myCell";
    UICollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:cellID forIndexPath:indexPath];
    if (cell.contentView.subviews.count <= 0) {
        UIButton* button = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, 60, 40)];
        [button setTitle:[filterNameArray objectAtIndex:indexPath.item] forState:UIControlStateNormal];
        [button setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
        [button setTag:indexPath.item];
        [button addTarget:self action:@selector(filterSelcted:) forControlEvents:UIControlEventTouchUpInside];
        [cell.contentView addSubview:button];
    }
    else{
        UIButton* button = (UIButton*)[cell.contentView.subviews objectAtIndex:0];
        [button setTitle:[filterNameArray objectAtIndex:indexPath.item] forState:UIControlStateNormal];
        [button setTag:indexPath.item];
    }
    
    return cell;
}

-(void)filterSelcted:(id)sender{
    UIButton* button = (UIButton*)sender;
    NSInteger tag = button.tag;
    [videoCamera setExtraFilter:[filterNameArray objectAtIndex:tag]];
}

//配置每个item的size
- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath
{
    return CGSizeMake(60, 40);;
}

//配置item的边距
- (UIEdgeInsets)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout insetForSectionAtIndex:(NSInteger)section
{
    return UIEdgeInsetsMake(5, 5, 5, 5);;
}

//点击item时触发
- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath
{
    [collectionView cellForItemAtIndexPath:indexPath].backgroundColor = [UIColor orangeColor];
}

//当前ite是否可以点击
- (BOOL) collectionView:(UICollectionView *)collectionView shouldSelectItemAtIndexPath:(nonnull NSIndexPath *)indexPath
{
    return YES;
}
@end
