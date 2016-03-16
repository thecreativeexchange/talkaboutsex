//
//  GameViewController.h
//
//  Created by Gavin Wood on 07/11/2012.
//  Copyright (c) 2012 Gavin Wood. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreMotion/CoreMotion.h>
#import <CoreLocation/CoreLocation.h>

@interface GameViewController : GLKViewController
<
UIImagePickerControllerDelegate,
AVAudioPlayerDelegate
>
{
    CMMotionManager                     *motionManager;             // To capture movement events
    AVAudioPlayer                       *audioPlayer;               // To play audio
    CLLocationManager                   *locationManager;           // To capture location
    AVCaptureSession                    *captureSession;
    
    IBOutlet UILabel                    *mText;
    IBOutlet UILabel                    *mText2;
    IBOutlet UILabel                    *mText3;
}

@property (nonatomic, retain) CMMotionManager *motionManager;
@property (nonatomic, retain) UILabel *mText;
@property (nonatomic, retain) UILabel *mText2;
@property (nonatomic, retain) UILabel *mText3;

@end
