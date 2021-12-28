#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_
//
// Created by Kfir Inbal 209013168
//
#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
    float threshold;
public:
	HybridAnomalyDetector(){
	    threshold = SimpleAnomalyDetector::getThresholdValue();
	};
	virtual ~HybridAnomalyDetector(){};
    virtual void createCorrelatedFeatures(std::vector<float> x, std::string xName, std::vector<float> y, std::string yName,
                  float max_correlation);
    virtual void deviationThresholdCalc(std::vector<float> x,std::vector<float> y,correlatedFeatures &cf);
    virtual float distanceFromDeviationCalc(correlatedFeatures cf, Point* new_point);
    float getThresholdValue() { return this->threshold;}
    void setThreshold(float newThreshold) {this->threshold =newThreshold;}
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
