//
//  DataView.m
//  CxdNNTrainer
//
//  Created by Chris Davey on 9/05/10.
//  Copyright 2010 none. All rights reserved.
//

#import "DataView.h"


@implementation DataView

@synthesize scrollView;
@synthesize tableView;
@synthesize delegate;

-(id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
		
        self.scrollView = [[NSScrollView alloc] initWithFrame:
						   NSMakeRect(frame.origin.x, frame.origin.y + 10, 
									  frame.size.width - 50, frame.size.height - 5) ];
		
		self.tableView = [[NSTableView alloc] initWithFrame:[self.scrollView bounds]];
		
		[self.scrollView setAutoresizingMask:
		 NSViewWidthSizable 
		 | NSViewHeightSizable 
		 | NSViewMaxXMargin 
		 | NSViewMinXMargin 
		 | NSViewMaxYMargin 
		 | NSViewMinYMargin];
		
		[self.scrollView setHasVerticalScroller:YES];
		[self.scrollView setHasHorizontalScroller:YES];
		
		[self.scrollView setDocumentView:self.tableView];
		[self addSubview:self.scrollView];
		self.delegate = [[DataViewDelegate alloc] init];
    }
    return self;
}

-(void)awakeFromNib
{
	self.scrollView = [[NSScrollView alloc] initWithFrame:
					   NSMakeRect([self frame].origin.x, 
								  [self frame].origin.y - 10, 
								  [self frame].size.width - 10, 
								  [self frame].size.height - 40) ];
	
	self.tableView = [[NSTableView alloc] initWithFrame:
					  NSMakeRect(
					  
								 [self.scrollView bounds].origin.x,
								 [self.scrollView bounds].origin.y - 10,
								 [self.scrollView bounds].size.width,
								 [self.scrollView bounds].size.height - 10)];
	
	[self.tableView setGridStyleMask:
	 NSTableViewSolidVerticalGridLineMask 
	 | NSTableViewSolidHorizontalGridLineMask];
	
	[self.tableView setUsesAlternatingRowBackgroundColors:YES];
	
	[self.scrollView setAutoresizingMask:
	 NSViewWidthSizable 
	 | NSViewHeightSizable 
	 | NSViewMaxXMargin 
	 | NSViewMinXMargin 
	 | NSViewMaxYMargin 
	 | NSViewMinYMargin];
	
	[self.scrollView setHasVerticalScroller:YES];
	[self.scrollView setHasHorizontalScroller:YES];
	
	[self addSubview:self.scrollView];
	self.delegate = [[DataViewDelegate alloc] init];
}



-(void)dealloc
{
	if (self.tableView != nil)
	{
		[self.tableView autorelease];
	}
	if (self.scrollView != nil)
	{
		[self.scrollView autorelease];	
	}
	if (self.delegate != nil)
	{
		[self.delegate autorelease];	
	}
	[super dealloc];	
}

-(void)drawRect:(NSRect)dirtyRect {
	
}


/**
 Open a table for display.
 **/
-(void)openTable:(NSString *)filePath
{
	[self closeTable];
	if (delegate == nil)
		return;
	if ([delegate loadDataAtPath:filePath])
	{
	// create the table.	
		[self createTableColumns:[delegate numCols]];
		[self.tableView setDelegate:delegate];
		[self.tableView setDataSource:delegate];
	}
	[self.tableView reloadData];
	[self.scrollView setDocumentView:self.tableView];
	//[self.tableView setNeedsDisplay:YES];
	[self.scrollView setNeedsDisplay:YES];
	[self setNeedsDisplay:YES];
}

-(void)closeTable
{
	for(NSTableColumn *col in [self.tableView tableColumns])
	{
		[self.tableView removeTableColumn:col];	
	}
	[self.tableView setDelegate:nil];
	[self.tableView setDataSource:nil];
	[self.tableView setNeedsDisplay:YES];
}



/**
 Force the data table to update.
 **/
-(void)redisplay
{
	if ([self.tableView numberOfColumns] != [delegate numCols])
	{
		[self.tableView setDataSource:nil];
		NSMutableArray *removable = [[NSMutableArray alloc] initWithCapacity:[[self.tableView tableColumns] count]];
		
		for(NSTableColumn *col in [self.tableView tableColumns])
		{
			[removable addObject:col];	
		}
		for(NSTableColumn *col in removable)
		{
			[self.tableView removeTableColumn:col];
		}
		[self createTableColumns:[delegate numCols]];
	}
	[self.tableView setDelegate:delegate];
	[self.tableView setDataSource:delegate];
	[self.tableView reloadData];
	[self.scrollView setDocumentView:self.tableView];
	[self.scrollView setNeedsDisplay:YES];
	[self setNeedsDisplay:YES];
}



/*
 Save the table view to the supplied file path.
 */
-(void)saveTable:(NSString *)filePath
{
	if (delegate == nil) 
		return;
	if ([delegate numRows] > 0 && [delegate numCols] > 0)
	{
		[delegate saveDataToPath:filePath];	
	}
}

-(void)createTableColumns:(NSInteger)numCols
{
	for(int i=0;i<numCols;i++)
	{
		NSNumber *num = [NSNumber numberWithInt:i];
		[self addColumn:num 
			  withTitle:[num stringValue]];
	}
}

- (void)addColumn:(NSNumber*)newid withTitle:(NSString*)title
{
	NSTableColumn *column=[[NSTableColumn alloc] initWithIdentifier:newid];
	if (title != nil)
	{
		[[column headerCell] setStringValue:title];
	}
	[[column headerCell] setAlignment:NSCenterTextAlignment];
	[column setWidth:70.0];
	[column setMinWidth:50];
	[column setEditable:YES];
	[column setResizingMask:NSTableColumnAutoresizingMask | NSTableColumnUserResizingMask];
	[self.tableView addTableColumn:column];
}

@end
