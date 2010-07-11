//
//  MyDocument.m
//  CxdNNTrainer
//
//  Created by Chris Davey on 18/04/10.
//  Copyright 2010 none. All rights reserved.
//

/**
 Note using precompiled headers with CPP
 will cause compilation problems as objective-c and C++ headers
 may have same defines that override each other.
 
 To prevent this set the project not to precompile headers
 and set the header file to blank.
 
 **/

#include "CxdMath.h"
#import "MyDocument.h"


@implementation MyDocument


@synthesize accuracy;
@synthesize errorSignal;

@synthesize projectConfig;
@synthesize rawDataResource;
@synthesize trainDataResource;
@synthesize targetDataResource;
@synthesize networkResource;

@synthesize projectTree;
@synthesize rawDataProperty;
@synthesize netProperty;


@synthesize rawDataView;

// training data view.
@synthesize trainDataView;

// target data view.
@synthesize targetDataView;

@synthesize dataCommandDraw;

@synthesize trainer;

@synthesize trainButton;
@synthesize progressBar;

@synthesize errorPlot;
@synthesize	testPlot;
@synthesize testPlotPanel;

- (id)init
{
    self = [super init];
    if (self) {
    
        // Add your subclass-specific initialization here.
        // If an error occurs here, send a [self release] message and return nil.
		
    }
    return self;
}


-(void)dealloc
{
	if (self.projectConfig != nil)
	{
		[self.projectConfig autorelease];
	}
	if (self.rawDataResource != nil)
	{
		[self.rawDataResource autorelease];	
	}
	if (self.trainDataResource != nil)
	{
		[self.trainDataResource autorelease];	
	}
	if (self.targetDataResource != nil)
	{
		[self.targetDataResource autorelease];	
	}
	if (self.networkResource != nil)
	{
		[self.networkResource autorelease];	
	}
	if (self.projectTree != nil)
	{
		[self.projectTree autorelease];	
	}
	if (self.rawDataView != nil)
	{
		[self.rawDataView autorelease];	
	}
	if (self.dataCommandDraw != nil)
	{
		[self.dataCommandDraw autorelease];	
	}
	if (self.trainDataView != nil)
	{
		[self.trainDataView autorelease];	
	}
	if (self.targetDataView != nil)
	{
		[self.targetDataView autorelease];	
	}
	if (self.trainer != nil)
	{
		[self.trainer autorelease];	
	}
	if (self.trainButton != nil)
	{
		[self.trainButton autorelease];	
	}
	if (self.progressBar != nil)
	{
		[self.progressBar autorelease];	
	}
	if (self.errorPlot != nil)
	{
		[self.errorPlot autorelease];	
	}
	if (self.netProperty != nil)
	{
		[self.netProperty autorelease];	
	}
	if (self.testPlot != nil)
	{
		[self.testPlot autorelease];	
	}
	if (self.testPlotPanel != nil)
	{
		[self.testPlotPanel autorelease];
	}
	[super dealloc];
}

- (NSString *)windowNibName
{
   // Override returning the nib file name of the document
    // If you need to use a subclass of NSWindowController or if your document supports multiple NSWindowControllers, you should remove this method and override -makeWindowControllers instead.
    return @"MyDocument";
}

- (void)windowControllerDidLoadNib:(NSWindowController *) aController
{
    [super windowControllerDidLoadNib:aController];
    // Add any code here that needs to be executed once the windowController has loaded the document's window.

	// store a reference to the project from the object managed context.
	[self initialiseModel];
	[self initialiseViewsFromModel];
	[self subscribeToNotifications];
	
}

