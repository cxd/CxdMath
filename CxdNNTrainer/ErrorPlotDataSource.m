//
//  ErrorPlotDataSource.m
//  CxdNNTrainer
//
//  Created by Chris Davey on 27/05/10.
//  Copyright 2010 none. All rights reserved.
//

#import "ErrorPlotDataSource.h"


@implementation ErrorPlotDataSource


@synthesize xSamples;
@synthesize ySamples;

-(id)init {
	self = [super init];
	self.xSamples = [[NSMutableArray alloc] init];
	self.ySamples = [[NSMutableArray alloc] init];
	maxY = [NSNumber numberWithInt:INT_MIN];
	[maxY retain];
	minY = [NSNumber numberWithInt:INT_MAX];
	[minY retain];
	_shouldRecalculate = NO;
	_shouldRecalculateMin = NO;
	// maximum samples to display.
	maxRange = 500;
	offset = 0;
	return self;
}

-(id)initWithMaxSamples:(NSUInteger)max
{
	self = [self init];
	maxRange = max;
	self.xSamples = [[NSMutableArray alloc] initWithCapacity:max];
	
	return self;
}


-(void)dealloc {
	
	[self.xSamples removeAllObjects];
	[self.ySamples removeAllObjects];
	[self.xSamples autorelease];
	[self.ySamples autorelease];
	[minY autorelease];
	[maxY autorelease];
	[super dealloc];
}

-(void)resetData
{
	[self.ySamples autorelease];
	self.ySamples = [[NSMutableArray alloc] init];
}


-(void)accumulateY:(NSNumber *)yVal
{
	if ([self.ySamples count] == maxRange)
	{
		[self shiftSamples];	
	}
	[self.ySamples addObject:yVal];
	if ([self.xSamples count] < maxRange)
		[self.xSamples addObject:[NSNumber numberWithInt:[self.ySamples count]]];
	_shouldRecalculate = YES;
	_shouldRecalculateMin = YES;
}

-(void)accumulateX:(NSNumber *)xVal Y:(NSNumber*)yVal
{
	if ([self.xSamples count] == maxRange)
	{
		[self shiftSamples];	
	}
	
	[self.xSamples addObject:xVal];
	[self.ySamples addObject:yVal];
	_shouldRecalculate = YES;
	_shouldRecalculateMin = YES;
}

-(void)shiftSamples
{
	//[self.xSamples removeObjectAtIndex:0];
	[self.ySamples removeObjectAtIndex:0];
}


-(NSUInteger)numberOfRecords 
{
	return [self.xSamples count];	
}

/**	@brief The number of data points for the plot.
 *	@param plot The plot.
 *	@return The number of data points for the plot.
 **/
-(NSUInteger)numberOfRecordsForPlot:(CPPlot *)plot
{
	return [self.xSamples count];
}

/**	@brief Gets a plot data value for the given plot and field.
 *	@param plot The plot.
 *	@param fieldEnum The field index.
 *	@param index The data index of interest.
 *	@return A data point.
 **/

-(NSNumber *)numberForPlot:(CPPlot *)plot field:(NSUInteger)fieldEnum recordIndex:(NSUInteger)index
{
	if (fieldEnum == CPScatterPlotFieldX)
	{
		return [self.xSamples objectAtIndex:index];
	} else {
		return [self.ySamples objectAtIndex:index];
	}
}





/**
 Get the max y value.
 **/
-(NSNumber *)maxYValue
{
	if (_shouldRecalculate)
	{
		maxY = [NSNumber numberWithInt:INT_MIN];
		for(NSNumber *num in self.ySamples)
		{
			if (num > maxY)
			{
			maxY = num;	
			}
		}
		_shouldRecalculate = NO;
	}
	return maxY;
}

/**
 Get the min Y value.
 **/
-(NSNumber *)minYValue
{
	if (_shouldRecalculateMin)
	{
		minY = [NSNumber numberWithInt:INT_MAX];
		for(NSNumber *num in self.ySamples)
		{
			if (num < minY)
			{
			minY = num;	
			}
		}
		_shouldRecalculateMin = NO;
	}
	return minY;
}




@end
