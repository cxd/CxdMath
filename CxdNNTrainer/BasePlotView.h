//
//  BasePlotView.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 11/07/10.
//  Copyright 2010 none. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <CorePlot/CorePlot.h>



/**
 BasePlotView 
 the base of plot views inside this project.
 
 currently not really any behavioural traits.
 can be used to initialise the default hosting layer.
 and provides shared properties.
 **/
@interface BasePlotView : NSView<CPPlotSpaceDelegate> {
	CPXYGraph *graph;
	CPLayerHostingView *host; 
	
	CPXYAxis *xAxis;
	CPXYAxis *yAxis;
	
	CPScatterPlot *dataSourceLinePlot;
	
	NSUInteger maxSamples;
	NSUInteger maxYSample;
	
	NSDictionary *textAttributes;
	
	NSString *xAxisTitle;
	NSString *yAxisTitle;
}

@property(retain) NSString *xAxisTitle;
@property(retain) NSString *yAxisTitle;

@property(retain) CPXYGraph *graph;
@property(retain) CPLayerHostingView *host;
@property(retain) CPXYAxis *xAxis;
@property(retain) CPXYAxis *yAxis;
@property(retain) CPScatterPlot *dataSourceLinePlot;
@property(assign) NSUInteger maxSamples;
@property(assign) NSUInteger maxYSample;
@property(retain) NSDictionary *textAttributes;


/**
 Initialise the main hosting layer.
 **/
-(void)initGraph;

/*
 Update the plot space based on the current data source.
 
 tutorial:
 http://blogs.remobjects.com/blogs/mh/2010/01/26/p973
 
 */
-(void)updatePlot:(NSNumber *)max;

@end
