/*
 * timeseries.cpp
 *
 * Author: Aviv Ben Yaakov
 * ID: 206261695
 */


#include "timeseries.h"
#include "AnomalyDetector.h"
#include <fstream>
#include <vector>
#include <regex>


TimeSeries::TimeSeries(const char *CSVfileName) {
    fileName1 = CSVfileName;
    read_file(fileName1);
}

TimeSeries::TimeSeries() = default;


void TimeSeries::read_file(const string &fileName) {

    std::ifstream myFile(fileName);
    std::string line, colmName;
    float value;

    // Read the column names, aka the names of the features.
    if (myFile) {
        std::getline(myFile, line);
        std::stringstream ss(line);

        while (std::getline(ss, colmName, ',')) {

            // Initialize and add <colmName, int vector> pairs to result
            content.push_back({colmName, vector<float>{}});
        }
    }

    // read the file.
    while (myFile && getline(myFile, line)) {
        std::stringstream ss(line);
        int columnIndex = 0;

        // Extract each float
        while (ss >> value) {
            content.at(columnIndex).second.push_back(value);

            // ignore commas.
            if (ss.peek() == ',') {
                ss.ignore();
            }

            // next column.
            columnIndex++;
        }
    }

    myFile.close();

}


void TimeSeries::findCorrelation() {
    int numOfProp = content.size();
    int numOfValues = content.at(0).second.size();


    for (int i = 0; i < numOfProp; i++) {

        float correlation = 0;

        for (int j = 0; j < numOfProp; j++) {

            // dont check feature with itself.
            if (i != j) {

                float *iValues = content.at(i).second.data();
                float *jValues = content.at(j).second.data();
                float currentCorrelation = pearson(iValues, jValues, numOfValues);

                // update the maximum correlation.
                if (abs(correlation) < abs(currentCorrelation)) {
                    correlation = currentCorrelation;

                    // for each features,only one maximum correlation exists.
                    if (correlatedFeatures.size() > i) {
                        correlatedFeatures.pop_back();
                        correlations.pop_back();
                        lineRegressions.pop_back();

                    }

                    // save the correlation data.
                    correlatedFeatures.emplace_back(content.at(i).first, content.at(j).first);
                    correlations.emplace_back(correlation);
                    lineRegressions.emplace_back(linear_reg(iValues, jValues, numOfValues));
                }

            }

        }


    }

}


TimeSeries::~TimeSeries() = default;


float TimeSeries::getCurrentVal(const string &feature, int time) {
    for (const auto &i: content) {
        if (i.first == feature) {
            return i.second.at(time);
        }
    }
    return 0;
}

const float *TimeSeries::getValues(const string &feature) {
    for (const auto &i: content) {
        if (i.first == feature) {
            return i.second.data();
        }
    }
    return nullptr;
}


int TimeSeries::getNumOfFeatures() {
    return content.size();
}

int TimeSeries::getNumOfValues() {
    return content.at(0).second.size();
}


float TimeSeries::getMaxDev(float *x, float *y, Line regLine) {
    int size = this->getNumOfValues();
    float maxDev = 0;
    for (int i = 0; i < size; i++) {
        maxDev = max(maxDev, dev(Point(x[i], y[i]), regLine));
    }

    return maxDev;

}

vector<string> TimeSeries::getFeaturesList() {
    for (const auto &i: content) {
        features.emplace_back(i.first);
    }
    return features;
}


void TimeSeries::addLine(const string& data) {
    float value;
    std::stringstream ss(data);
    int columnIndex = 0;

    // Extract each integer
    while (ss >> value) {
        content.at(columnIndex).second.push_back(value);

        // ignore commas.
        if (ss.peek() == ',') {
            ss.ignore();
        }

        // next column.
        columnIndex++;
    }
}

void TimeSeries::readFirstLine(const string& data) {
    std::string colmName;

    // Read the column names, aka the names of the features.
    std::stringstream ss(data);

    while (std::getline(ss, colmName, ',')) {

        // Initialize and add <colmName, int vector> pairs to result
        content.push_back({colmName, vector<float>{}});
    }

}