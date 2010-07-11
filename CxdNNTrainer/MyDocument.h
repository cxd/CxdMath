//
//  MyDocument.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 18/04/10.
//  Copyright 2010 none. All rights reserved.
//


#import <Cocoa/Cocoa.h>
#import "CxdNetTrainer.h"
#import "CxdTrainerModel.h"
#import "ProjectTreeView.h"
#import "ProjectNotifications.h"
#import "RawDataPropertyData.h"
#import "DataView.h"
#import "NetworkTrainer.h"
#import "TrainErrorCurve.h"
#import "TestPlotView.h"

@interface MyDocument : NSPersistentDocument
{
	NetTrainProjectConfig *projectConfig;
	NetTrainResource *rawDataResource;
	NetTrainResource *trainDataResource;
	NetTrainResource *targetDataResource;
	NetTrainResource *networkResource;
	
	
	ProjectTreeView *projectTree;
	RawDataPropertyData *rawDataProperty;
	NetworkProperties *netProperty;
	
	// raw data display
	DataView *rawDataView;
	
	// training data view.
	DataView *trainDataView;
	
	// target data view.
	DataView *targetDataView;
	
	// a drawer used to show further options about the data grids.
	NSDrawer *dataCommandDraw;
	
	NetworkTrainer *trainer;
	
	NSButton *trainButton;
	
	NSProgressIndicator *progressBar;
	
	TrainErrorCurve *errorPlot;
	
	TestPlotView *testPlot;
	
	NSPanel *testPlotPanel;
	// accuracy
	double accuracy;
	
	// error signal
	double errorSignal;
	
}

@property(assign) double accuracy;
@property(assign) double errorSignal;

@property(retain) NetTrainProjectConfig *projectConfig;
@property(retain) NetTrainResource *rawDataResource;
@property(retain) NetTrainResource *trainDataResource;
@property(retain) NetTrainResource *targetDataResource;
@property(retain) NetTrainResource *networkResource;

@property(retain) IBOutlet DataView *rawDataView;
@property(retain) IBOutlet DataView *trainDataView;
@property(retain) IBOutlet DataView *targetDataView;


@property(retain) IBOutlet ProjectTreeView *projectTree;
@property(retain) IBOutlet RawDataPropertyData *rawDataProperty;
@property(retain) IBOutlet NetworkProperties *netProperty;

@property(retain) IBOutlet NSDrawer *dataCommandDraw;

@property(retain) NetworkTrainer *trainer;

@property(retain) IBOutlet NSButton *trainButton;
@property(retain) IBOutlet NSProgressIndicator *progressBar;

@property(retain) IBOutlet TrainErrorCurve *errorPlot;
@property(retain) IBOutlet TestPlotView *testPlot;
@property(retain) IBOutlet NSPanel *testPlotPanel;


/**
 Initialise the model.
 **/
-(void)initialiseModel;

/**
 Read the values from the core data object into any custom views.
 **/
-(void)initialiseViewsFromModel;

/**
 Attach to notifications that are issued by the views.
 **/
-(void)subscribeToNotifications;

/**
 update the training data view.
 **/
-(void)updateTrainingDataView;

/**
 Update the view associated with targets.
 **/
-(void)updateTargetDataView;
/**
 Notifications to receive changes to configuration.
 **/
-(void)onTargetColumnChange:(NSNotification *)notification;

/*
 Handle the network training completed event.
 */
-(void)onNetworkTrainFinish:(NSNotification *)notification;

/*
 Handle the callback after testing.
 */
-(void)onNetworkTestFinish:(NSNotification *)notification;

/**
 Signal to begin training the network.
 **/
-(IBAction)onBeginTraining:(id)sender;


/**
 Signal to begin testing the network.
 **/
-(IBAction)onBeginNetworkTest:(id)sender;

/**
 Reset and delete the network.
 **/
-(IBAction)onResetNetwork:(id)sender;

/*
 Handle error signal.
 */
-(void)onErrorSignal:(NSNotification *)notification;

/**
 Utility method to convert files to path.
 **/
-(NSString *) convertToFilePath:(NSString *) path;

/*
 save the network.
 */
-(void)saveNetwork;

@end
