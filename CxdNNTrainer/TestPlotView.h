//
//  TestPlotView.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 11/07/10.
//  Copyright 2010 none. All rights reserved.
//
#ifdef __cplusplus
#include "CxdMath.h"


using namespace au::id::Cxd::Operator;
using namespace arma;

#endif

#import <Cocoa/Cocoa.h>
#import <CorePlot/CorePlot.h>
#import "PlotDataSource.h"
#import "BasePlotView.h"
#import "NetworkTrainer.h"
/**
 The test plot view will display
 the networks projections against the targets
 that are provided by data test set.
 
 This will be updated by the test button and can be displayed at different times during training.
 
 **/
@interface TestPlotView : BasePlotView  {

	PlotDataSource *outputDataSource;
	PlotDataSource *targetDataSource;
	CPScatterPlot *netOutputLinePlot;
	BOOL mustUpdate;
	int column;
	NetworkTrainer *trainerRef;
}

/**
 Column associated with output to display.
 **/
@property(assign) int column;


/**
 Display test results.
 **/
-(void)onDisplayResults:(NetworkTrainer *)trainer;

/**
 receive changed selection.
 **/
-(IBAction)onSelectionChange:(id)sender;

@end
