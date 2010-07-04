/*
 *  NormaliseMatrix.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 11/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 
 This class will perform min max normalisation of the matrix input.
 
 
 */
#ifndef CXD_MATRIX_NORMALISE
#define CXD_MATRIX_NORMALISE

#include "IOperator.hpp"
#include "MatrixOperator.hpp"

#include "armadillo"

using namespace arma;

namespace au {
	namespace id {
		namespace Cxd  {
			namespace Operator {

				template <class Number> class NormaliseMatrix : public MatrixOperator<Number>
				{
					
					
				public:
					
					NormaliseMatrix(Mat<Number> source) : MatrixOperator<Number>::MatrixOperator(source)
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
						 end
						 **/
						
						for(int k=0;k<source.n_cols;k++)
						{
							Col<Number> V = source.col(k);
							
							
							//if (min(V) != max(V))
							//{
							//	result.col(k) = (V - min(V)) / (max(V) - min(V));	
							//} else {
								_divisor = ones(V.n_rows) * sqrt(sum(V%V));
								result.col(k) = V/_divisor;	
							//}
							
						}
						MatrixOperator<Number>::_result = result;
						
						return true;
					}
					
					Col<Number> getDivisor() {
						return _divisor;	
					}
					
				private:
					/* 
					 the divisor used to normalise the matrix.
					 the number of columns in the divisor is equal to the number of columns
					 in the matrix.
					 This can be used to normalise data for the same number of attributes using
					 the same divisor as was originally used by the source matrix.
					*/
					Col<Number> _divisor;
					
				};
				
			}
		}
	}
}


#endif
