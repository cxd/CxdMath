//
//  TrainErrorCurve.m
//  CxdNNTrainer
//
//  Created by Chris Davey on 25/05/10.
//  Copyright 2010 none. All rights reserved.
//

#import "TrainErrorCurve.h"


@implementation TrainErrorCurve

@synthesize dataSource;

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
		[self initGraph];
		self.maxSamples = 100;
		self.maxYSample = 100;
	
		self.textAttributes = [[NSDictionary alloc] initWithObjectsAndKeys:
						  [NSFont boldSystemFontOfSize:12.0], NSFontAttributeName, 
						  nil];
		
	}
    return self;
}

-(void)awakeFromNib
{
	self.maxSamples = 100;
	self.maxYSample = 100;
	[self initGraph];
}

-(void)dealloc
{
	if (self.dataSource != nil)
	{
	[self.dataSource autorelease];
	}
	[super dealloc];
}

/*
 Initialise the graph.
 */
-(void)initGraph 
{
	if (self.dataSource != nil)
	{
		[self.dataSource autorelease];
	}
	self.xAxisTitle = @"Time";
	self.yAxisTitle = @"Error Signal";
	previousSize = 0;
	self.dataSource = [[PlotDataSource alloc] initWithMaxSamples:maxSamples];
	[super initGraph];
	self.dataSourceLinePlot.dataSource = self.dataSource;
}

-(void)refreshDataSource
{
	[self.dataSource resetData]; 	
}


/*
 Update the plot space based on the current data source.
 
 tutorial:
 http://blogs.remobjects.com/blogs/mh/2010/01/26/p973
 
 */
-(void)updatePlot
{
	if ([self.dataSource.ySamples count] == 0)
		return;
	
	NSNumber *max = [self.dataSource maxYValue];
	[self updatePlot:max];
		
	/*
	[plotSpace 
	 scaleToFitPlots:
	 [NSArray arrayWithObjects:dataSourceLinePlot, nil]];
	*/
}

#pragma mark Plot Space Delegate Methods
/*
-(CPPlotRange *)plotSpace:(CPPlotSpace *)space willChangePlotRangeTo:(CPPlotRange *)newRange forCoordinate:(CPCoordinate)coordinate {
    // Impose a limit on how far user can scroll in x
    if ( coordinate == CPCoordinateX ) {
        CPPlotRange *maxRange = [CPPlotRange 
								 plotRangeWithLocation:
								 CPDecimalFromFloat(-1.0f) 
								 length:CPDecimalFromFloat(6.0f)];
        CPPlotRange *changedRange = [[newRange copy] autorelease];
        [changedRange shiftEndToFitInRange:maxRange];
        [changedRange shiftLocationToFitInRange:maxRange];
        newRange = changedRange;
    }
    return newRange;
}
*/

#pragma mark -

-(void)updateError:(NSNumber *)err
{
	[self.dataSource accumulateY:err];
	[self setNeedsDisplay:YES];
}


- (void)drawRect:(NSRect)dirtyRect {
    
	// update the graph.
	[self.graph reloadData];
	[self updatePlot];
	/*
	if ( (previousSize != [self.dataSource numberOfRecords]) &&
		(([self.dataSource numberOfRecords] % 10) == 0) )
	{
		previousSize = [self.dataSource numberOfRecords];
		[self updatePlot];
	}
	 */
}

@end
