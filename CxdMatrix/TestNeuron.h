/*
 *  TestNeuron.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 12/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#ifndef TEST_CXD_NEURON_H
#define TEST_CXD_NEURON_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include "CxdMath.h"
#include "armadillo"
#include <sstream>

using namespace std;

class TestNeuron :  public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (TestNeuron);
    CPPUNIT_TEST (testActivate);
    // add a CPPUNIT_TEST macro for each test method.
	CPPUNIT_TEST_SUITE_END ();
	
public:
	void setUp (void);
	void tearDown (void);
	void testActivate (void);
	
};
#endif