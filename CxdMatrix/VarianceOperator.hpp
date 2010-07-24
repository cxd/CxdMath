/*
 *  VarianceOperator.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 19/07/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#ifndef CXD_MATRIX_VARIANCE
#define CXD_MATRIX_VARIANCE

#include "IOperator.hpp"
#include "MatrixOperator.hpp"
#include "MinMaxNormaliseMatrix.hpp"

#include "armadillo"

using namespace arma;

namespace au {
	namespace id {
		namespace Cxd  {
			namespace Operator {
				
				/**
				 Variance of matrix.
				 
				 Var(X) = sum P(x)*(x - u)^2
				 where u = sum x*P(x) = mean x
				 
				 If x is uniformly distributed u = 1/N 
				 where N is the size of the population (number of rows).
				 
				 **/
				template <class Number> class VarianceOperator : public MatrixOperator<Number>
				{
					
				public:
					
					VarianceOperator(Mat<Number> source) : MatrixOperator<Number>::MatrixOperator(source)
					{
					}
					
					/**
					 Perform the operation.
					 @returns
					 True if successful false otherwise.
					 **/
					virtual bool Operate()
					{
						Mat<Number> source = MatrixOperator<Number>::_source;
						Mat<Number> mean = sum(source, 0) / source.n_rows;
						mean = repmat(mean, source.n_rows, 1);
						Mat<Number> delta = source - mean;
						delta = delta % delta;
						Number px = 1.0 / source.n_rows;
						MatrixOperator<Number>::_result = sum(delta*px, 0);
						return true;
					}
					
				protected:
					
				private:
					
				};
			}
		}
	}
}
#endif