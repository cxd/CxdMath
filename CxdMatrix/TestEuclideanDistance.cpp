/*
 *  TestEuclideanDistance.cpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 18/07/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#include "TestEuclideanDistance.h"

using namespace au::id::Cxd::Operator;
using namespace arma;

// register with the test framework.
CPPUNIT_TEST_SUITE_REGISTRATION (TestEuclideanDistance);

void TestEuclideanDistance::setUp (void) 
{
	
}

void TestEuclideanDistance::tearDown (void)
{
	
}

void TestEuclideanDistance::test (void)
{
	mat test1 = zeros(5, 10);
	mat test2 = ones(5, 10);
	mat test3 = randu(5, 10);
	
	EuclideanDistance<double> dist(test1, test1, ROWS);
	dist.Operate();
	mat result = dist.getResult();
	CPPUNIT_ASSERT_MESSAGE("Result is not zero", result(0,0) == 0);
	EuclideanDistance<double> dist2(test1, test2, ROWS);
	dist2.Operate();
	mat result2 = dist2.getResult();
	
	EuclideanDistance<double> dist3(test1, test2, COLS);
	dist3.Operate();
	mat result3 = dist3.getResult();
	
	CPPUNIT_ASSERT_MESSAGE("Result is zero", result2(0,0) > 0);
	cout << "Distance 01" << endl << result << endl;
	cout << "Distance 02" << endl << result2 << endl;
	cout << "Distance 03" << endl << result3 << endl;

}