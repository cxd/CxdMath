//
//  InitialiseProjectConfig.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 8/05/10.
//  Copyright 2010 none. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "CxdTrainerModel.h";

@protocol InitialiseProjectConfig

/**
 Initialise project info.
 **/
-(void)initialiseProjectInfo:(NetTrainProjectConfig *)config;

@end
