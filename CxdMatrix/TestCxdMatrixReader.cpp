/*
 *  TestCxdMatrixReader.cpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 10/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#include "TestCxdMatrixReader.h"

using namespace au::id::Cxd::IO;
using namespace arma;

// register with the test framework.
CPPUNIT_TEST_SUITE_REGISTRATION (TestCxdMatrixReader);

void TestCxdMatrixReader::setUp (void)
{
_testFilePath = string("/Users/cd/Projects/Math/CxdMatrix/testmatrix01.csv");	
}

void TestCxdMatrixReader::tearDown (void)
{
	
}

void TestCxdMatrixReader::testRead (void)
{
	MatrixReader reader = MatrixReader(_testFilePath);
	CPPUNIT_ASSERT_MESSAGE("Failed to read matrix", reader.Read());
	mat resultMat = reader.getMatrix();
	cout << "Result Mat Rows: " << resultMat.n_rows << " Cols: " << resultMat.n_cols << endl;
	CPPUNIT_ASSERT_MESSAGE("Rows are 0", resultMat.n_rows > 0);
	CPPUNIT_ASSERT_MESSAGE("Cols are 0", resultMat.n_cols > 0);
	stringstream str;
	resultMat.save(cout, raw_ascii);
}
