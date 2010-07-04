//
//  ProjectTreeDelegate.m
//  CxdNNTrainer
//
//  Created by Chris Davey on 25/04/10.
//  Copyright 2010 none. All rights reserved.
//

#import "ProjectTreeDelegate.h"


@implementation ProjectTreeDelegate

@synthesize projectTreeView;
@synthesize outline;
@synthesize propertyTable;
@synthesize rawDataProperty;
@synthesize netProperties;

-(id)init
{
	self = [super init];
	
	return self;
}

-(void)dealloc
{
	if (self.projectTreeView != nil)
	{
		[self.projectTreeView autorelease];
	}
	if (self.outline != nil)
	{
		[self.outline autorelease];	
	}
	if (self.propertyTable != nil)
	{
		[self.propertyTable autorelease];	
	}
	if (self.rawDataProperty != nil)
	{
		[self.rawDataProperty autorelease]; 	
	}
	if (self.netProperties != nil)
	{
		[self.netProperties autorelease];	
	}
	[super dealloc];
}




/**
 Determine if the item is editable.
 **/
- (BOOL)outlineView:(NSOutlineView *)outlineView shouldEditTableColumn:(NSTableColumn *)tableColumn item:(id)item
{
	if (item == nil)
		return NO;
	
	ProjectItem *pItem = (ProjectItem *)item;
	switch(pItem.type)
	{
		case ItemTitle:
			return NO;
		default:
			if ([[tableColumn identifier] isEqualTo:@"title"]) 
				return YES;
	}
	return NO;
}

/**
 Assign a button for the 2nd column of the project item.
 **/
- (NSCell *)outlineView:(NSOutlineView *)outlineView dataCellForTableColumn:(NSTableColumn *)tableColumn item:(id)item
{
	if (item == nil) return nil;
	ProjectItem *pItem = (ProjectItem*)item;
	NSString *idString = [tableColumn identifier];
	if ([idString isEqualToString:@"title"])
	{
		NSTextFieldCell *textCell = [[NSTextFieldCell alloc] initTextCell:pItem.itemDetails];
		[textCell setEditable:YES];
		return textCell;
	} else if (pItem.type != ProjectRoot && pItem.type != ItemTitle && [idString isEqualToString:@"actionColumn"]) {
		NSString *cellTitle = nil;
		switch(pItem.type)
		{
			case RawDataSource:
				cellTitle = @"Choose File";
				break;
			default:
				cellTitle = @"Save File";
				break;
		}
		NSButtonCell *btnCell = [[NSButtonCell alloc] initTextCell:cellTitle];
		int rowId = [outlineView rowForItem:item];
		[btnCell setIntValue:rowId]; // the row id can be used to retrieve the item at a later time.
		[btnCell setTarget:self];
		[btnCell setAction:@selector(onChooseFileClick:)];
		return btnCell;
	}
	return nil;
}



-(IBAction) onChooseFileClick:(id)sender
{
	NSOutlineView *outlineView = (NSOutlineView *)sender;
	int rowId = [outlineView clickedRow];
	ProjectItem *pItem = [outlineView itemAtRow:rowId];
	
	switch(pItem.type)
	{
		case RawDataSource:
			[self.projectTreeView selectFileForItem:pItem];
			break;
		default:
			[self.projectTreeView saveFileForItem:pItem];
	}
	
	// refresh the treeview data passing nil will reload the root.
	[outlineView reloadItem:nil reloadChildren:YES];
}

/**
 Capture the selection change.
 **/
-(void)outlineViewSelectionDidChange:(NSNotification *)notification
{
	int rowId = [self.outline selectedRow];
	ProjectItem *pItem = [self.outline itemAtRow:rowId];
	// ready to update property view.
	switch (pItem.type) {
		case RawDataSource:
			if (self.rawDataProperty != nil)
			{
				[self.propertyTable setDataSource:self.rawDataProperty];
				[self.propertyTable reloadData];
			}
			break;
		case ProjectRoot:
			if (self.netProperties != nil)
			{
				[self.propertyTable setDataSource:self.netProperties];
				[self.propertyTable reloadData];
			}
			break;
		default:
			break;
	}
}

@end
