/*
 *  LogSigOperator.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 11/04/10.
 *  Copyright 2010 none. All rights reserved.
 * 
 This class provides an implementation of the LogSig activation function
 over an input matrix.
 
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
 %o = (i ./ (1 + exp(-1*(h/T)))) + c;
 %do = i/T .* ( (o - c)/A) .* (1 - (o - c) / A);
 
 o = i ./ (1 + exp(-1*(T*h)));
 do = T.*o .* ( i - o);
 
 */

#ifndef CXD_MATRIX_LOGSIG
#define CXD_MATRIX_LOGSIG

#include "IOperator.hpp"
#include "MatrixOperator.hpp"

#include "armadillo"

using namespace arma;

namespace au {
		namespace id {
			namespace Cxd {
				namespace Operator {
					template <class Number>
					class LogSigOperator : public MatrixOperator<Number>
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
						 **/
						LogSigOperator(Mat<Number> &h,
									   Number &temp,
									   Number &amp) 
						: 
						MatrixOperator<Number>::MatrixOperator(h)
						{
							_temperature = temp;
							_amplitude = amp;
						}
						
						/**
						Apply the sigmoid function and store the result.
						 
						 The function is:
						 
						 σ(T,h) =	1	/ 1 + e−h/T
						 
						 In matlab: 
						 
						 i = ones(size(h))*A;
						 o = i ./ (1 + exp(-1*(T*h)));
						 do = T.*o .* ( i - o);
						 
						 **/
						virtual bool Operate() 
						{
							Mat<Number> h = MatrixOperator<Number>::_source;
							Mat<Number> i = ones(h.n_rows, h.n_cols) * _amplitude;
							Mat<Number> result = 
							i / (1 + exp(-1 * (_temperature * h) ) );
							MatrixOperator<Number>::_result = result;
							
							// compute the derivative.
							_derivative = _temperature * result % (i - result);
							
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
						
						Mat<Number> _derivative;
					};
				}
			}
		}
}

#endif

