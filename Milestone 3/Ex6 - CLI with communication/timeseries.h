//
// Created by Kfir Inbal 209013168
//

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <iostream>
#include <string.h>
#include <vector>
#include <list>
#include <iterator>


//This class represents the name of the csv file and a map(vector of pairs) of its data.
// It also contains functions helps to work with the data in the map.
class TimeSeries{
    std::string fileName;
    std::vector<std::pair<std::string, std::vector<float>>> featuresMap;
public:
    //constructor of TimeSeries object
    TimeSeries(const char* CSVfileName){
        fileName.append(CSVfileName);
        load();
    }
    ~TimeSeries(){}

    void load();

    void splitLineByComma(std::string line, char delimiter, int lineCounter);

    void print();

    std::vector<std::pair<std::string, std::vector<float>>> getMap() const;

    void add(std::vector<float> newLine);

    std::vector<std::string> getFeaturesNames();
    template<typename T> void printVector(const T& vector) {
        for (int i = 0; i < vector.size(); i++) {
            if (i == vector.size() - 1)
                std::cout << vector[i] <<std::endl;
            else
                std::cout << vector[i] << "\t";
        }
    }
    std::vector<float> getFeatureData(std::string featureName) const;

    float getValue(std::string featureName, int time);

    void updateFeaturesNames(std::list<std::string> featuresNames);

};



#endif /* TIMESERIES_H_ */
