/*
 *  TestNeuron.cpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 12/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#include "TestNeuron.h"

using namespace au::id::Cxd::Operator;
using namespace arma;

// register with the test framework.
CPPUNIT_TEST_SUITE_REGISTRATION (TestNeuron);

void TestNeuron::setUp (void)
{
	
}

void TestNeuron::tearDown (void)
{
	
}


void TestNeuron::testActivate (void)
{
	// X is a input Column Vector.
	// inputs column vector.
	mat X = "1; 0.5; 5; 5; -1; -1; -1;";
	X = repmat(X, 1, 8);
	
	// output weights same dimension as input vector.
	mat w = rand(X.n_rows, 1)/X.n_rows;
	// hidden weights is matrix of input vector Rows * Rows 
	// generated from a column vector.
	mat W = repmat(w, 1, X.n_rows);
	
	double temp = 0.8;
	double amp = 0.1;
	double bias = 0.001;
	
	
	cout << "Input Size: [" << X.n_rows << " , " << X.n_cols << "]" << endl;
	
	/*
	 Weights dimensions equals (input rows x input rows) 
	 */
	cout << "Weights Size: [" << W.n_rows << " , " << W.n_cols << "]" << endl;
	
	cout << endl;
	cout << "Inputs: " << endl;
	cout << X << endl;
	
	
	// hidden neuron
	McCullochPittsNeuron<double> neuron(X, htrans(W), temp, amp, bias);
	
	
	CPPUNIT_ASSERT_MESSAGE("Failed to activate neuron", neuron.Operate());
	
	mat O = neuron.getResult();
	
	cout << endl;
	cout << "Hidden Activation: " << endl;
	neuron.getResult().save(cout, raw_ascii);
	cout << endl;
	cout << "Hidden Derivative: " << endl;
	neuron.getDerivative().save(cout, raw_ascii);
	cout << endl;
	
	
	// output neuron
	McCullochPittsNeuron<double> output(O, w, temp, amp, bias);
	bool result = output.Operate();
	CPPUNIT_ASSERT_MESSAGE("Failed to activate output neuron", result);
	
	mat o = output.getResult();
	CPPUNIT_ASSERT_MESSAGE("network with 1 output neuron, Output colums does not equal input columns", 
						   1 == o.n_rows && X.n_cols == o.n_cols);
	

	
	cout << endl;
	cout << "Output Activation: " << endl;
	output.getResult().save(cout, raw_ascii);
	cout << endl;
	cout << "Output Derivative: " << endl;
	output.getDerivative().save(cout, raw_ascii);
	cout << endl;
	
	
	
}
