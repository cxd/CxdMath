//
//  ProjectTreeDelegate.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 25/04/10.
//  Copyright 2010 none. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "ProjectItem.h"
#import "ProjectItemType.h"
#import "ProjectTreeView.h"
#import	"RawDataPropertyData.h"
#import "NetworkProperties.h"

@interface ProjectTreeDelegate : NSObject<NSOutlineViewDelegate> {

	ProjectTreeView *projectTreeView;
	NSOutlineView *outline;
	NSTableView *propertyTable;
	RawDataPropertyData *rawDataProperty;
	NetworkProperties *netProperties;
}

@property(retain) IBOutlet ProjectTreeView *projectTreeView;

@property(retain) IBOutlet NSTableView *propertyTable;

@property(retain) IBOutlet RawDataPropertyData *rawDataProperty;

@property(retain) IBOutlet NetworkProperties *netProperties;
/**
 Weak reference 
 **/
@property(assign) IBOutlet NSOutlineView *outline;
/**
 Determine if the item is editable.
 **/
- (BOOL)outlineView:(NSOutlineView *)outlineView shouldEditTableColumn:(NSTableColumn *)tableColumn item:(id)item;


-(IBAction) onChooseFileClick:(id)sender;  
@end
