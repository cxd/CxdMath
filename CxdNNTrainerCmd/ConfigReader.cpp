/*
*  ConfigReader.cpp
*  CxdNNTrainerCmd
*
*  Created by Chris Davey on 12/07/10.
*  Copyright 2010 none. All rights reserved.
*
*/

#include "ConfigReader.h"

using namespace std;

namespace au {
	namespace id {
		namespace Cxd {
			

			ConfigReader::ConfigReader(string configFile)
			{
				this->_configFile = configFile;	
			}

			ConfigReader::~ConfigReader()
			{
				
			}

			/**
			 Read the config file file.
			 **/
			bool ConfigReader::ReadConfig()
			{
				try {
					ifstream fin(this->_configFile.c_str(), ifstream::in);
					
					while(!fin.eof() && !fin.bad())
					{
						string label;
						fin >> label;
						this->ParseLabel(label, fin);
						fin.ignore();
						
					}
					
					fin.close();
				} catch(exception e) {
					cout << "Error: " << e.what() << endl;
					return false;
				}
				return true;
			}

			/**
			 Parse the data in the current stream.
			 **/
			void ConfigReader::ParseLabel(string &label, istream &fin)
			{
				if (label == "TRAIN_DATA") {
					fin >> this->_trainData;
				} else if (label == "NETWORK_FILE") {
					fin >> this->_networkFile;
				} else if (label == "TARGET_DATA") {
					fin >> this->_targetData;	
				} else if (label == "STARTTARGETCOLUMN") {
					fin >> this->_startTargetColumn;	
				} else if (label == "ENDTARGETCOLUMN") {
					fin >> this->_endTargetColumn;	
				} else if (label == "MOMENTUM") {
					fin >> this->_momentum;	
				} else if (label == "LEARNRATE") {
					fin >> this->_learnRate;	
				} else if (label == "CONTINUOUS") {
					fin >> this->_isContinuous;	
				} else if (label == "EPOCHS") {
					fin >> this->_epochs;	
				} else if (label == "BIAS") {
					fin >> this->_bias;	
				} else if (label == "SOURCE_DATA") {
					fin >> this->_srcData;	
				} else if (label == "NETWORK_NAME") {
					fin >> this->_projectName;
				}
			}
			

			bool ConfigReader::ReadConfig(string configFile)
			{
				this->_configFile = configFile;
				return ReadConfig();
			}


			string ConfigReader::getSrcData()
			{
				return this->_srcData;	
			}

			string ConfigReader::getNetworkFile()
			{
				return this->_networkFile;	
			}

			string ConfigReader::getTrainData()
			{
				return this->_trainData;	
			}

			string ConfigReader::getTargetData()
			{
				return this->_targetData;	
			}

			string ConfigReader::getProjectName()
			{
				return this->_projectName;	
			}

			int ConfigReader::getStartTargetColumn()
			{
				return this->_startTargetColumn;	
			}

			int ConfigReader::getEndTargetColumn()
			{
				return this->_endTargetColumn;	
			}

			double ConfigReader::getMomentum()
			{
				return this->_momentum;	
			}

			double ConfigReader::getLearnRate()
			{
				return this->_learnRate;	
			}

			bool ConfigReader::getIsContinuous()
			{
				return this->_isContinuous;	
			}

			int ConfigReader::getEpochs()
			{
				return this->_epochs;	
			}

			double ConfigReader::getBias()
			{
				return this->_bias;	
			}
		}
	}
}
