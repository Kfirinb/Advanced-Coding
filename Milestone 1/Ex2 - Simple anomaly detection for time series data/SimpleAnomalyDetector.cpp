//
// Created by Kfir Inbal 209013168
//
#include "SimpleAnomalyDetector.h"

//This function learns the normal behavior of the features data according to a set up threshold.
//finally it uses another function to create a list(vector) of structs that contains the correlated features.
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    float correlation,max_correlation = 0;
    std::vector<std::pair<std::string, std::vector<float>>> fMap = ts.getMap();
    std::vector<float> x, y,data_ymax;
    std::string xName,ymax_Name;
    for (int i = 0; i < fMap.size() - 1; i++) {
        x = ts.getFeatureData(fMap[i].first);
        xName = fMap[i].first;
        for (int j = i + 1; j < fMap.size(); j++) {
            y = ts.getFeatureData(fMap[j].first);
            correlation = pearson(&x[0], &y[0], y.size());
            if(correlation < 0) //we want the absolute value of the correlation
                correlation = correlation * -1;
            if(correlation >= max_correlation) { //assures that we can only 2 features with maximum correlation value.
                max_correlation = correlation;
                ymax_Name = fMap[j].first;
                data_ymax = y;
            }
        }
        createCorrelatedFeatures(x,xName,data_ymax,ymax_Name,max_correlation);
        max_correlation = 0;
    }
}

//This function creates a new correlated features struct if the max correlation
// is greater than the correlation threshold
void SimpleAnomalyDetector::createCorrelatedFeatures(std::vector<float> x,std::string xName,std::vector<float> y,
             std::string yName,float max_correlation) {
    correlatedFeatures cf;
    if (max_correlation >= threshold) {//if the max correlation is greater than threshold.
        setBasicCFParam(cf,x, xName, y, yName, max_correlation);//creates struct of the correlated features
        SimpleAnomalyDetector::deviationThresholdCalc(x,y,cf);
        pushNormalModel(cf);//pushes the struct into a vector of these structs
    }
}
//This function converts 2 vectors of float into array of points
void SimpleAnomalyDetector::toPoints(Point** arrayP,std::vector<float> x, std::vector<float> y) {
    for (int m = 0; m < x.size(); m++)
        arrayP[m] = new Point(x[m], y[m]);
}
//This function sets the basic variables of the correlatedfeatures struct
void SimpleAnomalyDetector::setBasicCFParam(correlatedFeatures &cf,std::vector<float> x,std::string xName,std::vector<float> y,
                                                             std::string yName,float correlation) {
    cf.feature1 = xName;
    cf.feature2 = yName;
    cf.corrlation = correlation;
}
//This function calculates the deviation threshold
// and then sets the result into the threshold variable of the cf struct
void SimpleAnomalyDetector::deviationThresholdCalc(std::vector<float> x,std::vector<float> y,correlatedFeatures &cf) {
    std::vector<float> allDeviations;
    Point *arrayP[x.size()];
    toPoints(arrayP, x,y);
    cf.lin_reg = linear_reg(arrayP, x.size());
    for (int i = 0; i < x.size(); i++)
        allDeviations.push_back(dev(*arrayP[i], cf.lin_reg));
    for (int g = 0; g < x.size(); g++)
        delete arrayP[g];
    cf.threshold = *max_element(allDeviations.begin(), allDeviations.end()) * float(1.2);
}

//This function calculates the deviation of a new point from the linear regression.
float SimpleAnomalyDetector::distanceFromDeviationCalc(correlatedFeatures cf, Point* new_point) {
    float distance;
    distance = dev(*new_point, cf.lin_reg);
    return distance;
}
//This function detects, for a given new timeseries, if a point has exceeded the max deviation(threshold).
//It creates a report of the 2 correlated new features that exceeded the max deviation(threshold).
//finally it returns a list(vector) of those reports.
std::vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
    std::vector<std::pair<std::string, std::vector<float>>> fMap = ts.getMap();
    std::vector<float> x,y;
    Point* new_point;
    float distance;
    std::string features_names_report;
    std::vector<AnomalyReport> all_reports;
    AnomalyReport* new_report;
    for(auto it= allCorrelatedFeatures.begin(); it != allCorrelatedFeatures.end(); it++) {
        x = ts.getFeatureData(it->feature1);
        y = ts.getFeatureData(it->feature2);

        for(int i = 0; i < x.size(); i++) {
            new_point = new Point(x[i], y[i]);
            distance = distanceFromDeviationCalc(*(it),new_point);
            if (distance > it->threshold) {
                features_names_report = it->feature1 + "-" + it->feature2;
                new_report = new AnomalyReport(features_names_report, i + 1);
                all_reports.push_back(*new_report);
            }

            }
        }

    return all_reports;
}