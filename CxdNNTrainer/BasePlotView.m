//
//  BasePlotView.m
//  CxdNNTrainer
//
//  Created by Chris Davey on 11/07/10.
//  Copyright 2010 none. All rights reserved.
//

#import "BasePlotView.h"


@implementation BasePlotView



@synthesize graph;
@synthesize host;
@synthesize xAxis;
@synthesize yAxis;
@synthesize dataSourceLinePlot;
@synthesize maxSamples;
@synthesize maxYSample;
@synthesize textAttributes;
@synthesize xAxisTitle;
@synthesize yAxisTitle;


- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
	self.xAxisTitle = @"Time";
	self.yAxisTitle = @"Signal";
    return self;
}

-(void)dealloc {

	if (self.graph != nil)
	{
		[self.graph autorelease];	
	}
	if (self.host != nil)
	{
		[self.host autorelease];	
	}
	if (self.xAxis != nil)
	{
		[self.xAxis autorelease];	
	}
	if (self.yAxis != nil)
	{
		[self.yAxis autorelease];	
	}
	if (self.dataSourceLinePlot != nil)
	{
		[self.dataSourceLinePlot autorelease];	
	}
	if (self.textAttributes != nil)
	{
		[self.textAttributes autorelease];	
	}
	[self.xAxisTitle autorelease];
	[self.yAxisTitle autorelease];
	[super dealloc];
}


/*
 Initialise the graph.
 */
-(void)initGraph 
{
	if (self.graph != nil)
	{
		[self.host removeFromSuperview];
		[self.host autorelease];
		
		[self.graph autorelease];
		[self.xAxis autorelease];
		[self.yAxis autorelease];
		[self.dataSourceLinePlot autorelease];
	}
	
	NSRect rect = NSMakeRect([self frame].origin.x, [self frame].origin.y, 
							 [self frame].size.width - 10, [self frame].size.height - 20);
	
	self.graph = [[CPXYGraph alloc] initWithFrame: rect];
 	
	self.host = [[CPLayerHostingView alloc] initWithFrame: rect];
	
	
	[self.host setAutoresizingMask:
	 NSViewWidthSizable 
	 | NSViewHeightSizable 
	 | NSViewMaxXMargin 
	 | NSViewMinXMargin 
	 | NSViewMaxYMargin 
	 | NSViewMinYMargin];
	
	[self addSubview:self.host];
	CPTheme *theme = [CPTheme themeNamed:kCPDarkGradientTheme];
    [self.graph applyTheme:theme];
	
	[self.graph setAutoresizingMask:
	 NSViewWidthSizable 
	 | NSViewHeightSizable 
	 | NSViewMaxXMargin 
	 | NSViewMinXMargin 
	 | NSViewMaxYMargin 
	 | NSViewMinYMargin];
	
	
	self.host.hostedLayer = self.graph;
	self.graph.paddingTop = 30;
	self.graph.paddingLeft = 20;
	self.graph.paddingBottom = 20;
	self.graph.paddingRight = 20;
	
	
    // Setup scatter plot space
    CPXYPlotSpace *plotSpace = (CPXYPlotSpace *)self.graph.defaultPlotSpace;
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
												   length:CPDecimalFromFloat(self.maxSamples)];
	plotSpace.yRange = [CPPlotRange plotRangeWithLocation:CPDecimalFromFloat(-20)
												   length:CPDecimalFromFloat(self.maxYSample)];
	
	
	// Axes
    // Label x axis with a fixed interval policy
	
	CPXYAxisSet *axisSet = (CPXYAxisSet *)graph.axisSet;
    self.xAxis = axisSet.xAxis;
	self.xAxis.title = self.xAxisTitle;
	self.xAxis.plotSpace = [self.graph defaultPlotSpace];
	self.xAxis.preferredNumberOfMajorTicks = 10;
	self.xAxis.majorIntervalLength = CPDecimalFromFloat(self.maxSamples/ 10.f);
	// Label y with an automatic label policy. 
    
	self.yAxis = axisSet.yAxis;
	self.yAxis.plotSpace = [graph defaultPlotSpace];
	self.yAxis.title = self.yAxisTitle;
	self.yAxis.preferredNumberOfMajorTicks = 10;
	self.yAxis.majorIntervalLength = CPDecimalFromFloat(self.maxYSample/10.f);
	// Set axes
    self.graph.axisSet.axes = [NSArray arrayWithObjects: self.xAxis, self.yAxis, nil];
	
	self.dataSourceLinePlot = [[[CPScatterPlot alloc] init] autorelease];
	self.dataSourceLinePlot.identifier = @"signal";
	self.dataSourceLinePlot.dataLineStyle.lineWidth = 1.5f;
	self.dataSourceLinePlot.dataLineStyle.lineColor = [CPColor redColor];
	[self.graph addPlot:self.dataSourceLinePlot];
}


