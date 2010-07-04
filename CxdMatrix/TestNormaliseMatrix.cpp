/*
 *  TestNormaliseMatrix.cpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 11/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#include "TestNormaliseMatrix.h"

using namespace au::id::Cxd::Operator;
using namespace arma;

// register with the test framework.
CPPUNIT_TEST_SUITE_REGISTRATION (TestNormaliseMatrix);

void TestNormaliseMatrix::setUp (void) 
{
	
}

void TestNormaliseMatrix::tearDown (void)
{
	
}

void TestNormaliseMatrix::testNormalise (void)
{
	mat test = rand<mat>(5, 10);
	NormaliseMatrix<double> normalise(test);
	CPPUNIT_ASSERT_MESSAGE("Failed to normalise", normalise.Operate());
	cout << endl;
	normalise.getResult().save(cout, raw_ascii);
	cout << endl;
	double total = sum(normalise.getResult().col(0));
	cout << "Total of Column 1 = " << total << endl;
	
	MinMaxNormaliseMatrix<double> norm2(test);
	CPPUNIT_ASSERT_MESSAGE("Failed to min max normalise", norm2.Operate());
	cout << endl;
	norm2.getResult().save(cout, raw_ascii);
	cout << endl;
	total = sum(norm2.getResult().col(0));
	cout << "Total of Column 1 = " << total << endl;
	
	
}