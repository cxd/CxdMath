/*
 *  CovarianceOperator.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 20/07/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#ifndef CXD_MATRIX_COVARIANCE
#define CXD_MATRIX_COVARIANCE

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
				 Covariance of matrix.
				 
				 Cov(X) = E[(X - ux)(X - ux)]
				 
				 which is :
				 
				 integral ((X - ux)(X - ux)') * px * dx
				  
				 This operation approximates covariance between X, Y by using:
				 
				 Cov(X, Y) = sum ( ( X - mX)(Y - mY)' / N )
				 where mX = mean of X
				 mY = mean of Y
				 N = number of rows in X
				 
				 X and Y must be matrices of same dimension.
				 
				 **/
				template <class Number> class CovarianceOperator : public MatrixOperator<Number>
				{
					
				public:
					
					CovarianceOperator(Mat<Number> source, Mat<Number> otherMat) : MatrixOperator<Number>::MatrixOperator(source)
					{
						_other = otherMat;
					}
					
					/**
					 Perform the operation.
					 @returns
					 True if successful false otherwise.
					 **/
					virtual bool Operate()
					{
						Mat<Number> X = MatrixOperator<Number>::_source;
						Mat<Number> Y = _other;
						
						Mat<Number> mX = sum(X, 0) / X.n_rows;
						
						cout << "Mean X : " << mX << endl;
						
						mX = repmat(mX, X.n_rows, 1);
						Mat<Number> deltaX = X - mX;
						
						Number p = 1.0 / X.n_rows;
						
						Mat<Number> mY = sum(Y, 0) / Y.n_rows;
						mY = repmat(mY, Y.n_rows, 1);
						Mat<Number> deltaY = Y - mY;
						
						MatrixOperator<Number>::_result = (deltaX * htrans(deltaY))*p;
						
						return true;
					}
					
				protected:
					
				private:
					Mat<Number> _other;
				};
			}
		}
	}
}
#endif
