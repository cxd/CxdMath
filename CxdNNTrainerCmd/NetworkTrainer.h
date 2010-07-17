/*
 *  NetworkTrainer.h
 *  CxdNNTrainerCmd
 *
 *  Created by Chris Davey on 12/07/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#ifndef CXD_CMD_NET_READER
#define CXD_CMD_NET_READER

#include <iostream>
#include <fstream>
#include <iomanip>
#include "CxdMath.h"
#include "ConfigReader.h"

using namespace std;
using namespace arma;
using namespace au::id::Cxd::Operator;

namespace au {
	namespace id {
		namespace Cxd {

			class NetworkTrainer {
				
			public:
				
				NetworkTrainer(ConfigReader *r);
				~NetworkTrainer();
				
				
				void TrainNetwork();
				
				/**
				 Receive the callback from the error signal.
				 **/
				void OnErrorSignal(arma::mat &error) const;
				
				/*
				 define the training data for the network.
				 */
				void DefineNetworkTrainingData(arma::mat sourceData, arma::mat targetData);
				
			private:
				
				ConfigReader *config;
				mat _trainData;
				mat _targetData;
				mat _testData;
				mat _testTargetData;
				
			};
		}
	}
}
#endif