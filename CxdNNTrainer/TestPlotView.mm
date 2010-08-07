//
//  TestPlotView.m
//  CxdNNTrainer
//
//  Created by Chris Davey on 11/07/10.
//  Copyright 2010 none. All rights reserved.
//

#import "TestPlotView.h"


@implementation TestPlotView

@synthesize column;


- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
		[self initGraph];
		self.maxSamples = 100;
		self.maxYSample = 100;
		self.textAttributes = [[NSDictionary alloc] initWithObjectsAndKeys:
							   [NSFont boldSystemFontOfSize:12.0], NSFontAttributeName, 
							   nil];
		self.column = 0;
	}
    return self;
}

-(void)awakeFromNib
{
	self.maxSamples = 100;
	self.maxYSample = 100;
	self.column = 0;
	[self initGraph];
}

-(void)dealloc
{
	if (outputDataSource != nil)
	{
		[outputDataSource autorelease];
	}
	if (targetDataSource != nil)
	{
		[targetDataSource autorelease]; 	
	}
	// the network output line plot.
	if (netOutputLinePlot != nil)
	{
		[netOutputLinePlot autorelease];	
	}
	// trainer ref is a weak reference.
	trainerRef = NULL;
	
	[super dealloc];
}

/*
 Initialise the graph.
 */
-(void)initGraph 
{
	if (outputDataSource != nil)
	{
		[outputDataSource autorelease];
	}
	if (targetDataSource != nil)
	{
		[targetDataSource autorelease]; 	
	}
	self.xAxisTitle = @"Sample";
	self.yAxisTitle = @"Output Signal";
	
	outputDataSource = [[PlotDataSource alloc] initWithMaxSamples:maxSamples];
	targetDataSource = [[PlotDataSource alloc] initWithMaxSamples:maxSamples];
	[super initGraph];
	
	// the default axis is read, this will display the target data source.
	self.dataSourceLinePlot.dataSource = targetDataSource;
	
	// the network output line plot.
	if (netOutputLinePlot != nil)
	{
		[netOutputLinePlot autorelease];	
	}
	// TODO: set up the additional axis for the network output plot.
	netOutputLinePlot = [[[CPScatterPlot alloc] init] autorelease];
	netOutputLinePlot.identifier = @"Output Plot";
	netOutputLinePlot.dataLineStyle.lineWidth = 1.5f;
	netOutputLinePlot.dataLineStyle.lineColor = [CPColor greenColor];
	netOutputLinePlot.dataSource = outputDataSource;
	[self.graph addPlot:netOutputLinePlot];
}


/**
 Display test results.
 **/
-(void)onDisplayResults:(NetworkTrainer *)trainer
{
	arma::mat testTargets = [trainer testTargets];
	arma::mat testOutputs = [trainer testOutputs];
	self.maxSamples = testTargets.n_rows;
	trainerRef = trainer;
	for(int i=0;i<testTargets.n_rows;i++)
	{
		[targetDataSource accumulateY:[NSNumber numberWithDouble:testTargets(i, self.column)]];
	}
	for(int i=0;i<testOutputs.n_rows;i++)
	{
		[outputDataSource accumulateY:[NSNumber numberWithDouble:testOutputs(i, self.column)]];
	}
	mustUpdate = YES;
	
	if ([targetDataSource.ySamples count] == 0)
		return;
	
	NSNumber *maxA = [targetDataSource maxYValue];
	NSNumber *maxB = [outputDataSource maxYValue];
	
	if (maxA > maxB) {
		[self updatePlot:maxA];
		// update twice to force view adjustment.
		[self updatePlot:maxA];
	} else {
		[self updatePlot:maxB];
		// update twice to force view adjustment.
		[self updatePlot:maxB];
	}

	[self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect {
    if (mustUpdate)
	{
		mustUpdate = NO;
		[self.graph reloadData];
	}
}

/**
 receive changed selection.
 **/
-(IBAction)onSelectionChange:(id)sender
{
	if ([sender indexOfSelectedItem] >= 0)
	{
		self.column = [sender indexOfSelectedItem];
		if (trainerRef != NULL)
		{
			[self onDisplayResults:trainerRef];
		}
		[self.graph reloadData];
		[self setNeedsDisplay:YES];	
	}
}

@end
