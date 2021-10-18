//
// Created by Kfir Inbal 209013168
//

#include <vector>
#include <iostream>
#include <fstream>
#include "timeseries.h"
#include <bits/stdc++.h>
#include <list>

//This function returns the Map(vector of pairs) of the features and their data.
std::vector<std::pair<std::string, std::vector<float>>> TimeSeries::getMap() const{
    return featuresMap;
}

//This is helper function for "load" function. It helps splitting by comma the line of data it got from the load function.
//Also it pushes the data into a vector of pairs, to save the data from the csv file.
void TimeSeries::splitLineByComma(std::string line, char delimiter, int lineCounter) {
    std::string featureValue;
    std::stringstream streamedLine(line),stream(line);
    std::vector<std::string> stringLine; //Vector for first line of the table.
    std::vector<float> x{}, floatLine; //Vector for the next lines that contain floats.
    int col = 0;
    while(getline(streamedLine, featureValue, delimiter)) {
        if(lineCounter == 0) //if it's the first line of CSV file(the features names row)
            featuresMap.push_back(std::make_pair(featureValue,x));
        else {
            featuresMap[col].second.push_back(std::stof(&featureValue[0]));
            col++;
        }
    }
}

// Loads a CSV file into our vector of pairs data structure(featuresMap), line by line.
void TimeSeries::load() {
    std::ifstream file(fileName);
    std::string line = "";
    int lineCounter = 0;

    while (getline(file, line)) {
        splitLineByComma(line, ',',lineCounter);
        lineCounter++;
    }
    file.close();
}

//Prints the data from featuresMap(which contains all the data from the CSV file)
void TimeSeries::print() {
    for (auto it = featuresMap.begin();it != featuresMap.end(); it++) {
        std::cout << it -> first << "\t";
    }
    std::cout << std::endl;
    for(int m = 0; m < featuresMap[0].second.size(); m++) {
        for (int j = 0; j < featuresMap.size(); j++) {
            std::cout << featuresMap[j].second[m] << "\t";
        }
        std::cout << std::endl;
    }
}

//Adds new line to featuresMap, by the end.
void TimeSeries::add(std::vector<float> newLine) {
    for(int i = 0; i < newLine.size(); i++){
        featuresMap[i].second.push_back(newLine[i]);
    }
}

//Returns only the row of features names.
std::vector<std::string> TimeSeries::getFeaturesNames() {
    std::vector<std::string> featuresNames;
    for(int i = 0;i <featuresMap.size(); i++) {
        featuresNames.push_back(featuresMap[i].first);
    }
    return featuresNames;
}

//Returns the data of a given feature.
std::vector<float> TimeSeries::getFeatureData(std::string featureName) const {
    for (int i = 0; i < featuresMap.size(); i++) {
        if (featuresMap[i].first == featureName)
            return featuresMap[i].second;
    }
    std::cout << "Data couldn't be found!";
    std::vector<float> notFound = {};
    return notFound;
}

//Returns the value of a given feature at a given time.
float TimeSeries::getValue(std::string featureName, int time) {
    for (auto it = featuresMap.begin(); it != featuresMap.end(); it++) {
        if (it->first == featureName)
            return it->second[time - 1];
    }
    std::cout << "Value couldn't be found!";
    return 0.0;
}

//Changing the names of the features, from left to right.
void TimeSeries::updateFeaturesNames(std::list<std::string> featuresNames) {
    std::list<std::string>::iterator itList = featuresNames.begin();
    auto itMap = featuresMap.begin();
    for(;itList != featuresNames.end(); itMap++, itList++) {
        itMap ->first = *itList;
    }
}
