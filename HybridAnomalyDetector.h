

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
#include "anomaly_detection_util.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {

public:

    HybridAnomalyDetector();
    virtual ~HybridAnomalyDetector();
    virtual void learnNormal(const TimeSeries &ts);
    virtual vector<correlatedFeatures> getNormalModel();
    virtual void setThreshold(const TimeSeries &_ts, correlatedFeatures *corr);
    virtual Circle findMinCirc(TimeSeries ts, correlatedFeatures c);
    virtual void detectAnomaly(const correlatedFeatures &corr, int size, const TimeSeries &_ts);
    virtual vector<pair<string, long>> getAnomalyReports();
    virtual void setThreshold(float threshold);
    virtual vector<long> getPairTimeStep();



};

#endif /* HYBRIDANOMALYDETECTOR_H_ */

