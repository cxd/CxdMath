/*
 *  TanSigOperator.hpp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 11/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 This class provides an implementation of the LogSig activation function
 over an input matrix.
 
 This is the general form of the logsig function.
 
 σ(h) =	( A / (1 + e−h/T) ) + c
 
 This sigmoid has an amplitude A and ranges from c to c + 2A.
 
 An important advantage of the logistic (or hyperbolic tangent sigmoid) (D.16) is that its derivative is very easy to compute: 
 The symbolic calculation of the derivative of the general sigmoid function D.16 gives the following result
 
 dσ/dh	= (A*(1/T))*(e−h/T)/(T*(1+e−h/T)^2)
 
 
 The equivalent matlab function definition is:
 
 function [o, do] = logsig( h, T, A, c )
 % h - combined input to logsig
 % T - temperature expressed as whole number.
 % A - amplitude
 % c - range from [c, 2A] eg c = 0 A = 1 - logistic function. 
 % c = -1 A = 2 - tanh sigmoid
 % Outputs:
 % o - output of logsig
 % do - derivative of output.
 i = ones(size(h))*A;
 o = (i ./ (1 + exp(-1*(h/T)))) + c;
 do = i/T .* ( (o - c)/A) .* (1 - (o - c) / A);
 
 */

#ifndef CXD_MATRIX_RANGE_SIG
#define CXD_MATRIX_RANGE_SIG

#include "IOperator.hpp"
#include "MatrixOperator.hpp"

#include "armadillo"

using namespace arma;

namespace au {
	namespace id {
		namespace Cxd {
			namespace Operator {
				template <class Number>
				class RangeSigOperator : public MatrixOperator<Number>
				{
					
				public:
					
					/**
					 Construct with the following parameters.
					 
					 @param
					 * h	- the combined input to the logsig function
					 - this is the neuron output.
					 *
					 * temp	- this is the temperature 
					 - expressed as a whole number.
					 
					 * amp - this is the amplitude to apply to the operation.
					 
					 * c - this is the range of the function.
					 It is constrained between c to c + 2A.
					 
					 If c = 0 and A = 1 this is equivalent to the LogSig function.
					 
					 If c = 1 and A = 2 this is equivalent to the TanSig function.
					 
					 
					 **/
					RangeSigOperator(Mat<Number> h,
								   Number temp,
								   Number amp,
								   Number c)
					: 
					MatrixOperator<Number>::MatrixOperator(h)
					{
						_temperature = temp;
						_amplitude = amp;
						_range = c;
					}
					
					/**
					 Apply the sigmoid function and store the result.
					 
					 The function is:
					 
					 This is the general form of the logsig function.
					 
					 σ(T,h) =	( A / (1 + e−h/T) ) + c
					 
					 Its derivative is:
					 
					 dσ/dh	= (A*(1/T))*(e−h/T)/(T*(1+e−h/T)^2)
					 
					 In matlab: 
					 
					 i = ones(size(h))*A;
					 o = (i ./ (1 + exp(-1*(h/T)))) + c;
					 do = i/T .* ( (o - c)/A) .* (1 - (o - c) / A);					 
					 **/
					virtual bool Operate() 
					{
						Mat<Number> h = MatrixOperator<Number>::_source;
						Mat<Number> i = ones(h.n_rows, h.n_cols) * _amplitude;
						Mat<Number> result = 
						( i / (1 + exp(-1 * (_temperature * h) ) ) ) + _range;
						
						MatrixOperator<Number>::_result = result;
						
						// compute the derivative.
						_derivative = 
							_amplitude *  (i / _temperature) %
						
							((( result - _range) / _amplitude) % 
						
							 (1 - (result - _range) / _amplitude));
						
						return true;
					}
					
					/**
					 Access the derivative of the logsig operation.
					 Only available after the function has been applied.
					 **/
					Mat<Number> getDerivative() {
						return _derivative;	
					}
					
				private:
					
					Number _temperature;
					Number _amplitude;
					Number _range;
					
					Mat<Number> _derivative;
				};
			}
		}
	}
}

#endif

