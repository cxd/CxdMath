//
//  ProjectItem.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 20/04/10.
//  Copyright 2010 none. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "ProjectItemType.h"

@interface ProjectItem : NSObject {
	NSString *itemDetails;
	ProjectItemType type;
	int typeInt;
	BOOL hasFile; 
}

@property(retain) NSString *itemDetails;
@property(assign) ProjectItemType type;
@property(assign) BOOL hasFile;


-(int)typeInt;



-(id)initWithType:(ProjectItemType)t AndDetails:(NSString *)details;


@end
