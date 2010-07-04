//
//  ProjectTreeView.m
//  CxdNNTrainer
//
//  Created by Chris Davey on 20/04/10.
//  Copyright 2010 none. All rights reserved.
//

#import "ProjectTreeView.h"
#import "ProjectItem.h"


@implementation ProjectTreeView



/**
 Graph instance.
 **/
@synthesize graph;

/**
 Project config assigned from the document owner.
 **/
@synthesize projectConfig;

@synthesize outlineView;

/**
 constructor
 **/
-(id)init
{
	self = [super init];
	self.graph = [[CDGraph alloc] init];
	
	
	return self;
}

/**
 deallocate.
 **/
-(void)dealloc
{
	if (self.graph != nil)
	{
		[self.graph dealloc];
		self.graph = nil;
	}
	// unassign weak reference.
	if (self.projectConfig != nil)
	{
		[self.projectConfig autorelease];	
	}
	if (self.outlineView != nil)
	{
		[self.outlineView autorelease];	
	}
	[super dealloc];
}


/**
 Support for use in Nib files.
 **/
-(void)awakeFromNib
{
	
	// create static project structure.
	ProjectItem *project = nil;
	
	project = [[ProjectItem alloc] initWithType:ProjectRoot
										 AndDetails:@"New Project"];
		
		
	CDNode *projectNode = [self.graph add:project];
	
	
	ProjectItem *rawTitle = [[ProjectItem alloc] initWithType:ItemTitle
												   AndDetails:@"Raw Data Source"];
	
		
		
	ProjectItem *rawSource = nil;
	
	NetTrainResource *resource = nil;
	
	if (self.projectConfig != nil)
		resource = [self.projectConfig valueForKey:@"rawData"];
	
	if (resource != nil)
	{
		rawSource = [[ProjectItem alloc] initWithType:RawDataSource
											AndDetails:[resource valueForKey:@"location"]];
	} else {
		rawSource = [[ProjectItem alloc] initWithType:RawDataSource
										   AndDetails:@""];
	}
	CDNode *titleNode = [self.graph add:rawTitle];
	CDNode *dataNode = [self.graph add:rawSource];
	
	[self.graph 
	 connect:projectNode 
	 to:titleNode];
	
	[self.graph 
	 connect:titleNode
	 to:dataNode];
	
	ProjectItem *trainTitle = [[ProjectItem alloc] initWithType:ItemTitle
													 AndDetails:@"Training Data"];
	
	ProjectItem *trainSource = [[ProjectItem alloc] initWithType:TrainDataSource
													  AndDetails:@""];
	
	
	[self.graph add:trainTitle];
	[self.graph add:trainSource];
	
	[self.graph connect:[self.graph find:[NSPredicate
										  predicateWithFormat:@"(self.itemDetails == %@) AND (self.typeInt == %i)",
										  @"Training Data", (int)ItemTitle]]
					 to:[self.graph find:[NSPredicate
										  predicateWithFormat:@"self.typeInt == %i",
										  (int)TrainDataSource]]
	 ];
	
	[self.graph connect:[self.graph find:[NSPredicate
										  predicateWithFormat:@"type == %i",
										  (int)ProjectRoot]]
					 to:[self.graph find:[NSPredicate
										  predicateWithFormat:
										  @"(itemDetails == %@) AND (type == %i)",
										  @"Training Data", (int)ItemTitle]]
	 ];
	
	ProjectItem *targetTitle = [[ProjectItem alloc] initWithType:ItemTitle
													  AndDetails:@"Training Targets"];
	
	ProjectItem *targetSource = [[ProjectItem alloc] initWithType:TrainTargetDataSource
													   AndDetails:@""];
	
	
	[self.graph add:targetTitle];
	[self.graph add:targetSource];
	[self.graph connect:[self.graph find:[NSPredicate
										  predicateWithFormat:
										  @"(itemDetails == %@) AND (type == %i)",
										  @"Training Targets", (int)ItemTitle]]
					 to:[self.graph find:[NSPredicate
										  predicateWithFormat:@"type == %i",
										  (int)TrainTargetDataSource]]
	 ];
	
	[self.graph connect:[self.graph find:[NSPredicate
										  predicateWithFormat:@"type == %i",
										  (int)ProjectRoot]]
					 to:[self.graph find:[NSPredicate
										  predicateWithFormat:
										  @"(itemDetails == %@) AND (type == %i)",
										  @"Training Targets", (int)ItemTitle]]
	 ];
	
	ProjectItem *netTitle = [[ProjectItem alloc] initWithType:ItemTitle
												   AndDetails:@"Network File"];
	
	ProjectItem *netSource = [[ProjectItem alloc] initWithType:NetworkDataFile
													AndDetails:@""];
	
	
	[self.graph add:netTitle];
	[self.graph add:netSource];
	[self.graph connect:[self.graph find:[NSPredicate
										  predicateWithFormat:
										  @"(itemDetails == %@) AND (type == %i)",
										  @"Network File", (int)ItemTitle]]
					 to:[self.graph find:[NSPredicate
										  predicateWithFormat:@"type == %i",
										  (int)NetworkDataFile]]
	 ];
	
	[self.graph connect:[self.graph find:[NSPredicate
										  predicateWithFormat:@"type == %i",
										  (int)ProjectRoot]]
					 to:[self.graph find:[NSPredicate
										  predicateWithFormat:
										  @"(itemDetails == %@) AND (type == %i)",
										  @"Network File", (int)ItemTitle]]
	 ];
}


