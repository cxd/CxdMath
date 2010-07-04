/*
 *  TestNetwork2.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 19/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */



#ifndef TEST_CXD_NN2_H
#define TEST_CXD_NN2_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include "CxdMath.h"
#include "armadillo"
#include <sstream>

using namespace std;

class TestNetwork2 :  public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (TestNetwork2);
    CPPUNIT_TEST (testNetwork);
    // add a CPPUNIT_TEST macro for each test method.
	CPPUNIT_TEST_SUITE_END ();
	
public:
	void setUp (void);
	void tearDown (void);
	void testNetwork (void);

private:
	std::string _filePath;
	
};
#endif