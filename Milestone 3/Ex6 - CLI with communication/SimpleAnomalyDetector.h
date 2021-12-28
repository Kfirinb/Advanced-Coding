//
// Created by kfir Inbal 209013168
//
#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include <string.h>
#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>

//This struct represents the information of 2 correlated features from the normal data.
struct correlatedFeatures{
    std::string feature1,feature2; // names of the correlated features
    float corrlation; //correlation between the 2 features
    Line lin_reg;//regression line from the points where each data in feature1 is x and in feature2 is y.
    float threshold;//max deviation from the regression line.
    Point circle_center = Point(0,0);
};

//This class inherits from the class TimeSeriesAnomalyDetector
class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
    float threshold;
    std::vector<correlatedFeatures> allCorrelatedFeatures; //list(vector) of all correlated featuresfound in the normal model
public:
    SimpleAnomalyDetector(){
        threshold = 0.9;
    };
    virtual ~SimpleAnomalyDetector(){};

    virtual void learnNormal(const TimeSeries& ts);
    virtual std::vector<AnomalyReport> detect(const TimeSeries& ts);
    //returns a list(vector) of all correlated features found in the normal model
    std::vector<correlatedFeatures> getNormalModel() {
        return allCorrelatedFeatures;
    }
    void pushNormalModel(correlatedFeatures cf) {
        allCorrelatedFeatures.push_back(cf);
    }
    virtual void createCorrelatedFeatures(std::vector<float> x,std::string xName,std::vector<float> y,
                           std::string yName,float max_correlation);
    void setBasicCFParam(correlatedFeatures &cf,std::vector<float> x,std::string xName,std::vector<float> y,
                                          std::string yName,float correlation);
    virtual void deviationThresholdCalc(std::vector<float> x,std::vector<float> y,correlatedFeatures &cf);
    virtual float distanceFromDeviationCalc(correlatedFeatures cf, Point* new_point);
    void toPoints(Point** arrayP,std::vector<float> x, std::vector<float> y);
    float getThresholdValue() { return threshold;}
};

#endif
