

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

#include <stdlib.h>

using namespace std;

//This class represents an abstract DefaultIO class
class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}
	// you may add additional methods here
};



// you may add here helper classes

//This class represents the description of two reported features and the startTime and EndTime of their
// deviation exceed report
class JointAnomalyReport{
public:
    const std::string description; //the 2 features names separated by "-"
    const long StartTimeStep; //the time interval, starting from 1.
    const long EndTimeStep;
    //constructor of this class
    JointAnomalyReport(std::string description, long StartTimeStep,long EndTimeStep):
            description(description),StartTimeStep(StartTimeStep),EndTimeStep(EndTimeStep){}
};

// you may edit this class
//This class represents an abstract Command class
class Command{
protected:
	DefaultIO* dio;
    string description;
public:
	Command(DefaultIO* dio):dio(dio){
	    this->dio = dio;
	}
	virtual void execute()=0;
	virtual string getDescription(){return description;}
    virtual ~Command(){}

};

// implement here your command classes

//This class inherits from Command class. It represents the "upload a time series csv file" command.
class uploadTS:public Command {
    string csvLine;
public:
    //Constructor
    uploadTS(DefaultIO *dio) : Command(dio) {
        this->dio=dio;
        this->description="1.upload a time series csv file\n";
    };

    virtual void execute() {
        ofstream TrainCSV("anomalyTrain.csv");
        dio->write("Please upload your local train CSV file.\n");
        csvLine=dio->read() +'\n';
        while(csvLine!="done\n") { //reads lines from the user for anomalyTrain file until the user writes "done"
            TrainCSV << csvLine;
            csvLine=dio->read() +'\n';
        }
        dio->write("Upload complete.\n");
        TrainCSV.close();
        csvLine = "";
        ofstream TestCSV("anomalyTest.csv");
        dio->write("Please upload your local test CSV file.\n");
        csvLine=dio->read() +'\n';
        while(csvLine!="done\n") {//reads lines from the user for anomalyTest file until the user writes "done"
            TestCSV << csvLine;
            csvLine=dio->read() +'\n';
        }
        dio->write("Upload complete.\n");
        TestCSV.close();
    }
    virtual string getDescription(){return description;}

};

//This class inherits from Command class. It represents the "algorithm settings" command.
class algoSettings: public Command {
    HybridAnomalyDetector* ad;
    float newThreshold;
public:
    //Constructor
    algoSettings(DefaultIO *dio,HybridAnomalyDetector* ad) : Command(dio) {
        this->ad = ad;
        this->description = "2.algorithm settings\n";
    };
    virtual void execute() {
        dio->write("The current correlation threshold is ");
        dio->write(ad->getThresholdValue());
        dio->write("\nType a new threshold\n");
        dio->read(&newThreshold);
        ad->setThreshold(newThreshold);
    }
    virtual string getDescription(){return description;}
};

//This class inherits from Command class. It represents the "detect anomalies" command.
class detectAnomalies:public Command {
    HybridAnomalyDetector* ad;
    vector<AnomalyReport>* anomalyDetectResults;
public:
    detectAnomalies(DefaultIO *dio,HybridAnomalyDetector* ad,vector<AnomalyReport>* anomalyDetectResults) : Command(dio) {
        this->description="3.detect anomalies\n";
        this->ad =ad;
        this->anomalyDetectResults=anomalyDetectResults;
    };
    virtual void execute() {
        TimeSeries* tsTrain = new TimeSeries("anomalyTrain.csv");
        TimeSeries* tsTest = new TimeSeries("anomalyTest.csv");
        ad->learnNormal(*tsTrain);
        vector<AnomalyReport> results = ad->detect(*tsTest);
        copy(results.begin(), results.end(), back_inserter(*anomalyDetectResults));
        dio->write("anomaly detection complete.\n");
    }
    virtual string getDescription(){return description;}

};
//This class inherits from Command class. It represents the "display results" command.
class displayResults:public Command {
    vector<AnomalyReport>* anomalyDetectResults;
public:
    displayResults(DefaultIO *dio,vector<AnomalyReport>* anomalyDetectResults) : Command(dio) {
        this->description="4.display results\n";
        this->anomalyDetectResults = anomalyDetectResults;
    };
    virtual void execute() {
        for(int i=0;i < (*anomalyDetectResults).size(); i++) {
            dio->write((*anomalyDetectResults)[i].timeStep);
            dio->write('\t' + (*anomalyDetectResults)[i].description + '\r'+'\n');
        }
        dio->write("Done.\n");
    }
    virtual string getDescription(){return description;}

};
//This class inherits from Command class. It represents the "upload anomalies and analyze results" command.
class UploadAnomaliesAnalyze:public Command {
    vector<AnomalyReport>* anomalyDetectResults;
    vector<JointAnomalyReport> jointAnomalyDetectResults;
    bool hasBeenJoint;
    long StartTimeStep,EndTimeStep;
    string Rdescription;
    vector<float> StartEnd;
public:
    //Constructor
    UploadAnomaliesAnalyze(DefaultIO *dio,vector<AnomalyReport>* anomalyDetectResults) : Command(dio) {
        this->description="5.upload anomalies and analyze results\n" ;
        this->anomalyDetectResults = anomalyDetectResults;
        this->hasBeenJoint = false;
    };
    virtual void execute() {
        CreateJointResults();
        dio->write("Please upload your local anomalies file.\n");
        ifstream TestCSV("anomalyTest.csv");
        int n = 0,P=0,TP=0,FP=0,N;
        string line;
        while(getline(TestCSV, line))
            n++;
        N=n-1;
        line = dio->read();
        while(line!="done") {
            P +=1;
            StartEnd = SplitStringToFloats(line);
            float range = StartEnd[1] - StartEnd[0] + 1;
            N = N - range;
            InBetween(StartEnd[0],StartEnd[1],jointAnomalyDetectResults,&TP,&FP);
            line = dio->read();
        }
        dio->write("Upload complete.\n");
        dio->write("True Positive Rate: ");
        dio->write(roundFloat((float)TP/(float)P));
        dio->write("\nFalse Positive Rate: ");
        dio->write(roundFloat((float)FP/(float)N));
        dio->write("\n");

    }
    virtual string getDescription(){return description;}