-(void)initialiseModel
{
	self.accuracy = 0.0;
	// how many tracks already exist.
	NSEntityDescription *desc = 
	[NSEntityDescription entityForName:@"NetTrainProjectConfig" inManagedObjectContext:[self managedObjectContext]];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:desc];
	NSError *error;
	NSArray *array = [[self managedObjectContext] executeFetchRequest:request error:&error];
	if ( (array != nil) && ([array count] > 0) )
	{
		self.projectConfig = [array objectAtIndex:0];
		
		self.rawDataResource = [self.projectConfig valueForKey:@"rawData"];
		self.trainDataResource = [self.projectConfig valueForKey:@"trainingSet"];
		self.targetDataResource = [self.projectConfig valueForKey:@"trainingTargets"];
		self.networkResource = [self.projectConfig valueForKey:@"networkBaseFile"];
		
	} else {
		self.projectConfig = 
			[NSEntityDescription insertNewObjectForEntityForName:@"NetTrainProjectConfig" 
										  inManagedObjectContext:[self managedObjectContext]];
		[self.projectConfig setValue: @"New Project" forKey: @"name"];
		
		[self.projectConfig setValue:[NSNumber numberWithInt:0] forKey:@"targetColumn"];
		[self.projectConfig setValue:[NSNumber numberWithInt:0] forKey:@"endTargetColumn"];
		[self.projectConfig setValue:[NSNumber numberWithInt:0] forKey:@"isContinuous"];
		[self.projectConfig setValue:@"" forKey:@"classList"];
		
		self.rawDataResource = 
			[NSEntityDescription insertNewObjectForEntityForName:@"NetTrainResource" 
										  inManagedObjectContext:[self managedObjectContext]];
		[self.rawDataResource setValue:@"" forKey:@"location"];
		
		[self.projectConfig setValue: self.rawDataResource forKey: @"rawData"];
		
		self.trainDataResource = 
		[NSEntityDescription insertNewObjectForEntityForName:@"NetTrainResource" 
									  inManagedObjectContext:[self managedObjectContext]];
		
		[self.trainDataResource setValue:@"" forKey:@"location"];
		[self.projectConfig setValue: self.trainDataResource forKey: @"trainingSet"];
		
		
		self.targetDataResource = 
		[NSEntityDescription insertNewObjectForEntityForName:@"NetTrainResource" 
									  inManagedObjectContext:[self managedObjectContext]];
		
		[self.targetDataResource setValue:@"" forKey:@"location"];
		[self.projectConfig setValue: self.targetDataResource forKey: @"trainingTargets"];
		
		
		self.networkResource = 
		[NSEntityDescription insertNewObjectForEntityForName:@"NetTrainResource" 
									  inManagedObjectContext:[self managedObjectContext]];
		
		[self.networkResource setValue:@"" forKey:@"location"];
		[self.projectConfig setValue: self.networkResource forKey: @"networkBaseFile"];
		
		
		[[self managedObjectContext] insertObject:self.projectConfig];
		
	}
}

/**
 Read the values from the core data object into any custom views.
 **/
-(void)initialiseViewsFromModel
{
	
	// update views from data.
	if (self.projectTree != nil)
	{
		[self.projectTree initialiseProjectInfo:self.projectConfig];
	}
	if (self.rawDataProperty != nil)
	{
		[self.rawDataProperty initialiseProjectInfo:self.projectConfig];	
	}
	if (self.netProperty != nil)
	{
		[self.netProperty initialiseProjectInfo:self.projectConfig];	
	}
	
	
	if (self.rawDataView != nil)
	{
		NSString *rawData = [self.rawDataResource valueForKey:@"location"];
		if ( (rawData != nil) && ([rawData length] > 0) )
		{
			[self.rawDataView openTable:rawData];
		}
	}
	
	if (self.trainDataView != nil)
	{
		NSString *trainData = [self.trainDataResource valueForKey:@"location"];
		if ( (trainData != nil) && ([trainData length] > 0) )
		{
			[self.trainDataView openTable:trainData];	
		}
	}
	
	if (self.targetDataView != nil)
	{
		NSString *targetData = [self.targetDataResource valueForKey:@"location"];
		if ( (targetData != nil) && ([targetData length] > 0) )
		{
			[self.targetDataView openTable:targetData];	
		}
	}
	
}

/**
 subscribe to the notifications that will be issued
 by edits occuring in the views.
 **/
