/*
 *  CsvMatrixReader.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 6/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#ifndef INCLUDE_CXDMATREADER
#define INCLUDE_CXDMATREADER

#include <iostream>
#include <fstream>
#include <sstream>
#include "armadillo"
#include "IReader.h"


//#define DEBUG 1

namespace au {
	namespace id {
		namespace Cxd {
		namespace IO {
			class MatrixReader : public IReader {
			public:
				
				/**
				 Initialise with a file path to the 
				 source to read.
				 **/
				MatrixReader(const std::string& filePath);
				
				// destructor.
				virtual ~MatrixReader();
				
				/**
				 Read the input file.
				 Return:
				 true if successful false otherwise.
				 **/
				virtual bool Read();
				
				/**
				 Retrieve the matrix after it has been read.
				 **/
				virtual arma::mat getMatrix();
				
			private:
				std::string _filePath;
				// the matrix produced by reading the file
				arma::mat _matrix;  
				
			};
		}
		}
	}
}
#endif