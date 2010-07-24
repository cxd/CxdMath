/*
 *  TestNearestNeighbour.cpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 24/07/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#include "TestNearestNeighbour.h"


using namespace au::id::Cxd::Operator;
using namespace arma;

// register with the test framework.
CPPUNIT_TEST_SUITE_REGISTRATION (TestNearestNeighbour);

void TestNearestNeighbour::setUp (void) 
{
	
}

void TestNearestNeighbour::tearDown (void)
{
	
}

void TestNearestNeighbour::test (void)
{
	mat test1 = zeros(5, 10);
	mat test2 = ones(5, 10);
	mat test3 = randu(5, 10);
	
	
	mat train1 = mat(10,10);
	train1.rows(0,4) = test1;
	train1.rows(5,9) = test2;
	train1 = shuffle(train1, 0);
	
	mat targets1 = train1.col(9);
	
	mat x1 = zeros(1, 10);
	mat x2 = ones(1, 10);
	
	KNearestNeighbour<double> knn1(3, EUCLIDEAN, x1.row(0), train1, targets1);
	knn1.Operate();
	mat result1 = knn1.getResult();
	
	KNearestNeighbour<double> knn2(3, EUCLIDEAN, x2.row(0), train1, targets1);
	knn2.Operate();
	mat result2 = knn2.getResult();
	
	cout << "KNN Result1: " << result1 << endl;
	
	cout << "KNN Result2: " << result2 << endl;
	CPPUNIT_ASSERT_MESSAGE("Result is not zero", result1(0,0) == 0);
	CPPUNIT_ASSERT_MESSAGE("Result is not 1", result2(0,0) > 0);
}