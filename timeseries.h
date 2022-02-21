#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <cstring>
#include <regex>
#include <list>
#include "anomaly_detection_util.h"


using namespace std;

class TimeSeries{
private:

    string fileName1;
    std::vector<std::pair<std::string, std::vector<float>>> content;
    std::vector<string> features;

public:

    vector<pair<string,string>> correlatedFeatures;
    vector<float> correlations;
    vector<Line> lineRegressions;

    /**
     * constructor.
     * @param CSVfileName const char* file name.
     */
    explicit TimeSeries(const char* CSVfileName);


    TimeSeries();

    void addLine(const string& s);

    void readFirstLine(const string& s);


    /**
     * reading the csv file and parsing it into 2d vector.
     * @param inputFileName string inputFile.
     */
    void read_file(const string &inputFileName);

    /**
     * destructor.
     */
    ~TimeSeries();

    /**
     * method to find all the correlated features, and save the data of the correlation.
     */
    void findCorrelation();

    /**
     *
     * @param feature.
     * @param index of the current value.
     * @return the value of the given index of the input feature.
     */
    float getCurrentVal(const string& feature, int index);

    /**
     *
     * @param feature
     * @return the values of the given feature.
     */
    const float * getValues(const string& feature);
    /**
     *
     * @return the number of value.
     */
    int getNumOfValues();

    /**
     *
     * @return the number of features.
     */
    int getNumOfFeatures();

    /**
     *
     * @param x array of floats
     * @param y array of floats.
     * @param regLine regression line.
     * @return tha maximum dev the with the given line.
     */
    float getMaxDev(float* x, float* y, Line regLine);

    /**
     *
     * @return list of the features.
     */
    vector<string> getFeaturesList();

};




#endif /* TIMESERIES_H_ */