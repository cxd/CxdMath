/*
 *  BackPropogration.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 12/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 A back propogation class to perform the weight update of a network.
 
 Example matlab code:
 
 function [ W, g, dW, e ] = backprop( C, W, dW, a, x, o, do, r, g, hn )
 % backprop( C, W, dW, a, x, o, do, r, g, hn )
 % Back propogate for a single neuron.
 % If neuron is output
 % C = vector of target classes
 % otherwise if neuron is hidden
 % C = Weights of next layer l+1
 % W = Weight matrix
 % dW = the previous delta weight matrix
 % a = a small value between 0 and 1 used for momemtum.
 % x = outputs of previous layer
 % o = output of current layer
 % do -= derivative of current layers output
 % r = learning rate
 % if neuron is hidden 
 % g = gradient of next layer l+1
 % hn = flag indicates whether layer is hidden 1 or 0 if it is output
 % Returns:
 % W - updated weight matrix
 % g - gradient of update.
 
 if (hn == 0)
 e = C - o;
 g = do .* e;
 dw = r*g*x';
 dw = a*dW + dw';
 else
 % 
 e = g'*C;
 % g = do'*e';
 g = do.*e';
 dw = r*sum(x*g', 2);%r*sum(g'*x');
 dw = repmat(dw, size(dw,2), size(W,1)); %repmat(dw, size(W,1), size(dw,1));
 dw = a*dW + dw';
 end
 W = W + dw;
 dW = dw;
 
 */