-(void)subscribeToNotifications
{
	
	// project name change event.
	[[NSNotificationCenter defaultCenter]
		addObserver:self
	 selector:@selector(onProjectNameUpdated:) 
	 name:ProjectNameEditNotification 
	 object:projectTree];
	
	// file resource events.
	[[NSNotificationCenter defaultCenter]
	 addObserver:self
	 selector:@selector(onFilePathChange:) 
	 name:RawDataFilePathNotification 
	 object:projectTree];
	
	[[NSNotificationCenter defaultCenter]
	 addObserver:self
	 selector:@selector(onFilePathChange:) 
	 name:TargetDataFilePathNotification 
	 object:projectTree];
	
	[[NSNotificationCenter defaultCenter]
	 addObserver:self
	 selector:@selector(onFilePathChange:) 
	 name:TrainDataFilePathNotification 
	 object:projectTree];
	
	[[NSNotificationCenter defaultCenter]
	 addObserver:self
	 selector:@selector(onFilePathChange:) 
	 name:NetworkBaseFilePathNotification 
	 object:projectTree];
	
	// subscribe to targets change values.
	[[NSNotificationCenter defaultCenter]
	 addObserver:self
	 selector:@selector(onTargetColumnChange:)
	 name:TargetColumnEditNotification
	 object:rawDataProperty];
	
	[[NSNotificationCenter defaultCenter]
	 addObserver:self
	 selector:@selector(onTargetColumnChange:)
	 name:EndTargetColumnEditNotification
	 object:rawDataProperty];
	
	[[NSNotificationCenter defaultCenter]
	 addObserver:self
	 selector:@selector(onTargetColumnChange:)
	 name:TargetContinuousEditNotification
	 object:rawDataProperty];
	
		
	[[NSNotificationCenter defaultCenter]
	 addObserver:self
	 selector:@selector(onNetworkPropertyChange:) 
	 name:TrainEpochsEditNotification
	 object:netProperty];
	
	[[NSNotificationCenter defaultCenter]
	 addObserver:self
	 selector:@selector(onNetworkPropertyChange:) 
	 name:TrainLearnRateEditNotification
	 object:netProperty];
	
	[[NSNotificationCenter defaultCenter]
	 addObserver:self
	 selector:@selector(onNetworkPropertyChange:) 
	 name:TrainBiasEditNotification
	 object:netProperty];
	
	[[NSNotificationCenter defaultCenter]
	 addObserver:self
	 selector:@selector(onNetworkPropertyChange:) 
	 name:TrainMomentumEditNotification
	 object:netProperty];
}



/**
 Project name updated.
 **/
-(void)onProjectNameUpdated:(NSNotification *)notification
{
	NSString *projectName = [[notification userInfo] objectForKey:@"ProjectName"];
	[self.projectConfig setValue:projectName forKey:@"name"];
	[[self managedObjectContext] processPendingChanges];
}


/**
 Notifications to receive changes to configuration.
 **/
-(void)onTargetColumnChange:(NSNotification *)notification
{
	NSNumber *targetCol = [[notification userInfo] objectForKey:@"TargetsColumn"];
	if ([[notification name] isEqualTo: TargetColumnEditNotification]) {
		[self.projectConfig setValue:targetCol forKey:@"targetColumn"];
	} else if ([[notification name] isEqualTo:EndTargetColumnEditNotification]) {
		[self.projectConfig setValue:targetCol forKey:@"endTargetColumn"];
	} else {
		[self.projectConfig setValue:targetCol forKey:@"isContinuous"];
	}
	[[self managedObjectContext] processPendingChanges];
	
	[self updateTrainingDataView];
	[self updateTargetDataView];
}

/**
 Notifications to receive changes to configuration.
 **/
-(void)onNetworkPropertyChange:(NSNotification *)notification
{
	NSNumber *targetCol = [[notification userInfo] objectForKey:@"value"];
	
	if ([[notification name] isEqualTo:TrainEpochsEditNotification])
	{
		[self.projectConfig setValue:targetCol forKey:@"epochs"];
	} 
	else if ([[notification name] isEqualTo:TrainLearnRateEditNotification])
	{
		[self.projectConfig setValue:targetCol forKey:@"learnRate"];
	}
	else if ([[notification name] isEqualTo:TrainBiasEditNotification])
	{
		[self.projectConfig setValue:targetCol forKey:@"bias"];
	}
	else if ([[notification name] isEqualTo:TrainMomentumEditNotification])
	{
		[self.projectConfig setValue:targetCol forKey:@"momentum"];
	}
	[[self managedObjectContext] processPendingChanges];
}

