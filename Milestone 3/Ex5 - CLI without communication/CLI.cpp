#include "CLI.h"
//Constructor of CLI class, getting DefaultIO and sets it to CLI's dio data member
CLI::CLI(DefaultIO* dio) {
    setDIO(dio);
}
//This function creates the Command Line Interface. First it creates an array of commands and then iterating
//the menu(the CLI) for the user and executes each command chosen by the user time after time.
void CLI::start(){
    Command* upload = new uploadTS(dio);
    Command* algo = new algoSettings(dio,&ad);
    Command* detect = new detectAnomalies(dio,&ad,&anomalyDetectResults);
    Command* display = new displayResults(dio,&anomalyDetectResults);
    Command* uploadAnomalies = new UploadAnomaliesAnalyze(dio,&anomalyDetectResults);
    Command* exit = new exitCLI(dio);
    Command** commandArray[] = {&upload,&algo,&detect,&display,&uploadAnomalies,&exit};
    Command* ptr;
    while(input!=6.0) {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for (int i =0;i <(sizeof(commandArray)/sizeof(commandArray[0]));i++ ) {
            ptr = *commandArray[i];
            dio->write(ptr->getDescription());
        }
        dio->read(&input);
        ptr = *commandArray[int(input)-1];
        ptr->execute();
    }
}
//CLI destructor
CLI::~CLI() {}