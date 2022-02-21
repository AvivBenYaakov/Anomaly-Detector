

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>


struct correlatedFeatures {
    correlatedFeatures(Circle circle);

    string feature1, feature2;  // names of the correlated features
    float corrlation{};
    Line lin_reg;
    float threshold{};
    Circle circle;
};


class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
//    vector<AnomalyReport> ar;
    vector<correlatedFeatures> cf;
protected:
    float threshold = 0.9;
    vector<long> timeStepForPairs;
    vector<pair<string,long>> report;
    vector<pair<long,long>> anomalyRange;
public:

    SimpleAnomalyDetector();

    ~SimpleAnomalyDetector() override;

    /**
     * learn about the normal correlation and store the data inside vector.
     * @param ts TimeSeries.
     */
    virtual void learnNormal(const TimeSeries &ts);

    /**
     * detect anomalies.
     * @param ts TimeSeries.
     * @return vector of anomalies.
     */
    virtual vector<AnomalyReport> detect(const TimeSeries &ts);

    virtual vector<correlatedFeatures> getNormalModel() {
        return cf;
    }

    /**
     * erase duplicates in the correlation vector.
     */
    void eraseDuplicates();

    /**
     * helper method for finding the anomalies.
     * @param regLine Line regression line.
     * @param threshold float.
     * @param ts TimeSeries.
     * @param feature1 string.
     * @param feature2 string.
     * @param size int number of values.
     */
    virtual void detectAnomaly(const correlatedFeatures &corr, int size, const TimeSeries &_ts);

    virtual void setThreshold(const TimeSeries &_ts, correlatedFeatures *corr);

    virtual void setThreshold(float threshold);

    virtual float getThreshold();

    virtual vector<pair<string, long>> getAnomalyReports();

    virtual vector<long> getPairTimeStep();

    virtual vector<pair<long,long>> getReportsOfEachPair();



};


#endif /* SIMPLEANOMALYDETECTOR_H_ */
