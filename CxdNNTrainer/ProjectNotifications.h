//
//  ProjectNotifications.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 8/05/10.
//  Copyright 2010 none. All rights reserved.
//

#import <Cocoa/Cocoa.h>


// notifications issued by the project view.

extern NSString *const ProjectNameEditNotification;

extern NSString *const RawDataFilePathNotification;

extern NSString *const TargetDataFilePathNotification;

extern NSString *const TrainDataFilePathNotification;

extern NSString *const NetworkBaseFilePathNotification;

/**
 Start Target column value changed 
 **/
extern NSString * const TargetColumnEditNotification;

/**
 End target column value changed.
 **/
extern NSString *const EndTargetColumnEditNotification;

extern NSString *const TargetContinuousEditNotification;


extern NSString *const TrainEpochsEditNotification;

extern NSString *const TrainLearnRateEditNotification;

extern NSString *const TrainBiasEditNotification;

extern NSString *const TrainMomentumEditNotification;


extern NSString *const NetworkTrainFinished;

extern NSString *const NetworkTestFinished;

extern NSString *const NetworkErrorSignal;

@interface ProjectNotifications : NSObject {

}

@end
