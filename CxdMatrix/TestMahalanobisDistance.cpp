/*
 *  TestMahalanobisDistance.cpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 18/07/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#include "TestMahalanobisDistance.h"


using namespace au::id::Cxd::Operator;
using namespace arma;

// register with the test framework.
CPPUNIT_TEST_SUITE_REGISTRATION (TestMahalanobisDistance);

void TestMahalanobisDistance::setUp (void) 
{
	
}

void TestMahalanobisDistance::tearDown (void)
{
	
}

void TestMahalanobisDistance::test (void)
{
	mat test1 = zeros(5, 10);
	mat test2 = ones(5, 10);
	mat test3 = randu(5, 10);
	
	MahalanobisDistance<double> dist(test1, test1);
	dist.Operate();
	mat result = dist.getResult();
	MahalanobisDistance<double> dist2(test1, test2);
	dist2.Operate();
	mat result2 = dist2.getResult();
	MahalanobisDistance<double> dist3(test3, repmat(test3.row(1), test3.n_rows, 1));
	dist3.Operate();
	mat result3 = dist3.getResult();
	
	VarianceOperator<double> op(test3);
	op.Operate();
	mat vartest = op.getResult();
	
	cout << "M Variance" << endl << vartest << endl;
	
	CovarianceOperator<double> covar(test2, test3);
	covar.Operate();
	mat cotest = covar.getResult();
	cout << "Covariance: " << endl << cotest << endl;
	
	CovarianceOperator<double> covar2(test3, test3);
	covar2.Operate();
	mat cotest2 = covar2.getResult();
	cout << "Covariance2: " << endl << cotest2 << endl;
	
	
	cout << "M Distance 01" << endl << result << endl;
	cout << "M Distance 02" << endl << result2 << endl;
	cout << "M Distance 03" << endl << result3 << endl;
	
	//CPPUNIT_ASSERT_MESSAGE("Result is not zero", result(0,0) == 0);
	//CPPUNIT_ASSERT_MESSAGE("Result is zero", result2(0,0) > 0);
}