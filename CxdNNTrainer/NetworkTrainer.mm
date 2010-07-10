//
//  NetworkTrainer.m
//  CxdNNTrainer
//
//  Created by Chris Davey on 22/05/10.
//  Copyright 2010 none. All rights reserved.
//


#import "NetworkTrainer.h"

using namespace au::id::Cxd::Operator;
using namespace arma;


class TrainerCallback {
public:
	
	TrainerCallback(NetworkTrainer *t, McCullochPittsNeuralNetwork<double> *net)
	{
		_trainer = t;
		[_trainer retain];
		_network = net;
		// bind to the signal 
		_connection = _network->
						connectToErrorSignal(
											boost::bind(&TrainerCallback::OnErrorSignal, this, _1));
	}
	
	~TrainerCallback() {
		_connection.disconnect();
		// remove weak reference.
		_network = NULL;
		[_trainer autorelease];
	}
	
	/**
	 Receive the callback from the error signal.
	 **/
	void OnErrorSignal(arma::mat &error) const {
		[_trainer errorSignal:error];
	}
	
private:
	boost::signals2::connection _connection;
	McCullochPittsNeuralNetwork<double> *_network;
	NetworkTrainer *_trainer;
};

@implementation NetworkTrainer


@synthesize isTrained;

/*
 Initialise by loading from file.
 */
-(id)initWithFile:(NSString *)filePath trainData: (DataViewDelegate *)trainView targetData:(DataViewDelegate *)targetView
{
	self = [super init];
	trainPercent = 75;
	[self readNetworkFromPath:filePath];
	if (network != NULL)
	{
		[self setNetAttributeTrainingData:trainView targetData:targetView];	
	}
	return self;
}

/**
 Initialise with the data table delegates.
 **/
-(id)initWithTrainData:(DataViewDelegate *)trainSource 
			targetData:(DataViewDelegate *)targetView 
			 learnRate:(double)rate
{
	self = [super init];
	trainPercent = 75;
	trainDataDelegate = trainSource;
	[trainDataDelegate retain];
	targetDataDelegate = targetView;
	[targetDataDelegate retain];
	[self defineNetworkTrainingData:[trainSource getMatrix] Targets:[targetView getMatrix]];	
	
	network = new au::id::Cxd::Operator::McCullochPittsNeuralNetwork<double>(_trainData,
																   _targetData,
																   rate);
	return self;
}

/**
 Initialise with the data table delegates.
 **/
-(id)initWithTrainData:(DataViewDelegate *)trainSource targetData:(DataViewDelegate *)targetView learnRate:(double)rate momentum:(double)m bias:(double)b
{
	self = [super init];
	trainPercent = 75;
	trainDataDelegate = trainSource;
	[trainDataDelegate retain];
	targetDataDelegate = targetView;
	[targetDataDelegate retain];
	[self defineNetworkTrainingData:[trainSource getMatrix] Targets:[targetView getMatrix]];	
	network = new au::id::Cxd::Operator::McCullochPittsNeuralNetwork<double>(_trainData,
																			 _targetData,
																			 rate,
																			 m,
																			 1,
																			 1,
																			 b);
	return self;
}


-(void)dealloc
{
	[trainDataDelegate autorelease];
	[targetDataDelegate autorelease];
	delete network;
	network = NULL;
	
	[super dealloc];	
}

/**
 Train the network.
 **/
-(void)beginTrainNetworkForEpochs:(int)epochs errorThreshold:(double)threshold
{
	trainEpochs = epochs;
	errorThreshold = threshold;
	isTrained = NO;
	NSOperationQueue* aQueue = [[NSOperationQueue alloc] init];
	NSInvocationOperation *op = [[NSInvocationOperation alloc] initWithTarget:self
										  selector:@selector(trainTask) object:nil];
	if (op != nil)
		[aQueue addOperation:op];
}

-(void)trainTask
{
	// attach to the callback using the proxy class "TrainerCallback".
	TrainerCallback cb(self, network);
	network->TrainNetwork(trainEpochs, errorThreshold);
	isTrained = YES;	
	[[NSNotificationCenter defaultCenter]
				postNotificationName:NetworkTrainFinished
							  object:self
							userInfo:nil];
}


-(void)beginTestNetwork
{
	NSOperationQueue* aQueue = [[NSOperationQueue alloc] init];
	NSInvocationOperation *op = [[NSInvocationOperation alloc] initWithTarget:self
																	 selector:@selector(testTask) 
																	   object:nil];
	if (op != nil)
		[aQueue addOperation:op];
}

