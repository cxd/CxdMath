//
//  DataView.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 9/05/10.
//  Copyright 2010 none. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "IDataViewDelegate.h"
#import "DataViewDelegate.h"

/**
 
 This data view manages a table and interacts with the data view delegate to 
 load and display data.
 
 **/

@interface DataView : NSView {

	NSTableView *tableView;
	NSScrollView *scrollView;
	DataViewDelegate *delegate;
}

@property(retain) NSScrollView *scrollView;

@property(retain) NSTableView *tableView;

@property(retain) IBOutlet DataViewDelegate *delegate;


-(void)openTable:(NSString *)filePath;

/*
 Save the table view to the supplied file path.
 */
-(void)saveTable:(NSString *)filePath;


-(void)closeTable;

-(void)createTableColumns:(NSInteger)numCols;

- (void)addColumn:(NSNumber*)newid withTitle:(NSString*)title;

/**
 Force the data table to update.
 **/
-(void)redisplay;

@end
