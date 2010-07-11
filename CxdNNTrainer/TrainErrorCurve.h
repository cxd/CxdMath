//
//  TrainErrorCurve.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 25/05/10.
//  Copyright 2010 none. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <CorePlot/CorePlot.h>
#import "PlotDataSource.h"
#import "BasePlotView.h"

@interface TrainErrorCurve : BasePlotView {
	PlotDataSource *dataSource;
	NSUInteger previousSize;
}

@property(retain) PlotDataSource *dataSource;

-(void)initGraph;

-(void)refreshDataSource;

-(void)updatePlot;

-(void)updateError:(NSNumber *)err;
@end
