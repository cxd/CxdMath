//
//  ProjectItemType.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 20/04/10.
//  Copyright 2010 none. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum _ProjectItemType {
	RawDataSource = 0,
	TrainDataSource = 1,
	TrainTargetDataSource = 2,
	NetworkDataFile = 3,
	ItemTitle = 4,
	ProjectRoot = 5 
} ProjectItemType;
