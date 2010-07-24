/*
 *  TestNeuralNetwork.cpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 17/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#include "TestNeuralNetwork.h"
#include <boost/bind.hpp>

using namespace au::id::Cxd::Operator;
using namespace arma;

// register with the test framework.
CPPUNIT_TEST_SUITE_REGISTRATION (TestNeuralNetwork);



void TestNeuralNetwork::setUp (void)
{
	
}

void TestNeuralNetwork::tearDown (void)
{
	
}


void TestNeuralNetwork::testNetwork (void)
{
	/*
	 State: ORGANISE_INPUT_DATA
	 
	 
	 Each row in the input data is a vector for the input sample.
	 */
	mat X = "1 0.5 5 5 -1 -1 1; 0.99 0.45 4.5 3 -0.7 -0.5 0.8";
	X = repmat(X, 4, 1); // 8 rows
	cout << "Input" << endl;
	cout << X << endl;
	
	NormaliseMatrix<double> normalise(X);
	normalise.Operate();
	mat data = normalise.getResult();
	cout << "Normal Data: " << endl;
	cout << data << endl;
	/*
	 State: ORGANISE_TARGET_DATA
	 */
	// a 2 column target vector.
	// 8 rows therefore 8 targets.
	//mat targets = "-1 1; 0.5 0; -1 1; -1 1; 0.5 0; 0.5 0; 0.5 0; -1 1;";
	
	// try 3 target columns
	mat targets = "1;0;1;0;1;0;1;0;";
	
	// training epochs
	int epochs = 100000;
	
	McCullochPittsNeuralNetwork<double> network(data,
												targets,
												0.0001);
	
	//network.connectToErrorSignal(boost::bind(&TestNeuralNetwork::onErrorSignal, this, _1));
	
	cout << "Training network" << endl;
	int epochCount = network.TrainNetwork(epochs, 0.5);
	cout << "Num Epochs: " << epochCount 
	<< endl 
	<< "Error: " << network.getSumSquaredError()
	<< endl;
	
	mat results(targets.n_rows, 2);
	results.col(0) = targets;
	results.col(1) = htrans(network.getOutput());
	cout << "Targets, Outputs: " << endl;
	cout << results << endl;
	
	// test save.
	network.Write("test_network.net");
	
	// test read
	McCullochPittsNeuralNetwork<double> stored("test_network.net");
	stored.Read();
	
}

void onErrorSignal(mat error)
{

}