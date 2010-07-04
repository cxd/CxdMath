//
//  ProjectItem.m
//  CxdNNTrainer
//
//  Created by Chris Davey on 20/04/10.
//  Copyright 2010 none. All rights reserved.
//

#import "ProjectItem.h"


@implementation ProjectItem


@synthesize itemDetails;
@synthesize type;
@synthesize hasFile;

-(id)initWithType:(ProjectItemType)t AndDetails:(NSString *)details
{
	self = [super init];
	self.itemDetails = details;
	self.type = t;
	return self;
}

-(void)dealloc
{
if (self.itemDetails != nil)
{
	[self.itemDetails dealloc];
	self.itemDetails = nil;
}
	[super dealloc];
}

-(int)typeInt
{
	return (int)self.type;
}

@end
