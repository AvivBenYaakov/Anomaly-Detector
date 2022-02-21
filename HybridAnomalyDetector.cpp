
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() = default;

HybridAnomalyDetector::~HybridAnomalyDetector() = default;

void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
    SimpleAnomalyDetector::learnNormal(ts);
}

void HybridAnomalyDetector::setThreshold(const TimeSeries &ts, correlatedFeatures *corr) {
    const TimeSeries& _ts = ts;
    if (abs(corr->corrlation) >= 0.9) {
        SimpleAnomalyDetector::setThreshold(_ts, corr);
        return;
    }

    corr->circle = findMinCirc(_ts, *corr);
}

Circle HybridAnomalyDetector::findMinCirc(TimeSeries ts, correlatedFeatures c) {
    int size = ts.getNumOfValues();
    auto* p = new Point*[size];
    for (int i = 0; i < size; i++) {
        p[i] = new Point(ts.getCurrentVal(c.feature1, i),ts.getCurrentVal(c.feature2, i));
    }

    Circle circle = findMinCircle(p, size);
    circle.radius = 1.1*circle.radius;
    return circle;

}



vector<correlatedFeatures> HybridAnomalyDetector::getNormalModel() {
    return SimpleAnomalyDetector::getNormalModel();
}

void HybridAnomalyDetector::detectAnomaly(const correlatedFeatures &corr, int size, const TimeSeries &_ts) {
    TimeSeries ts = _ts;
    string feature1 = corr.feature1;
    string feature2 = corr.feature2;
    for (int i = 0; i < size; i++) {
        float x = ts.getCurrentVal(feature1, i);
        float y = ts.getCurrentVal(feature2, i);
        Point p(x, y);
        float dist = sqrt(pow((p.x - corr.circle.center.x), 2) + pow((p.y - corr.circle.center.y), 2));
        if (dist > corr.circle.radius) {
            timeStepForPairs.emplace_back(i + 1);
            report.emplace_back(feature1 + "-" += feature2, i+1);
        }
    }
}

void HybridAnomalyDetector::setThreshold(float threshold) {
    SimpleAnomalyDetector::setThreshold(threshold);
}

vector<long> HybridAnomalyDetector::getPairTimeStep() {
    return SimpleAnomalyDetector::getPairTimeStep();
}

vector<pair<string, long>> HybridAnomalyDetector::getAnomalyReports() {
    return SimpleAnomalyDetector::getAnomalyReports();
}







