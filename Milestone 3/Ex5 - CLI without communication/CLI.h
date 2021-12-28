

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;
/*
 * This class creates the Command Line Interface.
 */
class CLI {
	DefaultIO* dio;
	// you can add data members
    float input=0.0;
    vector<AnomalyReport> anomalyDetectResults;
    HybridAnomalyDetector ad;
public:
    CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();

	void setDIO(DefaultIO* dio){
	    this->dio = dio;
	}
};

#endif /* CLI_H_ */
