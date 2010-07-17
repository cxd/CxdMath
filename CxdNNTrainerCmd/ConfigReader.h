/*
 *  ConfigReader.h
 *  CxdNNTrainerCmd
 *
 *  Created by Chris Davey on 12/07/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#ifndef CXD_CONFIG_READER
#define CXD_CONFIG_READER

#include <iostream>
#include <fstream>

using namespace std;

namespace au {
	namespace id {
		namespace Cxd {
			
			class ConfigReader {
				
			public:
				
				ConfigReader(string configFile);
				
				~ConfigReader();
				
				bool ReadConfig();
				
				bool ReadConfig(string configFile);				
				
				
				string getSrcData();
				
				string getNetworkFile();
				
				string getTrainData();
				
				string getTargetData();
				
				string getProjectName();
				
				int getStartTargetColumn();
				
				int getEndTargetColumn();
				
				double getMomentum();
				
				double getLearnRate();
				
				bool getIsContinuous();
				
				int getEpochs();
				
				double getBias();
			
				void ParseLabel(string &label, istream &fin);
				
			private:
				
				string _configFile;
				
				string _srcData;
				string _networkFile;
				string _trainData;
				string _targetData;
				string _projectName;
				
				int _startTargetColumn;
				int _endTargetColumn;
				double _momentum;
				double _learnRate;
				bool _isContinuous;
				int _epochs;
				double _bias;
				
			};
			
		}
	}
}
#endif