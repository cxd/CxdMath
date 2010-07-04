/*
 *  SquaredError.hpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 17/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 * Calculate the squared error of a input matrix against a target matrix.
 */


#ifndef CXD_IF_MATRIX_OP_SQUARED_ERROR
#define CXD_IF_MATRIX_OP_SQUARED_ERROR

#include "IOperator.hpp"
#include "armadillo"

using namespace arma;

namespace au {
	namespace id {
		namespace Cxd {
			namespace Operator {
				template <class Number>
				class SquaredError : public MatrixOperator<Number>
				{
				public:
					
					/**
					 Construct with source matrix.
					 **/
					SquaredError(Mat<Number> source, Mat<Number> targets) 
					: MatrixOperator<Number>::MatrixOperator(source)
					{
						_targets = targets;
					}
					
					/**
					 Calculate the sum of squared errors for the target and source.
					 
					 The source and targets must be the same dimensions.
					 
					 Matlab example:
					 LE(n) = 0.5*sum((LC - Lo).^2);
					 **/
					bool Operate()
					{
						Mat<Number> source = MatrixOperator<Number>::_source;
						_error = (_targets - source);
						// error.^2
						_squaredError = _error % _error;
						// sum(squaredError)
						_sumOfSquaredError = 0.5*sum(_squaredError);
						MatrixOperator<Number>::_result = _sumOfSquaredError;
						return true;
					}
				
					Mat<Number> getError()
					{
						return _error;	
					}
					
					Mat<Number> getSquaredError()
					{
						return _squaredError;
					}
					
					Mat<Number> getSumOfSquaredError()
					{
						return _sumOfSquaredError;
					}
					
				protected:
					
					Mat<Number> _targets;
					Mat<Number> _squaredError;
					Mat<Number> _sumOfSquaredError;
					Mat<Number> _error;
				};
			}
		}
	}
	
}
#endif