//
//  ProjectTreeView.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 20/04/10.
//  Copyright 2010 none. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "CDGraphLib.h"
#import "ProjectItem.h"
#import "CxdTrainerModel.h"
#import "ProjectNotifications.h"
#import "InitialiseProjectConfig.h"



@interface ProjectTreeView : NSObject<NSOutlineViewDataSource, InitialiseProjectConfig> {

	CDGraph *graph;
	NetTrainProjectConfig *projectConfig;
	NSOutlineView *outlineView;
}

/**
 Graph instance.
 **/
@property(retain) CDGraph *graph;

/**
 reference to the project config.
 
 **/
@property(retain) NetTrainProjectConfig *projectConfig;


/**
 Parent outline view.
 **/
@property(retain) NSOutlineView *outlineView;

/**
 constructor
 **/
-(id)init;

/**
 deallocate.
 **/
-(void)dealloc;

/**
 Support for use in Nib files.
 **/
-(void)awakeFromNib;


/**
 Update project details.
 **/
-(void)initialiseProjectInfo:(NetTrainProjectConfig *)config;

/**
 Method to return the number of children of the current item.
 **/
-(NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item;


/**
 Method to identify whether the current item can be expanded
 **/
-(BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item;


/**
 Retrieve the nth child at the supplied index of the current item.
 **/
-(id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item;


/**
 retrieve the value for the table column.
 **/
-(id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item;

/**
 Set the object value for the selected item.
 **/
-(void)outlineView:(NSOutlineView *)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn *)tableColumn;


/**
 select a file based on a given item.
 **/
-(void)selectFileForItem:(ProjectItem *)item;

/*
 save a file based on a given item.
 */
-(void)saveFileForItem:(ProjectItem *)item;

@end
