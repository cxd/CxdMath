/*
 *  TestNormaliseMatrix.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 11/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

// CppUnit-Tutorial
// file: fractiontest.h
#ifndef TEST_CXD_NORMALISE_MATRIX_H
#define TEST_CXD_NORMALISE_MATRIX_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include "CxdMath.h"
#include "armadillo"
#include <sstream>

using namespace std;

class TestNormaliseMatrix :  public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (TestNormaliseMatrix);
    CPPUNIT_TEST (testNormalise);
    // add a CPPUNIT_TEST macro for each test method.
	CPPUNIT_TEST_SUITE_END ();
	
public:
	void setUp (void);
	void tearDown (void);
	void testNormalise (void);
	
};
#endif