#ifndef CXD_MATRIX_MCP_BACKPROP
#define CXD_MATRIX_MCP_BACKPROP

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
				
				template <class Number>
				class BackPropogration : public MatrixOperator<Number>
				{
				public:
					
					/**
					 Initialise back propogation for the outer layer.
					 
					 outputs - outputs of previous layer
							 - these are the outputs of the hidden layer prior to the output layer.
					 
					 previousOutputs - the outputs of the previous hidden layer
					 
					 weights - the weights of the output layer.
					 
					 deltaWeights - the delta weights of the output layer.
					 
					 targets - the targets for the output layer used for training.
					 
					 derivative - the derivative of the previous layer
					 
					 learnRate - the learning rate of the network
					 
					 momentum - a small value between 0 and 1 for network momentum
					 - to avoid being stuck in local minima
					 
					 **/
					BackPropogration(Mat<Number> outputs,
									 Mat<Number> previousOutputs,
									 Mat<Number> weights,
									 Mat<Number> deltaWeights,
									 Mat<Number> targets,
									 Mat<Number> derivative,
									 Number learnRate,
									 Number momentum) 
					: MatrixOperator<Number>::MatrixOperator(outputs)
					{
						_isHidden = false;
						_weights = weights;
						_deltaWeights = deltaWeights;
						_targets = targets;
						_derivative = derivative;
						_learnRate = learnRate;
						_momentum = momentum;
						_previousOutputs = previousOutputs;
					}
					
					
					/**
					 Initialise the back propogation for a hidden layer.
					 
					 
					 previousOutputs - the outputs of the previous layer 
									 - in a network with 2 layers these will be the inputs to the network
					 
					 weights - the weights of the current layer.
					 
					 deltaWeights - the delta weights of the current layer.
					 
					 nextLayerWeightTargets - the weights of the next layer in front of the current hidden layer
											- in the network these are used for targets.
					 
					 derivatives - the derivative of the outputs of the previous layer.
					 
					 learnRate - the learning rate of the network
					 
					 momentum - a small value between 0 and 1 for network momentum
							  - to avoid being stuck in local minima
					 
					 nextLayerGradient - the gradient from the next layer.
									   - in a 2 layer network this is the gradient from the output layer backprop
					 **/
					BackPropogration(
									 Mat<Number> previousOutputs,
									 Mat<Number> weights,
									 Mat<Number> deltaWeights,
									 Mat<Number> nextLayerWeightTargets,
									 Mat<Number> derivative,
									 Number learnRate,
									 Number momentum,
									 Mat<Number> nextLayerGradient
									 ) 
					: MatrixOperator<Number>::MatrixOperator(previousOutputs)
					{
						_isHidden = true;
						_weights = weights;
						_deltaWeights = deltaWeights;
						_targets = nextLayerWeightTargets;
						_derivative = derivative;
						_learnRate = learnRate;
						_momentum = momentum;
						_gradient = nextLayerGradient;
						_previousOutputs = previousOutputs;
					}
					
					bool Operate() {
						if (_isHidden)
						{
							HiddenLayerOperation();	
						} else {
							OutputLayerOperation();	
						}
						return true;
					}
					
					/**
					 Compute gradient for the output layer and update the weights.
					 
					 Matlab example:
					 
					 e = C - o;
					 g = do .* e;
					 dw = r*g*x';
					 dw = a*dW + dw';
					 
					 W = W + dw;
					 dW = dw;
					 
					 **/
					void OutputLayerOperation() {
						// compute the error.
						Mat<Number> source = MatrixOperator<Number>::getSource();
						Mat<Number> error = _targets - source;
						// compute the gradient
						_gradient = _derivative % error;
						Mat<Number> dw = _learnRate*_gradient*htrans(_previousOutputs);
						if (_momentum != 0)
						{
							dw = _momentum*(_deltaWeights + htrans(dw));
						} else {
							dw = (_deltaWeights + htrans(dw));
						}
							// do the weight update.
						
						//std::cout << "Source: " << std::endl << source << std::endl;
						//std::cout << "Error: " << std::endl << error << std::endl;
						//std::cout << "Momentum: " << std::endl << _momentum << std::endl;
						
						//std::cout << "Delta: " << std::endl << dw << std::endl;
						//std::cout << "Weights " << std::endl << _weights << std::endl;
						
						
						_weights = _weights + dw;
						_deltaWeights = dw;
						
						//std::cout << "Updated : " << std::endl << _weights << std::endl;
					}
					
					/**
					 Compute gradient for the hidden layer and update the weights.
					 
					 Matlab example:
					 
					 e = g'*C;
					 % g = do'*e';
					 g = do.*e';
					 dw = r*sum(x*g', 2);%r*sum(g'*x');
					 dw = repmat(dw, size(dw,2), size(W,1)); %repmat(dw, size(W,1), size(dw,1));
					 dw = a*dW + dw';
					 
					 W = W + dw;
					 dW = dw;
					 **/
					void HiddenLayerOperation() {
						Mat<Number> error = htrans(_gradient)*_targets;
						_gradient = _derivative % htrans(error);
						// create a new delta weight matrix in the correct size.
						Mat<Number> dw = _learnRate * sum(htrans(_previousOutputs)*htrans(_gradient), 1);
						dw = repmat(dw, dw.n_cols, _weights.n_rows);
						if (_momentum != 0) {
							dw = _momentum*_deltaWeights + htrans(dw);
						} else {
							dw = _deltaWeights + htrans(dw);
						}
						_weights = _weights + dw;
						_deltaWeights = dw;
					}
					
					/*
					 Retrieve the updated weights matrix
					 */
					Mat<Number> getWeights() {
						return _weights;	
					}
					
					/*
					 Retrieve the delta weights matrix.
					 */
					Mat<Number> getDeltaWeights() {
						return _deltaWeights;	
					}
					
					/*
					 Retrieve the gradient.
					 */
					Mat<Number> getGradient() {
						return _gradient;	
					}
					
					
					
				private:
					bool _isHidden;
					Mat<Number> _weights;
					Mat<Number> _deltaWeights;
					Mat<Number> _targets;
					Mat<Number> _derivative;
					Mat<Number> _previousOutputs;
					Number _learnRate;
					Number _momentum;
					Mat<Number> _gradient;
				};
				
			}
		}
	}
}

#endif