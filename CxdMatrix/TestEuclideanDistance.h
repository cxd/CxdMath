/*
 *  TestEuclideanDistance.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 18/07/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#ifndef TEST_CXD_EU_DISTANCE
#define TEST_CXD_EU_DISTANCE

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include "CxdMath.h"
#include "armadillo"
#include <sstream>

using namespace std;

class TestEuclideanDistance :  public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (TestEuclideanDistance);
    CPPUNIT_TEST (test);
    // add a CPPUNIT_TEST macro for each test method.
	CPPUNIT_TEST_SUITE_END ();
	
public:
	void setUp (void);
	void tearDown (void);
	void test (void);
	
};
#endif