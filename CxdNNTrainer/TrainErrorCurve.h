//
//  TrainErrorCurve.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 25/05/10.
//  Copyright 2010 none. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <CorePlot/CorePlot.h>
#import "ErrorPlotDataSource.h"

@interface TrainErrorCurve : NSView<CPPlotSpaceDelegate> {
	CPXYGraph *graph;
	CPLayerHostingView *host; 
	
	CPXYAxis *xAxis;
	CPXYAxis *yAxis;
	
	ErrorPlotDataSource *dataSource;
	
	NSUInteger previousSize;
	
	CPScatterPlot *dataSourceLinePlot;
	
	NSUInteger maxSamples;
	NSUInteger maxYSample;
	
	NSDictionary *textAttributes;
	
}

@property(retain) ErrorPlotDataSource *dataSource;

-(void)initGraph;

-(void)refreshDataSource;

-(void)updatePlot;

-(void)updateError:(NSNumber *)err;
@end