    //This function checks if the user's local anomaly report is in the frames(start and end time) one of the
    //anomaly detect algoritm reports.
    //This function gets a start time and end time values, and a vector of JointAnomaly Reports and
    // reference to TP and FP variables.
    //if it is then it raises TP value by 1 (True Positive)
    //afterwards it sets FP(False Positive) value to be the total algoritm reports minus the TP value
    void InBetween(float StartTime,float EndTime,vector<JointAnomalyReport> JAR, int* TP, int* FP){
        int lengthOfFrame;
        for(int i=0; i < JAR.size(); i++) {
            lengthOfFrame = JAR[i].EndTimeStep - JAR[i].StartTimeStep;
            if ((StartTime - JAR[i].StartTimeStep >= 0 && StartTime - JAR[i].StartTimeStep <= lengthOfFrame) ||
                (EndTime - JAR[i].StartTimeStep >= 0 && EndTime - JAR[i].StartTimeStep <= lengthOfFrame) ||
                    (StartTime - JAR[i].StartTimeStep <=0 && EndTime - JAR[i].EndTimeStep >=0))
                (*TP)++;
        }
        (*FP) =(JAR.size() - (*TP));
    }

    //This function gets a string line that built as "number,number" and splits it by comma and
    // returns a vector of both numbers
    vector<float> SplitStringToFloats(string line) {
        int commaIndex =line.find(',');
        float StartTime = std::stof(line.substr(0,commaIndex));
        float EndTime = std::stof(line.substr(commaIndex + 1));
        vector<float> floats = {StartTime, EndTime};
        return floats;
    }
    //This function creates JointAnomalyReports from anomalyDetectResults
    //It checks if the anomalyDetectResults has been joint one time, if it does the it doesn't join again.
    void CreateJointResults() {
        if (!hasBeenJoint) {
        for (int i = 0; i < (*anomalyDetectResults).size(); i++) {
            this->Rdescription = (*anomalyDetectResults)[i].description;
            this->StartTimeStep = (*anomalyDetectResults)[i].timeStep;
            while (Rdescription == (*anomalyDetectResults)[i].description) {
                i++;
                if (i == (*anomalyDetectResults).size())
                    break;
            }
            i--;
            this->EndTimeStep = (*anomalyDetectResults)[i].timeStep;
            JointAnomalyReport x(Rdescription, StartTimeStep, EndTimeStep);
            jointAnomalyDetectResults.push_back(x);
            hasBeenJoint = true;
        }
    }
    }

    //This function rounds a float number until 3 digits after the decimal dot.
    float roundFloat(float x) {
        x = ((float) ((int) (x * 1000))) / 1000;
        return x;
    }
};

//This class inherits from Command class. It represents the "exit" command.
class exitCLI:public Command {
public:
    exitCLI(DefaultIO *dio) : Command(dio) {
        this->description="6.exit\n";
    };
    virtual void execute() {}
    virtual string getDescription(){return description;}

};


#endif /* COMMANDS_H_ */
