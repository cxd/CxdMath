/*
 *  CentreData.hpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 25/07/10.
 *  Copyleft 2010 none. All rights reserved.
 *
 */
#ifndef CXD_MATRIX_CENTRE_DATA
#define CXD_MATRIX_CENTRE_DATA

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
				 Centre data around the mean.
				 
				 This is a simple operation that will 
				 simply centre the data around the mean.
				 
				 M = X - mean(X)
				 
				 **/
				template <class Number> class CentreData : public MatrixOperator<Number>
				{
					
				public:
					
					CentreData(Mat<Number> source) : MatrixOperator<Number>::MatrixOperator(source)
					{
						
					}
					
					/**
					 Perform the operation.
					 @returns
					 True if successful false otherwise.
					 **/
					virtual bool Operate()
					{
						Mat<Number> X = MatrixOperator<Number>::_source;
						_mean = sum(X, 0) / X.n_rows;
						mat mX = repmat(_mean, X.n_rows, 1);
						Mat<Number> deltaX = X - mX;
						MatrixOperator<Number>::_result = deltaX;
						return true;
					}
					
					/**
					 Retrieve the mean of the original source data.
					 **/
					Mat<Number> getMean() {
						return _mean;	
					}
					
				protected:
					
				private:
					Mat<Number> _mean;
				};
			}
		}
	}
}
#endif
