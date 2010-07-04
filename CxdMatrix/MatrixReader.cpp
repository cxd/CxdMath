/*
 *  CsvMatrixReader.cpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 6/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#include "MatrixReader.h"

using namespace std;
using namespace arma;

au::id::Cxd::IO::MatrixReader::
MatrixReader(const std::string& filePath) 
{
	_filePath = filePath;
}

au::id::Cxd::IO::MatrixReader::
~MatrixReader()
{
	
}

/**
 Read the input file and generate the output.
 **/
bool au::id::Cxd::IO::MatrixReader::
Read()
{
	try {
		ifstream fin(_filePath.c_str());
		double val;
		char delim;
		string line = "";
		if (!fin)
			return false; // file did not exist.
#ifdef DEBUG
		cout << endl;
#endif
		// now we compute the size of the matrix.
		int cols = 0;
		int rows = 0;
		vector< vector<double> > table;
		bool isFirstRow = true;
		while(fin >> line)
		{
			vector<double> row;
			istringstream sstream(line);
			do
			{
			if (sstream >> val)
			{
				if (isFirstRow)
					cols++;
				// a double value has been extracted
				row.push_back(val);
#ifdef DEBUG
				cout << val;
#endif
			}
			else if (!sstream)
			{
				sstream.clear();
				sstream >> delim;
#ifdef DEBUG
				cout << delim;
#endif
				if (!sstream) {
#ifdef DEBUG
					cout << std::endl;
#endif					
					break; // eol
				}
			}
			} while(sstream);
			isFirstRow = false;
			rows++;
			table.push_back(row);
		}
		fin.close();
		_matrix.set_size(rows,cols);
		for(int i=0;i<rows;i++)
		{
			vector<double> curRow = table.at(i);
			for(int j=0;j<cols;j++)
			{
				double curVal = curRow.at(j);
				_matrix(i,j) = curVal;
			}
		}
		return true;
	}
	catch (std::exception e) {
		cerr << "Exception raised: " << e.what() << endl;
	}
	return false;
}

mat au::id::Cxd::IO::MatrixReader::getMatrix()
{
	return _matrix;	
}