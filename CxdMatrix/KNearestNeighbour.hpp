/*
 *  KNearestNeighbour.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 24/07/10.
 *  Copyleft 2010 none. All rights reserved.
 *
 */

#ifndef CXD_MATRIX_KNEAREST
#define CXD_MATRIX_KNEAREST

#include <vector>
#include <algorithm>
#include "IOperator.hpp"
#include "MatrixOperator.hpp"
#include "MinMaxNormaliseMatrix.hpp"
#include "EuclideanDistance.hpp"
#include "CovarianceOperator.hpp"
#include "MahalanobisDistance.hpp"
#include "armadillo"

using namespace arma;
using namespace std;

namespace au {
	namespace id {
		namespace Cxd  {
			namespace Operator {
				
				/**
				 The distance operator to use to 
				 calculate nearest neighbour.
				 **/
				enum DistanceOperator {
					EUCLIDEAN,
					MAHALANOBIS
				};
				
				/**
				 A container class to represent a neighbour.
				 **/
				template <class Number> class Neighbour {
				
				public:
				
					Neighbour() {
						
					}
					
					/**
					 Initialise with member values.
					 **/
					Neighbour(int idx, Row<Number> target, Row<Number> dist)
					{
						_index = idx;
						_target = target;
						_distance = dist;
					}
					
					/**
					 The index of the record in the sample data set.
					 **/
					int getIndex() {
						return _index;
					}
					
					/**
					 Assign the index of the corresponding sample.
					 **/
					void setIndex(int i) {
						_index = i;
					}
					
					/**
					 The distance of the neighbour from the input sample.
					 **/
					Row<Number> getDistance() {
						return _distance;
					}
					
					/**
					 Store the distance from the input sample.
					 **/
					void setDistance(Row<Number> &dist)
					{
						_distance = dist;
					}
					
					/**
					 The target associated with the neighbour.
					 **/
					Row<Number> getTarget() 
					{
						return _target;
					}
					
					/**
					 Store the target.
					 **/
					void setTarget(Row<Number> &target)
					{
						_target = target;	
					}
					
					int CompareTo(Neighbour<Number> &b)
					{
						if (_distance.n_cols != b.getDistance().n_cols)
							return 1;
						Row<Number> n = sum(_distance - b.getDistance());
						if (n(0) == 0) return 0;
						if (n(0) < 0) return -1;
						return 1;
					}
					
					
					Number getDistanceFrom(Neighbour<Number> &b)
					{
						if (_distance.n_cols != b.getDistance().n_cols)
							return 1;
						Row<Number> n = sum(_distance - b.getDistance());
						return n(0);
					}
					
				private:
					Row<Number> _target;
					Row<Number> _distance;
					int _index;
				};
				
