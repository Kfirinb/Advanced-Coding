//
// Created by Kfir Inbal 209013168
//
#ifndef ANOMALYDETECTOR_H_
#define ANOMALYDETECTOR_H_



#include <string>
#include <vector>
#include "timeseries.h"
#include "math.h"

//This class represents the description and time step of 2 new features data that exceeded the max deviation.
class AnomalyReport{
public:
    const std::string description; //the 2 features names separated by "-"
    const long timeStep; //the time interval, starting from 1.
    //constructor of this class
    AnomalyReport(std::string description, long timeStep):description(description),timeStep(timeStep){}
};

//This class represents the interface of our program.
class TimeSeriesAnomalyDetector {
public:
    virtual ~TimeSeriesAnomalyDetector(){};
    virtual void learnNormal(const TimeSeries& ts)=0;
    virtual std::vector<AnomalyReport> detect(const TimeSeries& ts)=0;
};

#endif /* ANOMALYDETECTOR_H_ */

