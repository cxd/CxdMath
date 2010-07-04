//
//  RawDataPropertyData.m
//  CxdNNTrainer
//
//  Created by Chris Davey on 26/04/10.
//  Copyright 2010 none. All rights reserved.
//

#import "RawDataPropertyData.h"


@implementation RawDataPropertyData

-(id)init 
{
	self = [super init];
	targetsColumn = 0;
	endTargetColumn = 0;
	names = [[NSArray alloc] initWithObjects:
			  @"Target Column",
			  @"End Target Column",
			  @"Continuous Target",
			 nil];
	isContinuous = 0;
	return self;
}

/**
 Initialise project info.
 **/
-(void)initialiseProjectInfo:(NetTrainProjectConfig *)config
{
	if (config == nil)
		return;
	targetsColumn = [config valueForKey:@"targetColumn"];
	endTargetColumn = [config valueForKey:@"endTargetColumn"];
	isContinuous = [config valueForKey:@"isContinuous"];
}


-(NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
	return [names count];	
}

-(id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
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
			return [targetsColumn stringValue];
		else if (rowIndex == 1) 
			return [endTargetColumn stringValue];
		else {
			return [isContinuous stringValue];
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
			[targetsColumn autorelease];
			targetsColumn = [NSNumber numberWithInt:[valStr intValue]];
			notification = TargetColumnEditNotification;
			notifyValue = targetsColumn;
		} else if (rowIndex == 1) {
			[endTargetColumn autorelease];
			endTargetColumn = [NSNumber numberWithInt:[valStr intValue]];
			notification = EndTargetColumnEditNotification;
			notifyValue = endTargetColumn;
		} else {
			[isContinuous autorelease];
		    isContinuous = [NSNumber numberWithInt:[valStr intValue]];
			notification = TargetContinuousEditNotification;
			notifyValue = isContinuous;
		}
		[valStr autorelease];
		
		
		[[NSNotificationCenter defaultCenter]
			postNotificationName:notification
							object:self
							userInfo:
								[[NSDictionary alloc] initWithObjectsAndKeys:
										notifyValue,
									@"TargetsColumn", 
								 nil]
		 ];
		
	}
}


-(NSCell *)tableView:(NSTableView *)tableView dataCellForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
	NSString *txt = (row == 0) ? @"Target Column" : @"End Target Column";
	if ([[tableColumn identifier] isEqualToString:@"property"])
	{
		txt = ((row == 0) ? [targetsColumn stringValue] : [endTargetColumn stringValue]);
	}
	NSTextFieldCell *textCell = [[NSTextFieldCell alloc] initTextCell:txt];
	[textCell setEditable:NO];
	if ([[tableColumn identifier] isEqualToString:@"property"])
	{
		[textCell setEditable:YES];
	}
	return textCell;
}


-(BOOL)tableView:(NSTableView *)aTableView shouldEditTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	return ![[aTableColumn identifier] isEqualToString:@"title"];
}


@end