-(void)testTask
{
	
	McCullochPittsNeuralNetwork<double> testNet(_testData,
												_testTargetData,
												network->getLearnRate(),
												network->getMomentum(),
												network->getTemperature(),
												network->getAmplitude(),
												network->getBias());
	testNet.InitialiseNetwork(network->getOutputWeights(), network->getHiddenWeights());
	testNet.setAllowBackPropogation(false);
	NSLog(@"test network %i %i", _testData.n_rows, _testData.n_cols);
	testNet.Operate();
	
	// determine the error.
	arma::mat output = htrans(testNet.getOutput());
	// sum the errors.
	double totalMatched = 0.0; 
	for(int i=0;i<output.n_rows;i++)
	{
		for(int j=0;j<output.n_cols;j++)
		{
			NSLog(@"Output: %f Expected: %f", output(i,j), _testTargetData(i, j));
			if (output(i,j) == _testTargetData(i,j))
			{
				totalMatched += 1.0;	
			}
		}
	}
	totalMatched = totalMatched / (1.0*output.n_rows);
	// average the error.
	
	//NSLog(@"Accuracy: %f", totalMatched);
	
	NSNumber *notifyValue = [NSNumber numberWithDouble:totalMatched];
	
	[[NSNotificationCenter defaultCenter]
	 postNotificationName:NetworkTestFinished
	 object:self
	 userInfo:
	 [[NSDictionary alloc] initWithObjectsAndKeys:
	  notifyValue,
	  @"accuracy", 
	  nil]
	 ];
	
}

-(void)setNetAttributeLearnRate:(double)l momentum:(double)m bias:(double)b
{
	network->setLearnRate(l);
	network->setBias(b);
	network->setMomentum(m);
}

-(void)setNetAttributeTrainingData:(DataViewDelegate *)train targetData:(DataViewDelegate *)target 
{
	if (trainDataDelegate != nil)
	{
		[trainDataDelegate autorelease];
		trainDataDelegate = train;
		[trainDataDelegate retain];
	}
	if (targetDataDelegate != nil)
	{
		[targetDataDelegate autorelease];
		targetDataDelegate = target;
		[targetDataDelegate retain];
	}
	[self defineNetworkTrainingData:[train getMatrix] Targets:[target getMatrix]];
	network->setSource(_trainData);
	network->setTargets(_targetData);
}

-(void)errorSignal:(arma::mat)errMat
{
	
	// currently only select the first signal.
	if (errMat.n_rows == 0)
		return;
	
	//std::cout << "Error Signal " << std::endl << errMat << std::endl;
	
	double n = errMat(0,0);
	
	NSNumber *error = [NSNumber numberWithDouble:n];
	
	[[NSNotificationCenter defaultCenter]
	 postNotificationName:NetworkErrorSignal
	 object:self
	 userInfo:
	 [[NSDictionary alloc] initWithObjectsAndKeys:
	  error,
	  @"error", 
	  nil]];
}

/*
 Save the network to the supplied path.
 */
-(void)saveNetworkToPath:(NSString *)filePath
{
	if ([filePath length] == 0)
		return;
	if (network == NULL) return;
	network->Write(std::string([filePath UTF8String])); 
}

-(void)readNetworkFromPath:(NSString *)filePath
{
	bool isRead = false;
	if ([filePath length] != 0)
	{
		if (network != NULL)
		{
			delete network;
			network = NULL;
		}
		// read data from file.
		network = new McCullochPittsNeuralNetwork<double>(std::string([filePath UTF8String]));
		isRead = network->Read();
	} 
	if (!isRead)
	{
	// otherwise we need to recreate the network.
	network = new au::id::Cxd::Operator::McCullochPittsNeuralNetwork<double>(NULL,
																			 NULL,
																			 0.001);
	}
}


/*
 Split the training and test data from the source data.
 */
-(void)defineNetworkTrainingData:(arma::mat)sourceData Targets:(arma::mat)targetData
{
	int percent = sourceData.n_rows * (trainPercent*1.f)/(sourceData.n_rows*1.f);
	int rows = sourceData.n_rows - percent;	
	_trainData = sourceData.submat(0,0,rows,sourceData.n_cols-1);
	_targetData = targetData.submat(0,0,rows,targetData.n_cols-1);
	_testData = sourceData.submat(rows+1, 0, sourceData.n_rows-1,sourceData.n_cols-1);
	_testTargetData = targetData.submat(rows+1, 0, targetData.n_rows-1,targetData.n_cols-1);
}


@end
