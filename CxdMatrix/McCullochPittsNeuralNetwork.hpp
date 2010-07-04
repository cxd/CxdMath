/*
 *  McCullochPittsNeuralNetwork.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 17/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */


#ifndef CXD_IF_MATRIX_OP_MCULLOCH_PITTS_NEURAL_NET
#define CXD_IF_MATRIX_OP_MCULLOCH_PITTS_NEURAL_NET

#include "IOperator.hpp"
#include "CxdMath.h"
#include "armadillo"


using namespace arma;
using namespace std;
using namespace boost::signals2;

namespace au {
	namespace id {
		namespace Cxd {
			namespace Operator {
				
				
				template <class Number> class McCullochPittsNeuralNetwork 
				: public MatrixOperator<Number>

				{
					
				public:
					
					/**
					 Construct with source matrix.
					 **/
					McCullochPittsNeuralNetwork()
					: MatrixOperator<Number>::MatrixOperator()
					{
						_learnRate = 0.0001;
						_momentum = 0;
						_activationFn = LOGSIG;
						_temperature = 1;
						_amplitude = 1;
						_bias = 0;
						_range = 0;
					}
					
					/**
					 Construct using an input file.
					 **/
					McCullochPittsNeuralNetwork(const string& inputFilePath)
					{
						_inputFile = inputFilePath;
					}
					
					/**
					 Construct with source matrix.
					 
					 data = Matrix where each sample is a single row vector
					      - cols = number of features
						  - rows = number of samples
					 
					 targets = Matrix where cols are number of outputs and rows = number of rows in data
					 **/
					McCullochPittsNeuralNetwork(Mat<Number> data,
												Mat<Number> targets)
					: MatrixOperator<Number>::MatrixOperator(data)
					{
						_targets = targets;
						_learnRate = 0.0001;
						_momentum = 0;
						_activationFn = LOGSIG;
						_temperature = 1;
						_amplitude = 1;
						_bias = 0;
						_range = 0;
					}
					
					/**
					 Construct with source matrix.
					 data = Matrix where each sample is a single row vector
					 - cols = number of features
					 - rows = number of samples
					 
					 targets = Matrix where cols are number of outputs and rows = number of rows in data
					 
					 **/
					McCullochPittsNeuralNetwork(Mat<Number> data,
												Mat<Number> targets,
												Number learnRate)
					: MatrixOperator<Number>::MatrixOperator(data)
					{
						_targets = targets;
						_learnRate = learnRate;
						_momentum = 0;
						_activationFn = LOGSIG;
						_temperature = 1;
						_amplitude = 1;
						_bias = 0;
						_range = 0;
						_allowBackpropogation = true;
						_calculateErrors = true;
					}
					
					/**
					 Construct with source matrix.
					 Supply learning rate and momentum.
					 Creates a logsig based network
					 
					 data = Matrix where each sample is a single row vector
					 - cols = number of features
					 - rows = number of samples
					 
					 targets = Matrix where cols are number of outputs and rows = number of rows in data
					 
					 **/
					McCullochPittsNeuralNetwork(Mat<Number> data,
												Mat<Number> targets,
												Number learnRate,
												Number momentum,
												Number temperature,
												Number amplitude,
												Number bias)
					: MatrixOperator<Number>::MatrixOperator(data)
					{
						_targets = targets;
						_learnRate = learnRate;
						_momentum = momentum;
						_activationFn = LOGSIG;
						_temperature = temperature;
						_amplitude = amplitude;
						_range = 0;
						_bias = bias;
						_allowBackpropogation = true;
						_calculateErrors = true;
					}
					
					~McCullochPittsNeuralNetwork()
					{
						
					}
					
					/**
					 Construct with source matrix.
					 Supply learning rate and momentum.
					 As well as activation function type.
					 
					 data = Matrix where each sample is a single row vector
					 - cols = number of features
					 - rows = number of samples
					 
					 targets = Matrix where cols are number of outputs and rows = number of rows in data
					 
					 **/
					McCullochPittsNeuralNetwork(Mat<Number> data,
												Mat<Number> targets,
												Number learnRate,
												Number momentum,
												ActivationFunction fn,
												Number temperature,
												Number amplitude,
												Number range,
												Number bias)
					: MatrixOperator<Number>::MatrixOperator(data)
					{
						_targets = targets;
						_learnRate = learnRate;
						_momentum = momentum;
						_activationFn = fn;
						_temperature = temperature;
						_amplitude = amplitude;
						_range = range;
						_bias = bias;
						_allowBackpropogation = true;
						_calculateErrors = true;
					}
					
					bool Operate() {
						if (!_isInitialised)
							InitialiseNetwork();
						Activate();
						if (_allowBackpropogation)
							BackPropogate();
						if (_calculateErrors)
						{
							CalculateSumSquaredError();
							// notify listeners of error signal.
							_errorSignal(_sumSquaredError);
						}
						return true;
					}
					
					/**
					 Calculate the size of the network.
					 Initialise weights.
					 
					 data = Matrix where each sample is a single row vector
					 - cols = number of features
					 - rows = number of samples
					 
					 targets = Matrix where cols are number of outputs and rows = number of rows in data
					 
					 output rows = number of cols in Input matrix.
					 output columns = number of cols in targets.
					 
					 Hidden Weights Rows = Number of Cols in Input Vector 
					 Hidden Weights Cols = Number of Cols in Input Vector
					 
					 **/
					void InitialiseNetwork() {
						_isInitialised = true;
						int outputCount = _targets.n_cols;
						/*
						 output rows = number of rows in Input matrix.
						 output columns = number of columns in targets.
						 */
						_outputWeights = rand(MatrixOperator<Number>::_source.n_cols, outputCount)/MatrixOperator<Number>::_source.n_rows;
						_outputDeltaWeights = zeros(_outputWeights.n_rows, _outputWeights.n_cols);
						/**
						 Hidden Weights Rows = Number of Rows in Input Vector 
						 Hidden Weights Cols = Number of Rows in Input Vector
						 **/
						mat tempW = rand(MatrixOperator<Number>::_source.n_cols, 1)/MatrixOperator<Number>::_source.n_rows;
						_hiddenWeights = repmat(tempW, 1,MatrixOperator<Number>::_source.n_cols);
						_hiddenDeltaWeights = zeros(_hiddenWeights.n_rows, _hiddenWeights.n_cols);
					}
					
					/**
					 Alternative method of initialisation.
					 Supply the output weights and hidden weights.
					 
					 output rows = number of cols in Input matrix.
					 output columns = number of cols in targets.
					 
					 Hidden Weights Rows = Number of Cols in Input Vector 
					 Hidden Weights Cols = Number of Cols in Input Vector
					 
					 **/
					void InitialiseNetwork(Mat<Number> outputWeights, Mat<Number> hiddenWeights)
					{
						_isInitialised = true;
						_outputWeights = outputWeights; 
						_outputDeltaWeights = zeros(_outputWeights.n_rows, _outputWeights.n_cols);
						_hiddenWeights = hiddenWeights;
						_hiddenDeltaWeights = zeros(_hiddenWeights.n_rows, _hiddenWeights.n_cols);
					}
					
					/**
					 Feedforward activation.
					 **/
					void Activate() {
						/* 
						 hidden neuron
						 State:
						 ACTIVATE_HIDDEN_LAYER
						 */
						McCullochPittsNeuron<Number> neuron(htrans(MatrixOperator<Number>::_source), 
									htrans(_hiddenWeights), 
									_activationFn,
									_temperature, 
									_amplitude, 
									_range,
									_bias);
						_hiddenLayer = neuron;
						_hiddenLayer.Operate();
						
						
						// output neuron
						/*
						 State: ACTIVATE_OUTPUT_LAYER
						 */
#ifdef DEBUG
						cout << "Hidden Result " << endl;
						cout << _hiddenLayer.getResult() << endl;
#endif
						
						McCullochPittsNeuron<Number> neuron2(_hiddenLayer.getResult(), 
															_outputWeights, 
															_activationFn,
															_temperature, 
															_amplitude, 
															_range,
															_bias);
						_outputLayer = neuron2;
						_outputLayer.Operate();

#ifdef DEBUG
						cout << "Output Result " << endl;
						cout << _outputLayer.getResult() << endl;
#endif
					}
					
					/**
					 Backpropogation.
					 **/
					void BackPropogate() {
						/*
						 State: BACKPROP_outputLayer.getResult()UTPUT_LAYER
						 */
						BackPropogration<double> outputBackprop(_outputLayer.getResult(), 
																_hiddenLayer.getResult(), 
																_outputWeights, 
																_outputDeltaWeights, 
																htrans(_targets), 
																_outputLayer.getDerivative(), 
																_learnRate, 
																_momentum);
						outputBackprop.Operate();
						
						// assign the updated weights to the output layer.
						_outputWeights = outputBackprop.getWeights();
						_outputDeltaWeights = outputBackprop.getDeltaWeights();
						
						
						// transverse output weights.
						Mat<Number> nextLayerWeightTargets = htrans(_outputWeights);
						Mat<Number> nextLayerGradient = outputBackprop.getGradient(); 
						
						/*
						 State: BACKPROP_HIDDEN_LAYER
						 */
						BackPropogration<double> hiddenBackprop(MatrixOperator<Number>::_source,
																_hiddenWeights,
																_hiddenDeltaWeights,
																nextLayerWeightTargets,
																_hiddenLayer.getDerivative(),
																_learnRate,
																_momentum,
																nextLayerGradient);
						hiddenBackprop.Operate();
						// assign the updated weights
						_hiddenWeights = hiddenBackprop.getWeights();
						_hiddenDeltaWeights = hiddenBackprop.getDeltaWeights();
					
					}
					
					
					/**
					 Calculate the sum of the squared error
					 **/
					void CalculateSumSquaredError()
					{
						// compute the sum squared error.
						SquaredError<Number> squaredError(htrans(_outputLayer.getResult()), _targets);
						squaredError.Operate();
						_sumSquaredError = squaredError.getResult();
					}
					
					int TrainNetwork(int epochs, Number threshold)
					{
						_trainingEpochs = epochs;
						_errorThreshold = threshold;
						return TrainNetwork();
					}
					
					int TrainNetwork() {
						_allowBackpropogation = true;
						_calculateErrors = true;
						int numEpochs;
						for(int i=0;i<_trainingEpochs;i++)
						{
							numEpochs = i+1;
							Operate();
							Number total = sum(_sumSquaredError.row(0));
							double avg = total/(_sumSquaredError.n_cols*1.0);
							if (avg < _errorThreshold)
								break;
						}
						return numEpochs;
					}
					
					
					Mat<Number> getOutput() {
						return _outputLayer.getResult();
					}
					
					Mat<Number> getSumSquaredError()
					{
						return _sumSquaredError;	
					}
					
					
					Mat<Number> getOutputWeights() {
						return _outputWeights;	
					}
					
					Mat<Number> getHiddenWeights() {
						return _hiddenWeights;	
					}
					
					/*
					 Check if back propogation is allowed
					 */
					bool allowsBackPropogation()
					{
						return _allowBackpropogation;	
					}
					
					/*
					 Set whether to allow back propogation
					 */
					void setAllowBackPropogation(bool flag)
					{
						_allowBackpropogation = flag;	
					}
					
					/*
					 Set whether to calculate errors
					 */
					void setCalculateErrors(bool flag)
					{
						_calculateErrors = flag;	
					}
					
					/**
					 Check if calculate errors is enabled.
					 **/
					bool CanCalculateErrors() {
						return _calculateErrors;	
					}
					
					
					/**
					 Read network properties from a file.
					 
					 **/
					bool Read() {
						try {
							ifstream infile(_inputFile.c_str(), ifstream::in);
							infile >> _temperature;
							infile.ignore();
							infile >> _amplitude;
							infile.ignore();
							infile >> _range;
							infile.ignore();
							infile >> _bias;
							infile.ignore();
							int temp;
							infile >> temp;
							infile.ignore();
							_activationFn = (ActivationFunction)temp;
							infile >> _learnRate;
							infile.ignore();
							infile >> _momentum;
							infile.close();
							_outputWeights.load(_inputFile + "OW.mat");
							_outputDeltaWeights.load(_inputFile + "ODW.mat");
							_hiddenWeights.load(_inputFile + "HW.mat");
							_hiddenDeltaWeights.load(_inputFile + "HDW.mat");
							_isInitialised = true;
							return true;
						} 
						catch(exception e)
						{
						cerr << "Read - Exception raised: " << e.what() << endl;	
						}
						return false;
					}
					
					/**
					 Write network properties to a file.
					 **/
					bool Write(string outputFile) 
					{
						try {
							ofstream outfile(outputFile.c_str(), ofstream::out);
							outfile << _temperature << endl;
							outfile << _amplitude << endl;
							outfile << _range << endl;
							outfile << _bias << endl;
							outfile << (int)_activationFn << endl;
							outfile << _learnRate << endl;
							outfile << _momentum << endl;
							outfile.flush();
							outfile.close();
							_outputWeights.save(outputFile + "OW.mat");
							_outputDeltaWeights.save(outputFile + "ODW.mat");
							_hiddenWeights.save(outputFile + "HW.mat");
							_hiddenDeltaWeights.save(outputFile + "HDW.mat");
							return true;
						} 
						catch(exception e)
						{
							cerr << "Write - Exception raised: " << e.what() << endl;	
						}
						return false;
					}
					
					int getTrainingEpochs()
					{
						return _trainingEpochs;	
					}
					
					void setTrainingEpochs(int epochs)
					{
						_trainingEpochs = epochs;	
					}
					
					Number getErrorThreshold()
					{
						return _errorThreshold;	
					}
					
					void setErrorThreshold(Number t)
					{
						_errorThreshold = t;	
					}
					
					
					void setBias(double b) {
						_bias = b;	
					}
					
					double getBias() {
						return _bias;	
					}
					
					void setMomentum(double m) {
						_momentum = m;	
					}
					
					double getMomentum() {
						return _momentum;
					}
					
					void setLearnRate(double l) {
						_learnRate = l;	
					}
					
					double getLearnRate() {
						return _learnRate;	
					}
					
					double getTemperature() {
						return _temperature;	
					}
					
					void setTemperature(double t)
					{
						_temperature = t;	
					}
					
					double getAmplitude() 
					{
						return _amplitude;	
					}
					
					void setAmplitude(double a)
					{
						_amplitude = a;	
					}
					
					/*
					 Set the training targets.
					 */
					void setTargets(arma::mat t)
					{
						_targets = t;	
					}
					
					/*
					 Get the training targets.
					 */
					arma::mat getTargets()
					{
						return _targets;	
					}
					
					/* Connect a slot to the signal which will be emitted whenever
					 text is appended to the document. */
					boost::signals2::connection connectToErrorSignal(const boost::signals2::signal<void(arma::mat &)>::slot_type &subscriber)
					{
						return _errorSignal.connect(subscriber);
					}
					
					
				protected:
					Number _temperature;
					Number _amplitude;
					Number _range;
					Number _bias;
					ActivationFunction _activationFn;
					Number _learnRate;
					Number _momentum;
					Mat<Number> _targets;
					Mat<Number> _sumSquaredError;
					
					McCullochPittsNeuron<Number> _outputLayer;
					McCullochPittsNeuron<Number> _hiddenLayer;
					
					Mat<Number> _outputWeights;
					Mat<Number> _outputDeltaWeights;
					Mat<Number> _hiddenWeights;
					Mat<Number> _hiddenDeltaWeights;
					
					bool _isInitialised;
					bool _allowBackpropogation;
					bool _calculateErrors;
					
					int _trainingEpochs;
					Number _errorThreshold;
					
					// the input file to load the network from.
					string _inputFile;
					
					
				private:
					/*
					 A signal slot to allow connections to the error signal.
					 */
					boost::signals2::signal<void(arma::mat &)> _errorSignal;			
				};
			}
		}
	}
	
}
#endif