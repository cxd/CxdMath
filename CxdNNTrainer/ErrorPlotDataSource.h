//
//  ErrorPlotDataSource.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 27/05/10.
//  Copyright 2010 none. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <CorePlot/CorePlot.h>

@interface ErrorPlotDataSource : NSObject<CPPlotDataSource> {
	NSMutableArray *xSamples;
	NSMutableArray *ySamples;
	BOOL _shouldRecalculate;
	BOOL _shouldRecalculateMin;
	NSNumber *minY;
	NSNumber *maxY;
	NSUInteger maxRange;
	NSUInteger offset;
}

@property(assign) NSMutableArray *xSamples;
@property(assign) NSMutableArray *ySamples;

-(id)initWithMaxSamples:(NSUInteger)max;

-(void)accumulateY:(NSNumber *)yVal;

-(void)accumulateX:(NSNumber *)xVal Y:(NSNumber*)yVal;

-(void)resetData;

-(void)shiftSamples;

/**	@brief The number of data points for the plot.
 *	@param plot The plot.
 *	@return The number of data points for the plot.
 **/
-(NSUInteger)numberOfRecordsForPlot:(CPPlot *)plot; 

/**	@brief Gets a plot data value for the given plot and field.
 *	@param plot The plot.
 *	@param fieldEnum The field index.
 *	@param index The data index of interest.
 *	@return A data point.
 **/
-(NSNumber *)numberForPlot:(CPPlot *)plot field:(NSUInteger)fieldEnum recordIndex:(NSUInteger)index;

/**
 Get the max y value.
 **/
-(NSNumber *)maxYValue;

/**
 Get the min Y value.
 **/
-(NSNumber *)minYValue;

-(NSUInteger)numberOfRecords ;

@end
