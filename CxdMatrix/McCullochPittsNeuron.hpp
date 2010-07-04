/*
 *  McCullochPittsNeuron.hpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 11/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 
 This class represents a McCulloch and Pitts neuron.
 
 Using the activation function 
 
 y = o(W'*x)
 
 The function o is parameterised as the type of selected operator
 defined as the enumeration type for the activation function.
 
 */


#ifndef CXD_MATRIX_MCP_NEURON
#define CXD_MATRIX_MCP_NEURON

#include "IOperator.hpp"
#include "MatrixOperator.hpp"
#include "RangeSigOperator.hpp"
#include "LogSigOperator.hpp"
#include "armadillo"

using namespace arma;

namespace au {
	namespace id {
		namespace Cxd {
			namespace Operator {
				
				enum ActivationFunction {
					LOGSIG = 0,
					RANGESIG = 1
				};
				
				template <class Number>
				class McCullochPittsNeuron : public MatrixOperator<Number>
				{
				public:
					/**
					 Initialise without data.
					**/
					McCullochPittsNeuron()
					: MatrixOperator<Number>::MatrixOperator()
					{
					}
					
					/**
					 x - input matrix
					 W - weight matrix
					 fn - activation function to use.
					 temp - temperature for activation function.
					 amp - amplitude for activation function
					 c - range for activation function
					 **/
					McCullochPittsNeuron(Mat<Number> x,
										 Mat<Number> W,
										 Number temp,
										 Number amp,
										 Number bias)
					: MatrixOperator<Number>::MatrixOperator(x)
					{
						_temperature = temp;
						_amplitude = amp;
						_weights = W;
						_fn = LOGSIG;
						_bias = bias;
					}
					
					
					/**
					 x - input matrix
					 W - weight matrix
					 fn - activation function to use.
					 temp - temperature for activation function.
					 amp - amplitude for activation function
					 c - range for activation function
					 **/
					McCullochPittsNeuron(Mat<Number> x,
										 Mat<Number> W,
										 ActivationFunction fn,
										 Number temp,
										 Number amp,
										 Number c,
										 Number bias)
					: MatrixOperator<Number>::MatrixOperator(x)
					{
						_temperature = temp;
						_amplitude = amp;
						_range = c;
						_weights = W;
						_fn = fn;
						_bias = bias;
					}
					
					/**
					 Execute the activation function.
					 
					 Matlab example:
					 
					 function [ o, do ] = neuron( W, x, b, k, A, c )
						dim = size(W);
						W(dim(1)+1,:) = b;
						dim = size(x);
						x(dim(1)+1,:) = -1;
						h = W'*x;
						[o,do] = logsig(h, k, A, c);
					 
					 **/
					virtual bool Operate() {
						Mat<Number> W = Mat<Number>(_weights.n_rows + 1, _weights.n_cols);
						W.rows(0, _weights.n_rows - 1) = _weights;
						W.row(W.n_rows - 1) = ((Mat<Number>)(ones(1, _weights.n_cols) * _bias)).row(0);
#ifdef DEBUG
						cout << "Weights: " << endl;
						cout << W << endl;
#endif
						Mat<Number> X = Mat<Number>(MatrixOperator<Number>::_source.n_rows + 1, MatrixOperator<Number>::_source.n_cols);
						X.rows(0, MatrixOperator<Number>::_source.n_rows - 1) = MatrixOperator<Number>::_source;
						X.row(X.n_rows - 1) = ((Mat<Number>)(ones(1, X.n_cols) * -1)).row(0);

#ifdef DEBUG
						cout << "Input: " << endl;
						cout << X << endl;
#endif
						MatrixOperator<Number>::_result = htrans(W)*X;
						
#ifdef DEBUG
						cout << "Result: " << endl;
						cout << MatrixOperator<Number>::_result << endl;
#endif
						Activate();
						return true;
					}

					/**
					 Access the derivative of the logsig operation.
					 Only available after the function has been applied.
					 **/
					Mat<Number> getDerivative() {
						return _derivative;	
					}
					
				protected:
					
					/**
					 Execute the activation function.
					 **/
					void Activate()
					{
					switch(_fn)
						{
							case RANGESIG:
							{
								RangeSigOperator<Number> op(
															MatrixOperator<Number>::_result,
															_temperature,
															_amplitude,
															_range);
								op.Operate();
								MatrixOperator<Number>::_result = op.getResult();
								_derivative = op.getDerivative();
							}	
							break;
								
							default:
							{
								LogSigOperator<Number> logop(
															MatrixOperator<Number>::_result,
															_temperature,
															_amplitude);
								logop.Operate();
								MatrixOperator<Number>::_result = logop.getResult();
								_derivative = logop.getDerivative();
							}
								break;
						}
						
					}
					
				private:
					
					Number _temperature;
					Number _amplitude;
					Number _range;
					Number _bias;
					ActivationFunction _fn;
					Mat<Number> _derivative;
					Mat<Number> _weights;
				};
			}
		}
	}
}
#endif