//
//  NetworkProperties.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 25/05/10.
//  Copyright 2010 none. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "ProjectNotifications.h"
#import "InitialiseProjectConfig.h"

@interface NetworkProperties : NSObject<NSTableViewDataSource, NSTableViewDelegate, InitialiseProjectConfig> {
	NSArray *names;
	NSNumber *learnRate;
	NSNumber *momentum;
	NSNumber *bias;
	NSNumber *epochs;
	NSDictionary *mappedValues;
}

/**
Initialise project info.
**/
-(void)initialiseProjectInfo:(NetTrainProjectConfig *)config;

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView;

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;

-(void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;


-(NSCell *)tableView:(NSTableView *)tableView dataCellForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row;


-(BOOL)tableView:(NSTableView *)aTableView shouldEditTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;


@end
