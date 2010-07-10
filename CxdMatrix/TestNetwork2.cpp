/*
 *  TestNetwork2.cpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 19/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#include "TestNetwork2.h"



using namespace au::id::Cxd::Operator;
using namespace au::id::Cxd::IO;
using namespace arma;

// register with the test framework.
CPPUNIT_TEST_SUITE_REGISTRATION (TestNetwork2);



void TestNetwork2::setUp (void)
{
	_filePath = std::string("/Users/cd/Projects/Math/CxdMatrix/pima-indians-diabetes.csv");	
}

void TestNetwork2::tearDown (void)
{
	
}


void TestNetwork2::testNetwork (void)
{
	MatrixReader reader(_filePath);
	CPPUNIT_ASSERT_MESSAGE("Failed to read matrix", reader.Read());
	mat resultMat = reader.getMatrix();
	// divide the network into training and test data.
	int trainCnt = (int) (resultMat.n_rows * 0.8);
	int testCnt = resultMat.n_rows - trainCnt;
	mat trainSet = resultMat.submat(0, 0, trainCnt - 1, resultMat.n_cols - 2);
	mat trainTargets = resultMat.submat(0, resultMat.n_cols - 1, trainCnt - 1, resultMat.n_cols - 1);
	
	NormaliseMatrix<double> normalise(trainSet);
	normalise.Operate();
	trainSet = normalise.getResult();
	
	cout << "Train Set" << endl;
	cout << trainSet << endl;
	
	
	// training epochs
	int epochs = 1000;
	
	McCullochPittsNeuralNetwork<double> network(trainSet,
												trainTargets,
												0.01,
												0,
												1,
												1,
												0);
	
	
	cout << "Training test pima network" << endl;
	
	int trainedEpochs = network.TrainNetwork(epochs, 0.5);
	cout << "Num Epochs: " <<  trainedEpochs << endl; 
	
	cout << "Error: " 
	<< network.getSumSquaredError() 
	<< endl;
	
	mat results(trainTargets.n_rows, 2);
	results.col(0) = trainTargets;
	results.col(1) = htrans(network.getOutput());
	cout << "Targets, Outputs: " << endl;
	cout << results << endl;
	
	
	// test save.
	network.Write("test_pima_data_network.net");
}
