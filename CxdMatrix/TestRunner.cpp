/*
 *  TestRunner.cpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 10/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#include "TestRunner.h"

int main (int argc, char* argv[])
{
    // informs test-listener about testresults
    CPPUNIT_NS :: TestResult testresult;
	
    // register listener for collecting the test-results
    CPPUNIT_NS :: TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);
	
    // register listener for per-test progress output
    CPPUNIT_NS :: BriefTestProgressListener progress;
    testresult.addListener (&progress);
	
    // insert test-suite at test-runner by registry
    CPPUNIT_NS :: TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS :: TestFactoryRegistry :: getRegistry ().makeTest ());
    testrunner.run (testresult);
	
    // output results in compiler-format
    CPPUNIT_NS :: CompilerOutputter compileroutputter (&collectedresults, std::cerr);
    compileroutputter.write ();
	
    // return 0 if tests were successful
    return collectedresults.wasSuccessful () ? 0 : 1;
}