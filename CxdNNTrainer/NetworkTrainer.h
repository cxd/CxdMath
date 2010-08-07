//
//  NetworkTrainer.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 22/05/10.
//  Copyright 2010 none. All rights reserved.
//

#ifdef __cplusplus
#include "CxdMath.h"


using namespace au::id::Cxd::Operator;
using namespace arma;

#endif

#import <Cocoa/Cocoa.h>
#import "DataViewDelegate.h"
#import "ProjectNotifications.h"

/**
 
 The network trainer receives input data that has been prepared in the following way.
 
 1. It is normalised.
 2. Then it is centred around the mean.
 
 To feed data from a unknown sample, 
 it is possible to use the min/max vectors from the normalisation process
 and the mean from the data centre process in order to convert the new sample
 input vector into a vector that can be classified by the network once it has been trained.
 
 
 **/

@interface NetworkTrainer : NSObject {
	
#ifdef __cplusplus
	/*
	 network instance to train.
	 */
	McCullochPittsNeuralNetwork<double> *network;
	
	arma::mat _trainData;
	arma::mat _targetData;
	arma::mat _testData;
	arma::mat _testTargetData;
	
	arma::mat _testOutputs;
#endif
	
	DataViewDelegate *trainDataDelegate;
	DataViewDelegate *targetDataDelegate;
	
	BOOL isTrained;
	
	int trainEpochs;
	double errorThreshold;
	int trainPercent;
}

@property(assign) BOOL isTrained;

#ifdef __cplusplus

/**
 Accessor for the test targets.
 **/
-(arma::mat) testTargets;
/**
 Accessor for the test outputs.
 **/
-(arma::mat) testOutputs;

#endif


/**
 Get the number of output columns.
 **/
-(int)getOutputColumns;


/*
 Initialise by loading from file.
 */
-(id)initWithFile:(NSString *)filePath trainData: (DataViewDelegate *)trainView targetData:(DataViewDelegate *)targetView;

/**
 Initialise with the data table delegates.
 **/
-(id)initWithTrainData:(DataViewDelegate *)trainSource targetData:(DataViewDelegate *)targetView learnRate:(double)rate;

/**
 Initialise with the data table delegates.
 **/
-(id)initWithTrainData:(DataViewDelegate *)trainSource targetData:(DataViewDelegate *)targetView learnRate:(double)rate momentum:(double)m bias:(double)b;

/**
 Train the network.
 **/
-(void)beginTrainNetworkForEpochs:(int)epochs errorThreshold:(double)threshold;


/**
 Begin testing the network.
 **/
-(void)beginTestNetwork;

/**
 train the network.
 **/
-(void)trainTask;

/*
 set the training attributes for the network.
 */
-(void)setNetAttributeLearnRate:(double)l momentum:(double)m bias:(double)b;

/*
 Set training and target data.
 */
-(void)setNetAttributeTrainingData:(DataViewDelegate *)train targetData:(DataViewDelegate *)target;

/*
 Save the network to the supplied path.
 */
-(void)saveNetworkToPath:(NSString *)filePath;

/*
 Read a network from file path.
 */
-(void)readNetworkFromPath:(NSString *)filePath;

#ifdef __cplusplus
/**
 An error signal is received from the neural net during training.
 **/
-(void)errorSignal:(arma::mat)errMat;

/*
 Split the training and test data from the source data.
 */
-(void)defineNetworkTrainingData:(arma::mat)sourceData Targets:(arma::mat)targetData;

#endif
@end
