/*
 *  TestCxdMatrixReader.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 10/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */
// CppUnit-Tutorial
// file: fractiontest.h
#ifndef TEST_CXD_MATRIX_READER_H
#define TEST_CXD_MATRIX_READER_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include "CxdMath.h"
#include "armadillo"
#include <sstream>

using namespace std;

class TestCxdMatrixReader : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (TestCxdMatrixReader);
    CPPUNIT_TEST (testRead);
    // add a CPPUNIT_TEST macro for each test method.
	CPPUNIT_TEST_SUITE_END ();
	
public:
	void setUp (void);
	void tearDown (void);
	void testRead (void);
	
protected:
	string _testFilePath;
	
};


#endif

