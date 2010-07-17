#include <iostream>
#include "ConfigReader.h"
#include "NetworkTrainer.h"

using namespace std;
using namespace au::id::Cxd;

int main (int argc, char * const argv[]) {
    
	if (argc <= 1)
	{
		cout << "Provide network test configuration file as argument." << endl;
		return 0;
	}
	ConfigReader *reader = new ConfigReader(string(argv[1]));
	if (!reader->ReadConfig())
	{
		cout << "Failed to read config: " << argv[1] << endl;
		return 0;
	}
	cout << "Train Network" << endl 
	<< "Network file: " << reader->getNetworkFile() << endl
	<< "Train Data: " << reader->getTrainData() << endl
	<< "Target Data: " << reader->getTargetData() << endl 
	<< "Epochs: " << reader->getEpochs() << endl
	<< "Bias: " << reader->getBias() << endl
	<< "Learn Rate: " << reader->getLearnRate() << endl 
	<< "Momentum: " << reader->getMomentum() << endl 
	<< "Target Start Column: " << reader->getStartTargetColumn() << endl
	<< "Target End Column: " << reader->getEndTargetColumn() << endl;
	NetworkTrainer trainer(reader);
	trainer.TrainNetwork();
	return 0;
}
