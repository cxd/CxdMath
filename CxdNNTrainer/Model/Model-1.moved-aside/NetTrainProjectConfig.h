//
//  NetTrainProjectConfig.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 18/04/10.
//  Copyright 2010 none. All rights reserved.
//

#import <CoreData/CoreData.h>

@class NetTrainResource;

@interface NetTrainProjectConfig :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * name;
@property (nonatomic, retain) NetTrainResource * trainingSet;
@property (nonatomic, retain) NetTrainResource * trainingTargets;
@property (nonatomic, retain) NetTrainResource * networkBaseFile;
@property (nonatomic, retain) NetTrainResource * rawData;

@end