-(void)updateTrainingDataView
{
	NSInteger startCol = [[self.projectConfig valueForKey:@"targetColumn"] intValue];
	NSInteger endCol = [[self.projectConfig valueForKey:@"endTargetColumn"] intValue];
	
	if (startCol > endCol)
		return;
	
	int srcRows = [self.rawDataView.delegate numRows];
	int srcCols = [self.rawDataView.delegate numCols];
	
	if (srcRows == 0 || srcCols == 0)
		return;
	
	if ( (startCol == 0) && (endCol == 0) ) {
		
		[self.trainDataView.delegate
			copyDataSliceFrom:self.rawDataView.delegate 
					 startRow:0 
				  startColumn:endCol+1 
					   endRow:srcRows-1 
					   endCol:srcCols - 1
					normalise:YES];
	} else {
		[self.trainDataView.delegate copyDataSliceFrom:self.rawDataView.delegate 
											  startRow:0 
										   startColumn:0 
												endRow:srcRows - 1 
												endCol:startCol - 1
											 normalise:YES];
		
		if (endCol < srcCols - 1)
		{
			[self.trainDataView.delegate appendDataSliceFrom:self.rawDataView.delegate 
													startRow:0 
												 startColumn:endCol+1 
													  endRow:srcRows - 1 
													  endCol:srcCols - 1
												   normalise:YES];
		}
	}
	// save the traindata view.
	NSString *data = [self.trainDataResource valueForKey:@"location"];
	if ( (data != nil) && (![data isEqualTo:@""]) )
	{
		[self.trainDataView	saveTable:data];
	}
	[self.trainDataView redisplay];
}

-(void)updateTargetDataView
{
	NSInteger startCol = [[self.projectConfig valueForKey:@"targetColumn"] intValue];
	NSInteger endCol = [[self.projectConfig valueForKey:@"endTargetColumn"] intValue];
	BOOL isContinuous = [[self.projectConfig valueForKey:@"isContinuous"] boolValue];
	
	if (startCol > endCol)
		return;
	
	int srcRows = [self.rawDataView.delegate numRows];
	
	if (srcRows == 0)
		return;
	
	[self.targetDataView.delegate copyDataSliceFrom:self.rawDataView.delegate 
										  startRow:0 
									   startColumn:startCol 
											endRow:srcRows - 1 
											endCol:endCol
										 normalise:isContinuous];
	
	// save the target data view.
	NSString *data = [self.targetDataResource valueForKey:@"location"];
	if ( (data != nil) && (![data isEqualTo:@""]) )
	{
		[self.targetDataView saveTable:data];
	}
	
	[self.targetDataView redisplay];
}

/**
 Notification received to change the file path.
 **/
-(void)onFilePathChange:(NSNotification *)notification
{
	NSString *path = [self convertToFilePath:
					  [[notification userInfo] objectForKey:@"FilePath"]];
	
	if ([[notification name] isEqualTo: RawDataFilePathNotification])
	{
		
		[self.rawDataResource setValue:path forKey:@"location"];
		if (self.rawDataView != nil)
		{
			[self.rawDataView openTable:path];	
			[self updateTrainingDataView];
			[self updateTargetDataView];
		}
		
	} else if ([[notification name] isEqualTo: TargetDataFilePathNotification]) { 		
		
		[self.targetDataResource setValue:path forKey:@"location"];
		NSString *data = [self.targetDataResource valueForKey:@"location"];
		if ( (data != nil) && (![data isEqualTo:@""]) )
		{
			[self.targetDataView saveTable:data];
		}
		
	} else if ([[notification name] isEqualTo: TrainDataFilePathNotification]) { 		
		
		[self.trainDataResource setValue:path forKey:@"location"];
		NSString *data = [self.trainDataResource valueForKey:@"location"];
		if ( (data != nil) && (![data isEqualTo:@""]) )
		{
			[self.trainDataView	saveTable:data];
		}
		
	} else if ([[notification name] isEqualTo: NetworkBaseFilePathNotification]) { 		
		// only save the network to file if the trainer is initialised.
		if (trainer != nil)
		{
		 [self.networkResource setValue:path forKey:@"location"];
		 [self saveNetwork];
		}
	}
	[[self managedObjectContext] processPendingChanges];
}


/**
 Signal to begin training the network.
 **/
