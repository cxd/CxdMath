/*
 *  MahalanobisDistance.hpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 18/07/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#ifndef CXD_MATRIX_MAHALANOBIS_DISTANCE
#define CXD_MATRIX_MAHALANOBIS_DISTANCE

#include "IOperator.hpp"
#include "MatrixOperator.hpp"
#include "EuclideanDistance.hpp"
#include "VarianceOperator.hpp"
#include "CovarianceOperator.hpp"
#include "armadillo"

using namespace arma;

namespace au {
	namespace id {
		namespace Cxd  {
			namespace Operator {
				
				
				
				/**
				 Mahahalanobis distance of matrix, either in dimension of rows or columns.
				 
				 If dimension Rows is used, a row vector is returned with distance between each row.
				 
				 If dimension Cols is used, a col vector is returned with distance between each column.
				 
				 
				 **/
				template <class Number> class MahalanobisDistance : public MatrixOperator<Number>
				{
					
				public:
					
					MahalanobisDistance(Mat<Number> source, Mat<Number> from) : MatrixOperator<Number>::MatrixOperator(source)
					{
						_matrixFrom = from;
					}
					
					/**
					 Perform the operation.
					 
					 
					 Simple calculation of mahalanobis distance using:
					 
					 dist(X,Y) = sqrt(Delta(mX,mY)'*Inv(Cov(X,Y))*Delta(mX,mY))
					 
					 where mX = mean X
					 mY = mean Y
					 
					 This will result in a matrix of distances between each element in X,Y.
					 
					 Example taken from:
					 http://people.revoledu.com/kardi/tutorial/Similarity/MahalanobisDistance.html
					 
					 @returns
					 True if successful false otherwise.
					 **/
					virtual bool Operate()
					{
						CovarianceOperator<Number> covOp(_matrixFrom, MatrixOperator<Number>::_source);
						covOp.Operate();
						
						// compute the pseudo inverse.
						Mat<Number> CI = pinv(covOp.getResult());
						
						
						Mat<Number> mX = repmat( sum(_matrixFrom) / _matrixFrom.n_rows, _matrixFrom.n_rows, 1);
						Mat<Number> mY = repmat( sum(MatrixOperator<Number>::_source) / MatrixOperator<Number>::_source.n_rows,
												MatrixOperator<Number>::_source.n_rows, 1);
						Mat<Number> delta = mX - mY;
						MatrixOperator<Number>::_result = sqrt(htrans(delta)*CI*delta);
						return true;
					}
					
				protected:
					
				private:
					// the matrix to get distance from
					Mat<Number> _matrixFrom;
				};
			}
		}
	}
}

#endif