/**
 Update project details.
 **/
-(void)initialiseProjectInfo:(NetTrainProjectConfig *)config;
{
	
	if (config == nil)
		return;
	
	self.projectConfig = config;
	
	NetTrainResource *rawRes = [self.projectConfig valueForKey:@"rawData"];
	NetTrainResource *trainRes = [self.projectConfig valueForKey:@"trainingSet"];
	NetTrainResource *targetRes = [self.projectConfig valueForKey:@"trainingTargets"];
	NetTrainResource *netRes = [self.projectConfig valueForKey:@"networkBaseFile"];
	
	CDNode *projectNode = [self.graph find:[NSPredicate
											predicateWithFormat:@"self.typeInt == %i",
											(int)ProjectRoot]];
	((ProjectItem*)projectNode.data).itemDetails = [self.projectConfig valueForKey:@"name"];
	
	
	if (rawRes != nil)
	{
		CDNode *node = [self.graph find:[NSPredicate
											predicateWithFormat:@"self.typeInt == %i",
											(int)RawDataSource]];
		((ProjectItem*)node.data).itemDetails = [rawRes valueForKey:@"location"];
	}
	
	
	if (trainRes != nil)
	{
		CDNode *node = [self.graph find:[NSPredicate
										 predicateWithFormat:@"self.typeInt == %i",
										 (int)TrainDataSource]];
		((ProjectItem*)node.data).itemDetails = [trainRes valueForKey:@"location"];
	}
	
	
	if (targetRes != nil)
	{
		CDNode *node = [self.graph find:[NSPredicate
										 predicateWithFormat:@"self.typeInt == %i",
										 (int)TrainTargetDataSource]];
		((ProjectItem*)node.data).itemDetails = [targetRes valueForKey:@"location"];
	}
	
	
	if (netRes != nil)
	{
		CDNode *node = [self.graph find:[NSPredicate
										 predicateWithFormat:@"self.typeInt == %i",
										 (int)NetworkDataFile]];
		((ProjectItem*)node.data).itemDetails = [netRes valueForKey:@"location"];
	}
	[self.outlineView reloadData];
}

/**
 Method to return the number of children of the current item.
 
 Only ItemTitles have children.
 
 **/