				/**
				The KNearest neighbour algorithm
				 
				 The K nearest neighbour uses an existing set of samples
				 to determine the classification associated with the K-nearest set of values.
				 
				 The output of the algorithm provides the row index and the target and the distance
				 associated with the result.
				 
				 The result matrix is the target that has the closest proximity
				 and has the highest number of associated samples.
				 
				 **/
				template <class Number> class KNearestNeighbour : public MatrixOperator<Number>
				{
					
				public:
					
					/**
					 Initialise with the number of neighbours to associate
					 the source sample matrix.
					 the matrix instance to classify 
					 **/
					KNearestNeighbour(int numNeighbours, 
									  DistanceOperator op, 
									  Row<Number> inst, 
									  Mat<Number> source, 
									  Mat<Number> targets) 
					: MatrixOperator<Number>::MatrixOperator(source)
					{
						_instance = inst;
						_KN = numNeighbours;
						_targets = targets;
						_op = op;
					}
					
					/**
					 Perform the operation.
					 @returns
					 True if successful false otherwise.
					 **/
					virtual bool Operate()
					{
						if (MatrixOperator<Number>::_source.n_cols != _instance.n_cols)
						{
							throw runtime_error("Number of columns in instance does not match columns in samples.");	
						}
						if (MatrixOperator<Number>::_source.n_rows != _targets.n_rows)
						{
							throw runtime_error("Number of rows in source is not equal to rows in targets.");	
						}
						if (_KN > MatrixOperator<Number>::_source.n_rows)
						{
							throw runtime_error("K Neighbours must be less than number of rows in samples.");	
						}
						_neighbours = vector<Neighbour<Number> >();
						Mat<Number> A = repmat(_instance, MatrixOperator<Number>::_source.n_rows, 1);
						Mat<Number> dist = 
							(_op == EUCLIDEAN) 
							? DistanceEuclidean(A, MatrixOperator<Number>::_source)
							: DistanceMahalanobis(A, MatrixOperator<Number>::_source);
						
#ifdef DEBUG
						cout << "Instance: " << endl << _instance << endl;
						cout << "Source: " << endl << MatrixOperator<Number>::_source << endl; 
						cout << "Distance: " << endl << dist << endl;
#endif
						
						// find the rows that correspond to the closest records.
						for(int i=0;i<dist.n_rows;i++)
						{
							if (_neighbours.size() < _KN)
							{
								_neighbours.push_back(Neighbour<Number>(i, _targets.row(i), dist.row(i)));	
							} else {
								// sort neighbours from closest to furthest;
								sort(_neighbours.begin(), _neighbours.end(), CompareOp<Number>());
								// naive approach is to iterate of each item in neighbours
								// and find a value that is further away than the current sample.
								for(int j=0;j<_neighbours.size();j++)
								{
									Neighbour<Number> current(i, _targets.row(i), dist.row(i));
									if (current.CompareTo(_neighbours.at(j)) < 0)
									{
										_neighbours[j] = current;
										break;
									}
								}
							}
						}
						// sort from smallest to largest.
						sort(_neighbours.begin(), _neighbours.end(), CompareOp<Number>());
						
#ifdef DEBUG
						for(vector< Neighbour<double> >::iterator itr = _neighbours.begin(); itr != _neighbours.end(); itr++)
						{
							cout << "Neighbour " << 
							"Distance: " << (*itr).getDistance() << endl <<
							"Target: " << (*itr).getTarget() << endl <<
							"Index = " << (*itr).getIndex() << endl;
						}
#endif						
						// find the class that has most closest neighbours of the same type.
						// if there is no majority take the closest neighbour (K reduces to 1).
						map<Number, int> pairs;
						for(int i=0;i<_KN;i++)
						{
							Number idx = _neighbours.at(i).getTarget()(0);
							if (pairs.count(idx) == 0)
							{
								pairs[idx] = 1;
							} else {
								pairs[idx] = pairs[idx]+1;	
							}
							
						}
						// find the pair with the majority count.
						int max = -1;
						Number idx = 0;
						
						for(map<double,int>::iterator pairItr=pairs.begin();pairItr != pairs.end();pairItr++)
						{
#ifdef DEBUG
							cout << "Match Target: " << (*pairItr).first << " Count: " << (*pairItr).second << endl;
#endif
							if ((*pairItr).second > max)
							{
								idx = (*pairItr).first;
								max = (*pairItr).second;
							}
						}
						MatrixOperator<Number>::_result = Mat<Number>(1,1);
						MatrixOperator<Number>::_result(0,0) = idx;
						return true;
					}
					
					
				protected:
					
					/**
					 Compare two neighbours by distance.
					 **/
				template <class T> class CompareOp {
					public:
						CompareOp()
						{
						}
						
						bool operator()(Neighbour<T> a, Neighbour<T> b) const
						{
							int n = a.CompareTo(b);
							if (n < 0) return true;
							return false;
						}
						
					};
					
					/**
					 A method to determine whether to replace an item if it equals the
					 current replacement index.
					 **/
					
				template <class T> class ReplaceOp {
					public:
						ReplaceOp(int idx)
						{
							_idx = idx;
						}
						
						bool operator()(Neighbour<T> &a) const {
							return a.getIndex() == _idx;
						}
						
					private:
						int _idx;
					};
					
					
					Mat<Number> DistanceEuclidean(Mat<Number> &A, Mat<Number> &B)
					{
						EuclideanDistance<Number> dist(A, B, COLS);
						dist.Operate();
						// potentially may need to rescale.
						return dist.getResult();
					}
					
					Mat<Number> DistanceMahalanobis(Mat<Number> &A, Mat<Number> &B)
					{
						throw runtime_error("Mahalanobis distance is not yet implemented correctly use euclidean distance.");
						MahalanobisDistance<Number> mdist(A, B);
						mdist.Operate();
						// potentially may need to rescale.
						return mdist.getResult();
					}
					
					
				private:
					Row<Number> _instance;
					Mat<Number> _targets;
					int _KN;
					vector<Neighbour<Number> > _neighbours;
					DistanceOperator _op;
				};
			}
		}
	}
}
#endif
