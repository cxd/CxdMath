//
//  NetworkProperties.m
//  CxdNNTrainer
//
//  Created by Chris Davey on 25/05/10.
//  Copyright 2010 none. All rights reserved.
//

#import "NetworkProperties.h"


@implementation NetworkProperties


-(id)init {
	self = [super init];
	
	learnRate = [NSNumber numberWithDouble:0.000001];
	[learnRate retain];
	momentum = [NSNumber numberWithDouble:0.0000001];
	[momentum retain];
	bias = [NSNumber numberWithDouble:0.0];
	[bias retain];
	epochs = [NSNumber numberWithInt:10000];
	[epochs retain];
	
	names = [[NSArray alloc] initWithObjects:
			 @"Training Epochs",
			 @"Learning  Rate",
			 @"Learning Bias",
			 @"Learning Momentum",
			 nil];
	
	return self;
}

-(void)dealloc
{
	[learnRate autorelease];
	[momentum autorelease];
	[bias autorelease];
	[epochs autorelease];
	[names autorelease];
	[super dealloc];
}

/**
 Initialise project info.
 **/
-(void)initialiseProjectInfo:(NetTrainProjectConfig *)config
{
	[epochs autorelease];
	epochs = [config valueForKey:@"epochs"];
	[learnRate autorelease];
	learnRate = [config valueForKey:@"learnRate"];
	[bias autorelease];
	bias = [config valueForKey:@"bias"];
	[momentum autorelease];
	momentum = [config valueForKey:@"momentum"];
}



- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
	return [names count];	
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	if ([[aTableColumn identifier] isEqualToString:@"title"])
	{
		if (rowIndex < [names count])
		{
			return [names objectAtIndex:rowIndex];	
		} else 
			return nil;
	}
	else {
		if (rowIndex == 0)
			return [epochs stringValue];
		else if (rowIndex == 1) 
			return [learnRate stringValue];
		else if (rowIndex == 2) {
			return [bias stringValue];
		} else {
			return [momentum stringValue];	
		}
		
	}
}

-(void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	if ([[aTableColumn identifier] isEqualToString:@"property"])
	{
		// assign the value.
		NSString *valStr = [[anObject copy] retain];
		NSString *notification;
		NSNumber *notifyValue;
		if (rowIndex == 0)
		{
			[epochs autorelease];
			epochs = [NSNumber numberWithInt:[valStr intValue]];
			notification = TrainEpochsEditNotification;
			notifyValue = epochs;
		} else if (rowIndex == 1) {
			[learnRate autorelease];
			learnRate = [NSNumber numberWithDouble:[valStr doubleValue]];
			notification = TrainLearnRateEditNotification;
			notifyValue = learnRate;
		} else if (rowIndex == 2) {
			[bias autorelease];
		    bias = [NSNumber numberWithDouble:[valStr doubleValue]];
			notification = TrainBiasEditNotification;
			notifyValue = bias;
		} else {
			[momentum autorelease];
		    momentum = [NSNumber numberWithDouble:[valStr doubleValue]];
			notification = TrainMomentumEditNotification;
			notifyValue = momentum;
		}
		[valStr autorelease];
		
		
		[[NSNotificationCenter defaultCenter]
		 postNotificationName:notification
		 object:self
		 userInfo:
		 [[NSDictionary alloc] initWithObjectsAndKeys:
		  notifyValue,
		  @"value", 
		  nil]
		 ];
		
	}
}





@end
