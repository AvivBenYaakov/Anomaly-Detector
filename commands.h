#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <cstring>
#include <fstream>
#include <utility>
#include <vector>
#include "HybridAnomalyDetector.h"
#include <iomanip>

using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

//    virtual void read(float *f) = 0;

    virtual ~DefaultIO() = default;

    // you may add additional methods here
};





class commandsManagement {

public:
    TimeSeries train;
    TimeSeries test;
    HybridAnomalyDetector hybrid;

    commandsManagement() {
//        train = TimeSeries();
//        test = TimeSeries();
        hybrid = HybridAnomalyDetector();
    }

};


// you may edit this class
class Command {
protected:
    DefaultIO *dio;
    string description;
    commandsManagement *cm{};
public:
    explicit Command(DefaultIO *dio) : dio(dio) {}

    virtual void execute() = 0;

    virtual ~Command() = default;


    virtual string getDescription() {
        return this->description;
    }
};


// implement here your command classes
class UploadTimeSeries : public Command {

public:
    explicit UploadTimeSeries(DefaultIO *dio, commandsManagement *management) : Command(dio) {
        this->dio = dio;
        this->cm = management;
        this->description = "1.upload a time series csv file";

    }

    virtual void execute() {
        const char *train = "anomalyTrain.csv";
        const char *test = "anomalyTest.csv";
        dio->write("Please upload your local train CSV file.\n");
        uploadData(cm->train, train);
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        uploadData(cm->test, test);
        dio->write("Upload complete.\n");
        cm->hybrid.learnNormal(cm->train);
    }


    void uploadData(TimeSeries &ts, const char *fileName) {
        ofstream file;
        file.open(fileName);
        string line;
        while (true) {
            line = dio->read();
            if (line == "done") {
                break;
            }
            file<<line<< endl;
        }
        file.close();
        ts = TimeSeries(fileName);

    }

};

class Settings : public Command {

public:
    Settings(DefaultIO *dio, commandsManagement *management) : Command(dio) {
        this->dio = dio;
        this->description = "2.algorithm settings";
        this->cm = management;
    }


    virtual void execute() {
        float threshold;
        cm->hybrid.learnNormal(cm->train);
        dio->write("The current correlation threshold is ");
        dio->write(cm->hybrid.getThreshold());
        dio->write("\n");
        dio->write("Type a new threshold\n");
        threshold = stoi(dio->read());
        while (threshold < 0 || threshold > 1) {
            threshold = stoi(dio->read());
            dio->write("please choose a value between 0 and 1.\n");
        }
        cm->hybrid.setThreshold(threshold);

    }
};

class AnomalyDetection : public Command {

public:
    AnomalyDetection(DefaultIO *dio, commandsManagement *management) : Command(dio) {
        this->dio = dio;
        this->description = "3.detect anomalies";
        this->cm = management;
    }

    virtual void execute() {
        cm->hybrid.detect(cm->test);
        dio->write("anomaly detection complete.\n");
    }
};

class Results : public Command {

public:
    Results(DefaultIO *dio, commandsManagement *management) : Command(dio) {
        this->dio = dio;
        this->description = "4.display results";
        this->cm = management;
    }

    virtual void execute() {
        printAnomalyReport();
    }

    void printAnomalyReport() {
        vector<pair<string, long>> reports = cm->hybrid.getAnomalyReports();
        for (auto &report : reports) {
            dio->write(report.second);
            dio->write("\t");
            dio->write(report.first);
            dio->write("\n");

        }
        dio->write("Done.\n");
    }
};


class AnalyzeResults : public Command {
    int n = 0;
    int p = 0;
    int fp = 0;
    int tp = 0;


public:
    AnalyzeResults(DefaultIO *dio, commandsManagement *management) : Command(dio) {
        this->dio = dio;
        this->cm = management;
        this->description = "5.upload anomalies and analyze results";

    }


    void CheckForTruePositive(long first, long last) {
        vector<pair<string, long>> reports = cm->hybrid.getAnomalyReports();
        for (auto &report : reports) {
            if (first <= report.second && report.second <= last) {
                tp++;
                return;
            }
        }


    }

    void CheckForFalsePositive(const vector<pair<long, long>>& clientRange,
                               const vector<pair<long, long>> &detectorRange) {

        int count;
        for (const auto &detector: detectorRange) {
            count = 0;
            for (const auto &client: clientRange) {
                if (detector.second < client.first || client.second < detector.first) {
                    count++;
                }
            }
            if (count == clientRange.size()) {
                fp++;
            }
        }

    }


    void uploadAnomalies() {
        vector<pair<long, long>> detectorRange = cm->hybrid.getReportsOfEachPair();
        vector<pair<long, long>> clientRange;
        detectorRange.pop_back();
        n = cm->test.getNumOfValues();
        p = 0;
        fp = 0;
        tp = 0;
        string line, comma;
        int first, last;
        while (true) {
            line = dio->read();
            if (line == "done") {
                break;
            }
            first = stoi(line.substr(0, line.find(',')));
            last = stoi(line.substr(line.find(',') + 1));
            clientRange.emplace_back(first, last);
            p++;
            n = n - (last - first + 1);
            CheckForTruePositive(first, last);

        }
        CheckForFalsePositive(clientRange, detectorRange);

    }

    void report() {
        float tpRate = (float) tp / p;
        tpRate = floor(10 * tpRate) / 10;
        float fpRate = (float) fp / n;
        fpRate = floor(1000 * fpRate) / 1000;
        setprecision(1);
        dio->write("True Positive Rate: ");
        dio->write(tpRate);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write(fpRate);
        dio->write("\n");
    }

    virtual void execute() {
        dio->write("Please upload your local anomalies file.\n");
        dio->write("Upload complete.\n");
        uploadAnomalies();
        report();
    }
};

class Exit : public Command {
    UploadTimeSeries *uts;
    Settings *settings;
    AnomalyDetection *ad;
    Results *res;
    AnalyzeResults *analyzeResults;
public:
    Exit(DefaultIO *dio, commandsManagement *management, UploadTimeSeries *uts, Settings *settings,
         AnomalyDetection *ad, Results *res, AnalyzeResults *analyzeResults) : Command(dio) {
        this->cm = management;
        this->dio = dio;
        this->description = "6.exit";
        this->uts = uts;
        this->settings = settings;
        this->ad = ad;
        this->res = res;
        this->analyzeResults = analyzeResults;

    }

    virtual void execute() {
        delete uts;
        delete settings;
        delete ad;
        delete res;
        delete analyzeResults;
        delete cm;

    }

};

#endif /* COMMANDS_H_ */