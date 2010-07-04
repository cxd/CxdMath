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
		maxSamples = 100;
		maxYSample = 100;
	
		textAttributes = [[NSDictionary alloc] initWithObjectsAndKeys:
						  [NSFont boldSystemFontOfSize:12.0], NSFontAttributeName, 
						  nil];
		[textAttributes retain];
	
	}
    return self;
}

-(void)awakeFromNib
{
	maxSamples = 100;
	maxYSample = 100;
	[self initGraph];
}

-(void)dealloc
{
	[self.dataSource autorelease];
	[graph autorelease];
	[xAxis autorelease];
	[yAxis autorelease];
	[host autorelease];
	[textAttributes autorelease];
	[super dealloc];
}

/*
 Initialise the graph.
 */
-(void)initGraph 
{
	if (graph != nil)
	{
		[host removeFromSuperview];
		[host autorelease];

		[graph autorelease];
		[xAxis autorelease];
		[yAxis autorelease];
		[dataSourceLinePlot autorelease];
		[self.dataSource autorelease];
	}
	previousSize = 0;
	self.dataSource = [[ErrorPlotDataSource alloc] initWithMaxSamples:maxSamples];
	
	NSRect rect = NSMakeRect([self frame].origin.x, [self frame].origin.y, 
			   [self frame].size.width - 10, [self frame].size.height - 20);
	
	graph = [[CPXYGraph alloc] initWithFrame: rect];
 	[graph retain];
	
	host = [[CPLayerHostingView alloc] initWithFrame: rect];
	[host retain];
	
	
	[host setAutoresizingMask:
	 NSViewWidthSizable 
	 | NSViewHeightSizable 
	 | NSViewMaxXMargin 
	 | NSViewMinXMargin 
	 | NSViewMaxYMargin 
	 | NSViewMinYMargin];
	
	[self addSubview:host];
	CPTheme *theme = [CPTheme themeNamed:kCPDarkGradientTheme];
    [graph applyTheme:theme];
	
	[graph setAutoresizingMask:
	 NSViewWidthSizable 
	 | NSViewHeightSizable 
	 | NSViewMaxXMargin 
	 | NSViewMinXMargin 
	 | NSViewMaxYMargin 
	 | NSViewMinYMargin];
	
	
	host.hostedLayer = graph;
	graph.paddingTop = 30;
	graph.paddingLeft = 20;
	graph.paddingBottom = 20;
	graph.paddingRight = 20;
	
	
    // Setup scatter plot space
    CPXYPlotSpace *plotSpace = (CPXYPlotSpace *)graph.defaultPlotSpace;
    plotSpace.allowsUserInteraction = NO;
	plotSpace.delegate = self;
	
	// Grid line styles
    CPLineStyle *majorGridLineStyle = [CPLineStyle lineStyle];
    majorGridLineStyle.lineWidth = 1;
    majorGridLineStyle.lineColor = [[CPColor colorWithGenericGray:0.2] colorWithAlphaComponent:0.75];
    
    CPLineStyle *minorGridLineStyle = [CPLineStyle lineStyle];
    minorGridLineStyle.lineWidth = 1;
    minorGridLineStyle.lineColor = [[CPColor whiteColor] colorWithAlphaComponent:0.1];    
   
	
	plotSpace.xRange = [CPPlotRange plotRangeWithLocation:CPDecimalFromFloat(-20)
												   length:CPDecimalFromFloat(maxSamples)];
	plotSpace.yRange = [CPPlotRange plotRangeWithLocation:CPDecimalFromFloat(-20)
												   length:CPDecimalFromFloat(maxYSample)];
	
	
	// Axes
    // Label x axis with a fixed interval policy
	
	CPXYAxisSet *axisSet = (CPXYAxisSet *)graph.axisSet;
    xAxis = axisSet.xAxis;
	[xAxis retain];
    xAxis.title = @"Time";
	xAxis.plotSpace = [graph defaultPlotSpace];
	xAxis.preferredNumberOfMajorTicks = 10;
	xAxis.majorIntervalLength = CPDecimalFromFloat(maxSamples/ 10.f);
	// Label y with an automatic label policy. 
    
	yAxis = axisSet.yAxis;
	[yAxis retain];
    yAxis.plotSpace = [graph defaultPlotSpace];
	yAxis.title = @"Error Signal";
	yAxis.preferredNumberOfMajorTicks = 10;
	yAxis.majorIntervalLength = CPDecimalFromFloat(maxYSample/10.f);
	// Set axes
    graph.axisSet.axes = [NSArray arrayWithObjects: xAxis, yAxis, nil];
	 
	dataSourceLinePlot = [[[CPScatterPlot alloc] init] autorelease];
	dataSourceLinePlot.identifier = @"ErrorSignal";
	dataSourceLinePlot.dataLineStyle.lineWidth = 1.5f;
	dataSourceLinePlot.dataLineStyle.lineColor = [CPColor redColor];
	dataSourceLinePlot.dataSource = self.dataSource;
	[graph addPlot:dataSourceLinePlot];
	
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
	NSNumber *axisY = [NSNumber numberWithDouble:([max doubleValue] + 10.0)];
	
	CPXYPlotSpace *plotSpace = (CPXYPlotSpace *)graph.defaultPlotSpace;
	
	NSNumber *yMax = [NSNumber numberWithInt:maxYSample]; 
	
	
	
	if ([yMax intValue] < [max intValue])
	{
		maxYSample = [max intValue];
		plotSpace.yRange = [CPPlotRange plotRangeWithLocation:CPDecimalFromFloat(-10)
												   length:[axisY decimalValue]];
	}
	// otherwise if the min is below a given threshold.
	else if ( ([max floatValue] < 10.f) &&
		(maxYSample > 1) )
	{
		maxYSample = ([max floatValue] > 1) ? [max floatValue] : 1;
		plotSpace.yRange = [CPPlotRange plotRangeWithLocation:CPDecimalFromFloat(-2)
													   length:CPDecimalFromFloat(10)];
		
	
		CPLineStyle *minorGridLineStyle = [CPLineStyle lineStyle];
		minorGridLineStyle.lineWidth = 1;
		minorGridLineStyle.lineColor = [[CPColor whiteColor] colorWithAlphaComponent:0.1];    
		
		
		CPXYAxisSet *axisSet = (CPXYAxisSet *)graph.axisSet;
		yAxis = axisSet.yAxis;
		[yAxis retain];
		yAxis.plotSpace = [graph defaultPlotSpace];
		yAxis.title = @"Error Signal";
		yAxis.preferredNumberOfMajorTicks = 10;
		yAxis.majorIntervalLength = CPDecimalFromFloat(1.f);
		yAxis.minorTicksPerInterval = 10.f;
		// Set axes
		graph.axisSet.axes = [NSArray arrayWithObjects: xAxis, yAxis, nil];
		
	}
	else if ([max floatValue] < 1.f)
	{
		maxYSample = ([max floatValue] > 1) ? [max floatValue] : 1;
		plotSpace.yRange = [CPPlotRange plotRangeWithLocation:CPDecimalFromFloat(-0.5f)
													   length:CPDecimalFromFloat(1.0f)];
		
		
		CPLineStyle *minorGridLineStyle = [CPLineStyle lineStyle];
		minorGridLineStyle.lineWidth = 1;
		minorGridLineStyle.lineColor = [[CPColor whiteColor] colorWithAlphaComponent:0.1];    
		
		
		CPXYAxisSet *axisSet = (CPXYAxisSet *)graph.axisSet;
		yAxis = axisSet.yAxis;
		[yAxis retain];
		yAxis.plotSpace = [graph defaultPlotSpace];
		yAxis.title = @"Error Signal";
		yAxis.preferredNumberOfMajorTicks = 10;
		yAxis.majorIntervalLength = CPDecimalFromFloat(0.1f);
		// Set axes
		graph.axisSet.axes = [NSArray arrayWithObjects: xAxis, yAxis, nil];
		
	}
	
	
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
	[graph reloadData];
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
