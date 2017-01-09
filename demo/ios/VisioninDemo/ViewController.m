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

@interface ViewController (){
    VSVideoCamera*  videoCamera;
    UITextView*     smoothView;
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
    [Visionin initialize:@"f1a87ce5ae57f1e2484283419c3ef277" appKey:@"e1f638ec0d52ac96b5e3fc8242813df7"];
    videoCamera = [[VSVideoCamera alloc] initWithSessionPreset:AVCaptureSessionPresetHigh position:AVCaptureDevicePositionFront view:self.view];
    [videoCamera setMirrorFrontFacingCamera:TRUE];
    [videoCamera setMirrorFrontPreview:TRUE];
    [videoCamera setOutputSize:CGSizeMake(480, 640)];
    [videoCamera setOutputImageOrientation:UIInterfaceOrientationPortrait];
    // 如果设置了videoConnection.videoOrientation，则调用以下这句
    // [videoCamera setVideoOrientation:AVCaptureVideoOrientationPortrait];
    
    [videoCamera setSmoothLevel:0.9];
    // 设置滤镜，类型有origin、valencia、brannan、inkwell、pink、grass、beach、nature、sweety、clean、fresh、coral、sunset、vivid、lolita、crisp、rosy、urban、vintage
    [videoCamera setExtraFilter:@"origin"];
    
    // 开启人脸追踪
    VSFacer* facer = [VSFacer shareInstance];
    [facer startFaceTracking];
    // 开启整形，瘦脸大眼睛
    [facer startShaper];
    
    // 加载贴纸
    [[VSProps shareInstance] startProps:@"rabbit"];
    
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
    
    // 开启摄像头
    [videoCamera startCameraCapture];
}

- (void)initView{
    // 注意所有view的初始化都要放在VSVideoFrame的初始化之后，否则会被preview覆盖
    // 视频流预览
    videoView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 150, 135, 240)];
    [videoView setBackgroundColor:[UIColor whiteColor]];
    // [self.view addSubview:videoView];
    
    UIButton* record = [[UIButton alloc]initWithFrame:CGRectMake(10, 20, 60, 40)];
    [record setTitle:@"开始" forState:UIControlStateNormal];
    [record setBackgroundColor:[UIColor orangeColor]];
    //[record addTarget:self action:@selector(startRecode) forControlEvents:UIControlEventTouchUpInside];
    [record addTarget:videoCamera action:@selector(startCameraCapture) forControlEvents:UIControlEventTouchUpInside];
    UIButton* stop = [[UIButton alloc]initWithFrame:CGRectMake(80, 20, 60, 40)];
    [stop setTitle:@"停止" forState:UIControlStateNormal];
    [stop setBackgroundColor:[UIColor orangeColor]];
    //[stop addTarget:videoCamera action:@selector(stopRecode) forControlEvents:UIControlEventTouchUpInside];
    [stop addTarget:videoCamera action:@selector(stopCameraCapture) forControlEvents:UIControlEventTouchUpInside];
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
    
    UISlider* smooth = [[UISlider alloc] initWithFrame:CGRectMake(10, 570, 360, 20)];
    smooth.minimumValue = 0.0;
    smooth.maximumValue = 1.0;
    smooth.value = 0.9;
    smooth.continuous = NO;
    [smooth addTarget:self action:@selector(smoothValueChanged:) forControlEvents:UIControlEventValueChanged];
    [self.view addSubview:smooth];
    
    smoothView = [[UITextView alloc] initWithFrame:CGRectMake(10, 80, 100, 30)];
    [smoothView setBackgroundColor:[UIColor clearColor]];
    NSString* text = [[NSString alloc] initWithFormat:@"磨皮:%0.2f", 0.9];
    [smoothView setText:text];
    
    [self.view addSubview:smoothView];
    
    // 滤镜滑动窗口
    filterNameArray = [NSArray arrayWithObjects:@"origin", @"nature", @"clean", @"vivid", @"fresh", @"sweety", @"rosy", @"lolita", @"sunset", @"grass", @"coral",
                       @"pink",@"urban", @"crisp", @"valencia", @"beach", @"vintage", @"rococo", @"walden", @"brannan", @"inkwell", nil];
    filterButtonArray = [[NSMutableArray alloc] init];
    UICollectionViewFlowLayout *flowLayout = [[UICollectionViewFlowLayout alloc] init];
    flowLayout.scrollDirection = UICollectionViewScrollDirectionHorizontal;
    filterScrollView = [[UICollectionView alloc] initWithFrame:CGRectMake(10, 600, 360, 40) collectionViewLayout:flowLayout];
    filterScrollView.backgroundColor = [UIColor clearColor];
    [self.view addSubview:filterScrollView];
    filterScrollView.delegate = self;
    filterScrollView.dataSource = self;
    
    [filterScrollView registerClass:[UICollectionViewCell class] forCellWithReuseIdentifier:@"myCell"];
    
    // 屏幕旋转
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(orientationChanged:) name:UIDeviceOrientationDidChangeNotification object:nil];
}

- (void)viewDidLayoutSubviews {
    //self.view setBounds:CGRectMake(0, 0, self.view, CGFloat height)
    static bool flag = true;
    if (flag) {
        [self initView];
        flag = false;
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)smoothValueChanged:(id)sender{
    NSString* text = [[NSString alloc] initWithFormat:@"磨皮:%0.2f", [(UISlider*)sender value]];
    [smoothView setText:text];
    [videoCamera setSmoothLevel:[(UISlider*)sender value]];
}

-(void)setNoneSmooth{
    [videoCamera setSmoothLevel:0];
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
        [[VSProps shareInstance] startProps:@"rabbit"];
    }
}

- (void) viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    //	We're going onto the screen, enable auto rotations
    canRotateToAllOrientations = YES;
}

- (void) viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    //	We're now on the screen, disable auto rotations
    canRotateToAllOrientations = YES;
}
-(void) viewWillDisappear:(BOOL)animated{
    old_orientation = UIInterfaceOrientationUnknown;
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
    [videoCamera setPreview:self.view];
    //    if (old_orientation == orientation) {
    //        return;
    //    }
    
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
