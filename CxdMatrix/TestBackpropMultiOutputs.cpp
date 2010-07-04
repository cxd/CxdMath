/*
 *  TestBackpropMultiOutputs.cpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 17/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#include "TestBackpropMultiOutputs.h"


using namespace au::id::Cxd::Operator;
using namespace arma;

// register with the test framework.
CPPUNIT_TEST_SUITE_REGISTRATION (TestBackpropMultiOutputs);



void TestBackpropMultiOutputs::setUp (void)
{
	
}

void TestBackpropMultiOutputs::tearDown (void)
{
	
}


void TestBackpropMultiOutputs::test (void)
{
	// inputs column vector.
	// X is a input Column Vector.
	// inputs column vector.
	
	/*
	 State: ORGANISE_INPUT_DATA
	 
	 
	 Each row in the input data is a vector for the input sample.
	 */
	mat X = "1 0.5 5 5 -1 -1 -1";
	X = repmat(X, 8, 1); // 7 rows
	cout << "Input" << endl;
	cout << X << endl;
	/*
	 State: ORGANISE_TARGET_DATA
	 */
	// a 2 column target vector.
	// 8 rows therefore 8 targets.
	//mat targets = "-1 1; 0.5 0; -1 1; -1 1; 0.5 0; 0.5 0; 0.5 0; -1 1;";
	
	// try 3 target columns
	mat targets = "10 -1 1;5 0.5 0;10 -1 1;10 -1 1;5 0.5 0;10 0.5 0;5 0.5 0;10 -1 1;";
	
	
	int outputCount = targets.n_cols;
	
	
	/*
	 State: NORMALISE_INPUT_DATA
	 */
	
	
	/*
	 State: SEPARATE_LEARN_TEST_DATA
	 */
	
	
	
	/*
	 State: INITIALISE_WEIGHTS
	 */
	/*
	 output weights same dimension as input vector.
	 
	 output rows = number of cols in Input matrix.
	 output columns = number of columns in targets.
	 
	 
	 */
	mat w = rand(X.n_cols, outputCount)/X.n_rows;
	mat outputDelta = zeros(w.n_rows, w.n_cols);
	
	int outputWeightRows = w.n_rows;
	int outputWeightCols = w.n_cols;
	
	
	// hidden weights is matrix of input vector Rows * Rows 
	/**
	 Hidden Weights Rows = Number of Cols in Input Vector 
	 Hidden Weights Cols = Number of Cols in Input Vector
	 **/
	// generated from a column vector.
	mat tempW = rand(X.n_cols, 1)/X.n_rows;
	mat W = repmat(tempW, 1, X.n_cols);
	mat hiddenDelta = zeros(W.n_rows, W.n_cols);
	
	int hiddenWeightRows = W.n_rows;
	int hiddenWeightCols = W.n_cols;
	
	
	cout << "Input Size: [" << X.n_rows << " , " << X.n_cols << "]" << endl;
	/*
	 Weights dimensions equals (input rows x input rows) 
	 */
	cout << "Output Weights Size: [" << w.n_rows << " , " << w.n_cols << "]" << endl;
	cout << "Hidden Weights Size: [" << W.n_rows << " , " << W.n_cols << "]" << endl;
	
	
	double temp = 0.8;
	double amp = 0.1;
	double bias = 0.001;
	double learnRate = 0.000001;
	double momentum = 0.0001;
	
	
	/* 
	 hidden neuron
	 State:
	 ACTIVATE_HIDDEN_LAYER
	 */
	McCullochPittsNeuron<double> neuron(htrans(X), htrans(W), temp, amp, bias);
	neuron.Operate();
	mat hO = neuron.getResult();
	mat dHO = neuron.getDerivative();
	
	// output neuron
	/*
	 State: ACTIVATE_OUTPUT_LAYER
	 */
	McCullochPittsNeuron<double> output(hO, w, temp, amp, bias);
	output.Operate();
	mat O = output.getResult();
	mat dO = output.getDerivative();
	
	cout << "Output size ( " << O.n_rows << " , " << O.n_cols << " ) " << endl;
	
	CPPUNIT_ASSERT_MESSAGE("network with 2 output neurons, Output colums does not equal input columns", 
						   outputCount == O.n_rows && X.n_rows == O.n_cols);
	
	
	/*
	 State: BACKPROP_OUTPUT_LAYER
	 */
	BackPropogration<double> outputBackprop(O, hO, w, outputDelta, htrans(targets), dO, learnRate, momentum);
	outputBackprop.Operate();
	
	// assign the updated weights to the output layer.
	w = outputBackprop.getWeights();
	
	CPPUNIT_ASSERT_MESSAGE("Output Backprop weights size does not equal original weights size", 
						   w.n_rows == outputWeightRows && w.n_cols == outputWeightCols);
	
	
	
	// transverse output weights.
	mat nextLayerWeightTargets = htrans(w);
	mat nextLayerGradient = outputBackprop.getGradient(); 
	mat previousOutputs = X;
	
	
	/*
	 State: BACKPROP_HIDDEN_LAYER
	 */
	BackPropogration<double> hiddenBackprop(X,
											W,
											hiddenDelta,
											nextLayerWeightTargets,
											dHO,
											learnRate,
											momentum,
											nextLayerGradient);
	hiddenBackprop.Operate();
	// assign the updated weights
	W = hiddenBackprop.getWeights();
	
	CPPUNIT_ASSERT_MESSAGE("Hidden Backprop weights size does not equal original weights size", 
						   W.n_rows == hiddenWeightRows && W.n_cols == hiddenWeightCols);
	
	
	cout << "One epoch." << endl;
	cout << "Hidden Weights " << endl;
	cout << W << endl;
	cout << endl;
	cout << "Output Weights " << endl;
	cout << w << endl;
	cout << endl;
	
	// compute the sum squared error.
	SquaredError<double> squaredError(htrans(O), targets);
	squaredError.Operate();
	cout << "Sum of Squared Error." << endl;
	cout << squaredError.getResult() << endl;
	
	// compute the mean squared error.
	SquaredError<double> meanSquaredError(htrans(O), targets);
	meanSquaredError.Operate();
	cout << "Mean Squared Error." << endl;
	cout << meanSquaredError.getResult() << endl;
	
}