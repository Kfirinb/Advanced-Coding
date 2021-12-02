//
// Created by Kfir Inbal 209013168
//
#include "HybridAnomalyDetector.h"
//This function learns the normal behavior of the features data according to a set up threshold.
//In contrast to SimpleAnomalyDetector, in here it distinguishes between circle and linear regression.
//finally it uses another function to create a list(vector) of structs that contains the correlated features.
void HybridAnomalyDetector::createCorrelatedFeatures(std::vector<float> x, std::string xName, std::vector<float> y, std::string yName,float max_correlation) {
    correlatedFeatures cf;
    if (max_correlation >= threshold)//if the max correlation is greater than threshold.
       SimpleAnomalyDetector::createCorrelatedFeatures(x,xName,y,yName,max_correlation);
    else if(threshold > max_correlation && max_correlation > 0.5) {
        setBasicCFParam(cf,x, xName, y, yName, max_correlation);//creates struct of the correlated features
        HybridAnomalyDetector::deviationThresholdCalc(x,y,cf);
        pushNormalModel(cf);
    }
}
//This function calculates the deviation threshold for a circle scattering
// and then sets the result into the threshold variable of the cf struct
void HybridAnomalyDetector::deviationThresholdCalc(std::vector<float> x,std::vector<float> y,correlatedFeatures &cf) {
    Point *points[x.size()];
    toPoints(points,x,y);
    Circle circle = findMinCircle(points,x.size());
    cf.circle_center = circle.center;
    cf.threshold  = circle.radius * float(1.1);
}
//This function calculates the deviation of a new point from the linear regression if it a linear regression struct
//else it calculates the deviation of new point from a circle scattering struct(distance of new point from circle center)
float HybridAnomalyDetector::distanceFromDeviationCalc(correlatedFeatures cf, Point* new_point){
    float distance;
    if(cf.lin_reg.a != 0 && cf.lin_reg.b != 0)
        distance = dev(*new_point, cf.lin_reg);
    else
        distance = std::sqrt(std::pow(new_point->x - cf.circle_center.x,2) +
                             std::pow(new_point->y - cf.circle_center.y,2));
    return distance;
}


