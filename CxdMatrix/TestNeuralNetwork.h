/*
 *  TestNeuralNetwork.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 17/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */


#ifndef TEST_CXD_NN_H
#define TEST_CXD_NN_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include "CxdMath.h"
#include "armadillo"
#include <sstream>
#include <boost/bind.hpp>

using namespace std;

class TestNeuralNetwork :  public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (TestNeuralNetwork);
    CPPUNIT_TEST (testNetwork);
    // add a CPPUNIT_TEST macro for each test method.
	CPPUNIT_TEST_SUITE_END ();
	
public:
	void setUp (void);
	void tearDown (void);
	void testNetwork (void);
	void onErrorSignal(mat error);
	
};
#endif