/*
 Update the plot space based on the current data source.
 
 tutorial:
 http://blogs.remobjects.com/blogs/mh/2010/01/26/p973
 
 */
-(void)updatePlot:(NSNumber *)max
{
	// enough room for the axis to appear on the screen.
	NSNumber *axisY = [NSNumber numberWithDouble:([max doubleValue] + 25.0)];
	CPXYPlotSpace *plotSpace = (CPXYPlotSpace *)self.graph.defaultPlotSpace;
	NSNumber *yMax = [NSNumber numberWithInt:self.maxYSample]; 
	
	if ([yMax intValue] < [max intValue])
	{
		self.maxYSample = [max intValue];
		plotSpace.yRange = [CPPlotRange plotRangeWithLocation:CPDecimalFromFloat(-10)
													   length:[axisY decimalValue]];
	}
	// otherwise if the min is below a given threshold.
	else if ( ([max floatValue] < 10.f) &&
			 (self.maxYSample > 1) )
	{
		self.maxYSample = ([max floatValue] > 1) ? [max floatValue] : 1;
		plotSpace.yRange = [CPPlotRange plotRangeWithLocation:CPDecimalFromFloat(-2)
													   length:CPDecimalFromFloat(10)];
		
		
		CPLineStyle *minorGridLineStyle = [CPLineStyle lineStyle];
		minorGridLineStyle.lineWidth = 1;
		minorGridLineStyle.lineColor = [[CPColor whiteColor] colorWithAlphaComponent:0.1];    
		
		
		CPXYAxisSet *axisSet = (CPXYAxisSet *)self.graph.axisSet;
		if (self.yAxis != nil)
		{
			[self.yAxis autorelease];	
		}
		self.yAxis = axisSet.yAxis;
		self.yAxis.plotSpace = [self.graph defaultPlotSpace];
		self.yAxis.title = self.yAxisTitle;
		self.yAxis.preferredNumberOfMajorTicks = 10;
		self.yAxis.majorIntervalLength = CPDecimalFromFloat(10.f);
		
		// Set axes
		self.graph.axisSet.axes = [NSArray arrayWithObjects: self.xAxis, self.yAxis, nil];
		
	}
	else if ([max floatValue] <= 1.f)
	{
		self.maxYSample = ([max floatValue] > 1) ? [max floatValue] : 1;
		plotSpace.yRange = [CPPlotRange plotRangeWithLocation:CPDecimalFromFloat(-0.5f)
													   length:CPDecimalFromFloat(2.0f)];
		
		
		CPLineStyle *minorGridLineStyle = [CPLineStyle lineStyle];
		minorGridLineStyle.lineWidth = 1;
		minorGridLineStyle.lineColor = [[CPColor whiteColor] colorWithAlphaComponent:0.1];    
		
		
		CPXYAxisSet *axisSet = (CPXYAxisSet *)self.graph.axisSet;
		if (self.yAxis != nil)
		{
			[self.yAxis autorelease];	
		}
		self.yAxis = axisSet.yAxis;
		[self.yAxis retain];
		self.yAxis.plotSpace = [self.graph defaultPlotSpace];
		self.yAxis.title = self.yAxisTitle;
		self.yAxis.preferredNumberOfMajorTicks = 10;
		self.yAxis.majorIntervalLength = CPDecimalFromFloat(0.1f);
		// Set axes
		self.graph.axisSet.axes = [NSArray arrayWithObjects: self.xAxis, self.yAxis, nil];
		
	}
}


- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}

@end
