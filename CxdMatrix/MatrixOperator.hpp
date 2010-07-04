/*
 *  NormaliseMatrix.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 11/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#ifndef CXD_IF_MATRIX_OP
#define CXD_IF_MATRIX_OP

#include "IOperator.hpp"
#include "armadillo"

using namespace arma;

namespace au {
	namespace id {
		namespace Cxd {
			namespace Operator {
				template <class Number> class MatrixOperator
				{
				public:
					/**
					 Construct without source matrix.
					 **/
					MatrixOperator()
					{
					}
					
					/**
					 Construct with source matrix.
					 **/
					MatrixOperator(Mat<Number> source)
					{
						_source = source;
					}
					
					Mat<Number> getSource() {
						return _source;	
					}
					
					Mat<Number> setSource(Mat<Number> src) {
						_source = src;	
					}
					
					Mat<Number> getResult() {
						return _result;	
					}
					
				protected:
					Mat<Number> _result;
					Mat<Number> _source;
				};
			}
		}
	}

}
#endif