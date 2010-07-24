/*
 *  EuclideanDistance.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 10/07/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#ifndef CXD_MATRIX_EUCLIDEAN_DISTANCE
#define CXD_MATRIX_EUCLIDEAN_DISTANCE

#include "IOperator.hpp"
#include "MatrixOperator.hpp"

#include "armadillo"

using namespace arma;

namespace au {
	namespace id {
		namespace Cxd  {
			namespace Operator {
				
				/**
				 The dimension to use for the distance operation.
				 Either distance between rows or distance between columns.
				 **/
				enum Dimension {
					ROWS = 0,
					COLS = 1
				};
				
				
				/**
				 Euclidean distance of matrix, either in dimension of rows or columns.
				 
				 If dimension Rows is used, a row vector is returned with distance between each row.
				 
				 If dimension Cols is used, a col vector is returned with distance between each column.
				 
				 
				 distance(A, B) = sqrt (sum ((a(ij) - b(ij))^2) )
				 
				 **/
				template <class Number> class EuclideanDistance : public MatrixOperator<Number>
				{
					
				public:
					
					EuclideanDistance(Mat<Number> source, Mat<Number> from, Dimension dim) : MatrixOperator<Number>::MatrixOperator(source)
					{
						_dim = dim;
						_matrixFrom = from;
					}
					
					/**
					 Perform the operation.
					 @returns
					 True if successful false otherwise.
					 **/
					virtual bool Operate()
					{
						switch(_dim)
						{
							case ROWS:
								return DistanceBetweenRows();	
							case COLS:
								return DistanceBetweenColumns();
						}
						return true;
					}
					
				protected:
					
					/**
					 Calculate the distance between the rows in the matrix.
					 This will take the distance of each column accross the rows and produce a row vector.
					 where each column contains the distance between each column.
					 
					 The resulting matrix is 1 row x n cols.
					 **/
					bool DistanceBetweenRows() {
						Mat<Number> delta = MatrixOperator<Number>::_source - _matrixFrom;
						delta = square(delta);
						MatrixOperator<Number>::_result = Mat<Number>(1, delta.n_cols);
						MatrixOperator<Number>::_result.row(0) = sum(delta, 0);
						MatrixOperator<Number>::_result = sqrt(MatrixOperator<Number>::_result);
						return true;
					}
					
					/**
					 Calculate the distance between the columns in the matrix.
					 This will compute the distance between each row of A and B accross the columns and produce a column vector.
					 The resulting matrix is n rows by 1 column.
					 **/
					bool DistanceBetweenColumns() {
						Mat<Number> delta = MatrixOperator<Number>::_source - _matrixFrom;
						delta = square(delta);
						MatrixOperator<Number>::_result = Mat<Number>(delta.n_rows, 1);
						MatrixOperator<Number>::_result.col(0) = sum(delta, 1);
						MatrixOperator<Number>::_result = sqrt(MatrixOperator<Number>::_result);
						return true;
					}
					
				private:
					// dimension to perform operation.
					Dimension _dim;
					// the matrix to get distance from
					Mat<Number> _matrixFrom;
				};
			}
		}
	}
}

#endif