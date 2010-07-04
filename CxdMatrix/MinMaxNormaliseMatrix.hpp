/*
 *  MinMaxNormaliseMatrix.hpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 13/06/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#ifndef CXD_MATRIX_MIN_MAX_NORMALISE
#define CXD_MATRIX_MIN_MAX_NORMALISE

#include "IOperator.hpp"
#include "MatrixOperator.hpp"
#include "NormaliseMatrix.hpp"
#include "armadillo"

using namespace arma;

namespace au {
	namespace id {
		namespace Cxd  {
			namespace Operator {
				
				template <class Number> class MinMaxNormaliseMatrix : public NormaliseMatrix<Number>
				{
				public:
					
					MinMaxNormaliseMatrix(Mat<Number> source) : NormaliseMatrix<Number>::NormaliseMatrix(source)
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
						Mat<Number> result = Mat<Number>(source.n_rows, source.n_cols);
						/** matlab example.
						 for n=1:size(T,2)
						 % normalise the supplied column columns.
						 T(:,n) = ( T(:,n) - min(T(:,n)) )/(max(T(:,n)) - min(T(:,n)));
						 
						 to normalise within a range use:
						 
						 T(:,n) = ( T(:,n) - min(T(:,n)) )/(max(T(:,n)) - min(T(:,n))) * (B - A) + A;
						 
						 where A is the min value to clamp to 
						 and B is the max value to clamp to.
						 
						 fitting the range [A,B]
						 
						 end
						 **/
						Row<Number> maxVector = zeros(1,source.n_cols);
						Row<Number> minVector = zeros(1,source.n_cols);
						for(int k=0;k<source.n_cols;k++)
						{
							Col<Number> V = source.col(k);
							double minV = min(V);
							double maxV = max(V);
							maxVector(k) = maxV;
							minVector(k) = minV;
							// add a very small value to the min and max values to prevent division by zero.
							if ( (maxV - minV) != 0.0) {
								result.col(k) = (V - minV) / (maxV - minV);
							} else {
								result.col(k) = ones(V.n_rows) * 0.0;
							}
						}
						MatrixOperator<Number>::_result = result;
						_maxVect = maxVector;
						_minVect = minVector;
						return true;
					}
					
					Row<Number> getMaxVector() {
						return _maxVect;	
					}
					
					Row<Number> getMinVector() {
						return _minVect;	
					}
					
				private:
					
					Row<Number> _maxVect;
					Row<Number> _minVect;

				};

			}
		}
	}
}

#endif