-(NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item
{
	if (item == nil)
		return 1;
	if ( ((ProjectItem*)item).type == ItemTitle)
		return 1;
	if ( ((ProjectItem*)item).type == ProjectRoot)
		return 4;
	return 0;
}


/**
 Method to identify whether the current item can be expanded
 **/
-(BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item
{
	return (item == nil) || 
	( ((ProjectItem*)item).type == ItemTitle)  ||
	( ((ProjectItem*)item).type == ProjectRoot);	
}


/**
 Retrieve the nth child at the supplied index of the current item.
 **/
-(id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item
{
	if (item == nil)
	{
		return ((CDNode *)[self.graph.nodes objectAtIndex:0]).data;
	}
	BOOL hasChild = ( ( ((ProjectItem*)item).type == ItemTitle) ||
					 ( ((ProjectItem*)item).type == ProjectRoot) );
	if (!hasChild) 
		return nil;
	
	CDNode *node = [self.graph find:[NSPredicate
					  predicateWithFormat:@"(itemDetails == %@) AND (type == %i)",
					  ((ProjectItem*)item).itemDetails, (int)((ProjectItem*)item).type]];
	
	
	CDNode *next = nil;
	if (index < [node.neighbours count])
	{
		next= [node.neighbours objectAtIndex:index];
	}
	if (next != nil)
	{
		return next.data;
	}
	
	return nil;	
}


/**
 retrieve the value for the table column.
 **/
-(id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
	if (item == nil)
		return nil;
	NSString *identifier = [tableColumn identifier];
	
	if (![identifier isEqualToString:@"actionColumn"])
		return ((ProjectItem*)item).itemDetails;
	return nil;
}

/**
 Set the object value for the selected item.
 **/
-(void)outlineView:(NSOutlineView *)outlineView setObjectValue:(id)object 
	forTableColumn:(NSTableColumn *)tableColumn 
			byItem:(id)item
{
	if (item == nil)
		return;
	if ([object isKindOfClass: [NSString class]])
		((ProjectItem*)item).itemDetails = object;
	
	if (((ProjectItem*)item).type == ProjectRoot)
	{
		[[NSNotificationCenter defaultCenter]
		 postNotificationName:ProjectNameEditNotification
		 object:self
		 userInfo:
		 [[NSDictionary alloc] initWithObjectsAndKeys:
		  object,
		  @"ProjectName", 
		  nil]
		 ];
	}
}

/*
 select a file based on a given item.
 */
-(void)selectFileForItem:(ProjectItem *)item
{
	int result;
    NSArray *fileTypes = [NSArray arrayWithObjects:@"csv", @"raw", @"mat", @"txt", @"data", @"cxdmat", nil];
    NSOpenPanel *oPanel = [NSOpenPanel openPanel];
	
    [oPanel setAllowsMultipleSelection:NO];
    [oPanel setTitle:@"Choose Data File"];
    [oPanel setMessage:@"Choose a data file."];
	NSString *home = NSHomeDirectory();
    result = [oPanel runModalForDirectory:home file:nil types:fileTypes];
    if (result == NSOKButton) {
        NSString* file = [[[oPanel URLs] objectAtIndex:0] absoluteString];
		// determine the file type extension.
		item.itemDetails = file; 
		item.hasFile = YES;
		// only the raw data source is expected to open a file.
		if (item.type == RawDataSource)
		{
			[[NSNotificationCenter defaultCenter]
			 postNotificationName:RawDataFilePathNotification
			 object:self
			 userInfo:
				[[NSDictionary alloc] 
					initWithObjectsAndKeys:
						item.itemDetails,
						@"FilePath", 
						nil
				 ]
			 ];
			
		}
	}
	//[self.outlineView reloadData];
}

/*
 select a file based on a given item.
 */
-(void)saveFileForItem:(ProjectItem *)item
{
	int result;
    NSArray *fileTypes = [NSArray arrayWithObjects:@"csv", @"raw", @"mat", @"txt", @"data", nil];
    NSSavePanel *sp;
	sp = [NSSavePanel savePanel];
	[sp setAllowedFileTypes:fileTypes];
	result = [sp runModalForDirectory:NSHomeDirectory() file:@""];
	if (result == NSOKButton) {
		NSString *file = [[sp URL] absoluteString];
		item.itemDetails = file;
		item.hasFile = YES;
		NSString *notification;
		switch(item.type)
		{
			case TrainDataSource:
				notification = TrainDataFilePathNotification;
				break;
			case TrainTargetDataSource:
				notification = TargetDataFilePathNotification;
				break;
			case NetworkDataFile:
			default:
				notification = NetworkBaseFilePathNotification;
				break;
		}
		[[NSNotificationCenter defaultCenter]
		 postNotificationName:notification
		 object:self
		 userInfo:
			[[NSDictionary alloc] 
				initWithObjectsAndKeys:
					item.itemDetails,
					@"FilePath", 
					nil
			]
		 ];
		
	}
	
	//[self.outlineView reloadData];
	
	
}



@end
