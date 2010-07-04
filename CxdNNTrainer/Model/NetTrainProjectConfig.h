//
//  NetTrainProjectConfig.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 4/06/10.
//  Copyright 2010 none. All rights reserved.
//

#import <CoreData/CoreData.h>

@class NetTrainResource;

@interface NetTrainProjectConfig :  NSManagedObject  
{
}

@property (nonatomic, retain) NSNumber * targetColumn;
@property (nonatomic, retain) NSString * classList;
@property (nonatomic, retain) NSNumber * endTargetColumn;
@property (nonatomic, retain) NSNumber * learnRate;
@property (nonatomic, retain) NSNumber * momentum;
@property (nonatomic, retain) NSNumber * isContinuous;
@property (nonatomic, retain) NSNumber * epochs;
@property (nonatomic, retain) NSNumber * bias;
@property (nonatomic, retain) NSString * name;
@property (nonatomic, retain) NetTrainResource * trainingSet;
@property (nonatomic, retain) NetTrainResource * trainingTargets;
@property (nonatomic, retain) NetTrainResource * rawData;
@property (nonatomic, retain) NetTrainResource * networkBaseFile;

@end



