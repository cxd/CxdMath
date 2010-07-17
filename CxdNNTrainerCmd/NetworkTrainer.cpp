/*
 *  NetworkTrainer.cpp
 *  CxdNNTrainerCmd
 *
 *  Created by Chris Davey on 12/07/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#include "NetworkTrainer.h"


using namespace std;

namespace au {
	namespace id {
		namespace Cxd {
			
			
			NetworkTrainer::NetworkTrainer(ConfigReader *r)
			{
				this->config = r;
			}
			
			NetworkTrainer::~NetworkTrainer()
			{
				delete this->config;
				this->config = NULL;
			}
			
			void NetworkTrainer::TrainNetwork()
			{
				McCullochPittsNeuralNetwork<double> network(this->config->getNetworkFile());
				network.Read();
				// load the training data.
				mat source, targets;
				source.load(this->config->getTrainData());
				targets.load(this->config->getTargetData());
				this->DefineNetworkTrainingData(source, targets);
				network.setSource(this->_trainData);
				network.setTargets(this->_targetData);
				network.connectToErrorSignal(
									 boost::bind(&NetworkTrainer::OnErrorSignal, this, _1));
				network.TrainNetwork(this->config->getEpochs(), 0.005);
				// save after training.
				network.Write(this->config->getNetworkFile());
				// test the network.
				McCullochPittsNeuralNetwork<double> testNet(this->_testData,
															this->_testTargetData,
															network.getLearnRate(),
															network.getMomentum(),
															network.getTemperature(),
															network.getAmplitude(),
															network.getBias());
				testNet.InitialiseNetwork(network.getOutputWeights(), network.getHiddenWeights());
				testNet.setAllowBackPropogation(false);
				testNet.Operate();
				
				// determine the error.
				mat testOutputs = htrans(testNet.getOutput());
				// sum the errors.
				double totalMatched = 0.0; 
				for(int i=0;i<testOutputs.n_rows;i++)
				{
					for(int j=0;j<testOutputs.n_cols;j++)
					{
						if (testOutputs(i,j) == this->_testTargetData(i,j))
						{
							totalMatched += 1.0;	
						}
					}
				}
				totalMatched = totalMatched / (1.0*testOutputs.n_rows);
				cout << "Accuracy: " << totalMatched << endl;
			}
			
			
			/**
			 Receive the callback from the error signal.
			 **/
			void NetworkTrainer::OnErrorSignal(arma::mat &error) const
			{
				cout << "Error Signal: " << endl;
				cout << setprecision (8) << error(0,0) << endl;
			}
			
			/*
			 Split the training and test data from the source data.
			 */
			void NetworkTrainer::DefineNetworkTrainingData(arma::mat sourceData, arma::mat targetData)
			{
				int percent = sourceData.n_rows * (75*1.f)/(sourceData.n_rows*1.f);
				int rows = sourceData.n_rows - percent;	
				
				// use a random permutation of the source data.
				mat shuffledData = join_rows(sourceData, targetData);
				shuffledData = shuffle(shuffledData, 0);
				
				this->_trainData = shuffledData.submat(0,0,rows,shuffledData.n_cols-2);
				this->_targetData = shuffledData.submat(0,shuffledData.n_cols-1,rows,shuffledData.n_cols-1);
				this->_testData = shuffledData.submat(rows+1, 0, shuffledData.n_rows-1,shuffledData.n_cols-2);
				this->_testTargetData = shuffledData.submat(rows+1, shuffledData.n_cols-1, shuffledData.n_rows-1,shuffledData.n_cols-1);
			}
		}
	}
}