-(IBAction)onBeginTraining:(id)sender
{
	if (self.trainButton != nil)
	{
		[self.trainButton setEnabled:NO];
	}
	if (self.errorPlot != nil)
	{
		// reset the error graph
		[self.errorPlot refreshDataSource];	
	}
	
	NSNumber *learnRate = [self.projectConfig valueForKey:@"learnRate"];
	NSNumber *epochs = [self.projectConfig valueForKey:@"epochs"];
	NSNumber *momentum = [self.projectConfig valueForKey:@"momentum"];
	NSNumber *bias = [self.projectConfig valueForKey:@"bias"];
	
	NSString* networkFile = [self.networkResource valueForKey:@"location"];
	
	self.accuracy = 0.0;
	BOOL init = NO;
	if ( (trainer == nil) && (networkFile != nil) && ([networkFile length] != 0) )
	{
		trainer = [[NetworkTrainer alloc] initWithFile:networkFile
											 trainData:self.trainDataView.delegate
											targetData:self.targetDataView.delegate];
		[trainer setNetAttributeLearnRate:[learnRate doubleValue] 
								 momentum:[momentum doubleValue] 
									 bias:[bias doubleValue]];	
		[trainer retain];
	}
	else if (trainer == nil)
	{
		trainer = [[NetworkTrainer alloc] initWithTrainData: self.trainDataView.delegate
												 targetData:self.targetDataView.delegate
												  learnRate:[learnRate doubleValue]
												   momentum:[momentum doubleValue]
													   bias:[bias doubleValue]];
		[trainer retain];
	} else {
		init = YES;
	[trainer setNetAttributeLearnRate:[learnRate doubleValue] 
							 momentum:[momentum doubleValue] 
								 bias:[bias doubleValue]];	
		
	}
	
	// attach notifications to instance.
	if (!init) {
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(onNetworkTrainFinish:) 
		 name:NetworkTrainFinished 
		 object:trainer];
	
	
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(onNetworkTestFinish:) 
		 name:NetworkTestFinished 
		 object:trainer];
	
	
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(onErrorSignal:) 
		 name:NetworkErrorSignal 
		 object:trainer];
	}
	
	
	if (self.progressBar != nil)
	{
		[self.progressBar startAnimation:self];
	}
	[trainer beginTrainNetworkForEpochs:[epochs intValue] errorThreshold:0.00005];
}

/*
 Handle the network training completed event.
 */
-(void)onNetworkTrainFinish:(NSNotification *)notification
{
	if (self.trainButton != nil)
	{
		[self.trainButton setEnabled:YES];
	}
	if (self.progressBar != nil)
	{
		[self.progressBar stopAnimation:self];
	}
	if (self.errorPlot != nil)
	{
		[self.errorPlot updatePlot];	
	}
	[self saveNetwork];
}

-(IBAction)onBeginNetworkTest:(id)sender
{
	if (trainer == nil)
		return;
	self.accuracy = 0.0;
	[trainer beginTestNetwork];
}

-(void)onNetworkTestFinish:(NSNotification *)notification
{
	self.accuracy = 0.0;
	NSNumber *val = [NSNumber numberWithDouble:[[[notification userInfo] objectForKey:@"accuracy"] doubleValue] * 100.0];
	if (val != nil) {
		self.accuracy = [val intValue];
		NSLog(@"Accuracy %f", self.accuracy);
	}
	if ((self.testPlotPanel != nil) && (self.testPlot != nil))
	{
		[self.testPlot onDisplayResults:self.trainer];
		[self.testPlotPanel setIsVisible:YES];
	}
}

/*
 Handle error signal.
 */
-(void)onErrorSignal:(NSNotification *)notification
{
	NSNumber *error = [[notification userInfo] objectForKey:@"error"];
	if (error != nil)
	{
		self.errorSignal = [error doubleValue];	
	}
	// tell the plot to update.
	if (self.errorPlot != nil)
	{
		[self.errorPlot updateError:error];	
	}
}

/**
 Reset and delete the network.
 **/
-(IBAction)onResetNetwork:(id)sender
{
	if (trainer != nil)
	{
	[trainer autorelease];	
	}
	trainer = nil;
}

/*
 save the network.
 */
-(void)saveNetwork
{
	if (trainer == nil)
	{
		return;	
	}
	NSString *path = [self.networkResource valueForKey:@"location"];
	if (path == nil)
		return;
	[trainer saveNetworkToPath:path];
}

-(NSString *) convertToFilePath:(NSString *) path
{
	NSRange range = [path rangeOfString:@"file://localhost/"];
	NSString *result = [path substringFromIndex:range.length - 1];
	return [result stringByReplacingOccurrencesOfString:@"%20" withString:@" "];
}

@end
