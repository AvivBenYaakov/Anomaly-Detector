/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Aviv Ben Yaakov
 * ID: 206261695
 */

#include "SimpleAnomalyDetector.h"



SimpleAnomalyDetector::SimpleAnomalyDetector() = default;

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;

struct correlatedFeatures corr(Circle(Point(0.0, 0.0), 0));

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    TimeSeries ts2 = ts;
    ts2.findCorrelation();
    int size = ts2.getNumOfFeatures();


    for (int i = 0; i < size; i++) {
        if (ts2.correlations.at(i) < 0.5 && ts2.correlations.at(i) > -0.5) {
            continue;
        }
        corr.feature1 = ts2.correlatedFeatures.at(i).first;
        corr.feature2 = ts2.correlatedFeatures.at(i).second;
        corr.corrlation = ts2.correlations.at(i);
        corr.lin_reg = ts2.lineRegressions.at(i);
        corr.circle.radius = std::numeric_limits<float>::infinity();
        corr.threshold = 0;

        setThreshold(ts2, &corr);

        cf.emplace_back(corr);
    }

    this->eraseDuplicates();

}

void SimpleAnomalyDetector::setThreshold(const TimeSeries &ts, correlatedFeatures *corr) {
    TimeSeries _ts = ts;
    auto *firstFeature = const_cast<float *>(_ts.getValues(corr->feature1));
    auto *secondFeature = const_cast<float *>(_ts.getValues(corr->feature2));
    corr->threshold = 1.1 * _ts.getMaxDev(firstFeature, secondFeature, corr->lin_reg);
}


void SimpleAnomalyDetector::eraseDuplicates() {
    for (int i = 0; i < cf.size(); i++) {
        for (int j = 0; j < cf.size(); j++) {
            if (cf.at(i).feature1 == cf.at(j).feature2 && cf.at(j).feature1 == cf.at(i).feature2) {
                cf.erase(cf.begin() + j);
            }
        }
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> ar;
    TimeSeries _ts = ts;
    int size = _ts.getNumOfValues();
    vector<string> features = _ts.getFeaturesList();
    for (auto &i: cf) {
        for (const auto &j: features) {

            // check for anomaly for each feature.
            if (j == i.feature1) {
                if (isinf(i.circle.radius)) {
                    SimpleAnomalyDetector::detectAnomaly(i, size, _ts);

                } else {
                    detectAnomaly(i, size, ts);
                }

            }
        }
    }
    return ar;
}

void ::SimpleAnomalyDetector::detectAnomaly(const correlatedFeatures &corr, int size, const TimeSeries &_ts) {
    TimeSeries ts = _ts;
    string feature1 = corr.feature1;
    string feature2 = corr.feature2;

    for (int i = 0; i < size; i++) {
        float x = ts.getCurrentVal(feature1, i);
        float y = ts.getCurrentVal(feature2, i);
        Point p(x, y);
        float devartion = dev(p, corr.lin_reg);

        // anomaly detected, add to AnomalyReport.
        if (devartion > corr.threshold) {
            timeStepForPairs.emplace_back(i + 1);
            report.emplace_back(feature1 + "-" += feature2, i + 1);
        }

    }

}

vector<pair<string, long>> SimpleAnomalyDetector::getAnomalyReports() {
    return report;
}

void SimpleAnomalyDetector::setThreshold(float threshold) {
    this->threshold = threshold;
}

float SimpleAnomalyDetector::getThreshold() {
    return threshold;
}

vector<long> SimpleAnomalyDetector::getPairTimeStep() {
    return timeStepForPairs;
}

vector<pair<long, long>> SimpleAnomalyDetector::getReportsOfEachPair() {
    // make sure vector is empty
    anomalyRange.clear();
    int size = report.size();
    // temp for storing current correlated features.
    string temp = report[0].first;
    anomalyRange.emplace_back(report[0].second, report[0].second);
    for (int i = 1; i < size; i++) {
        // storing current description.
        temp = report[i].first;

        // if we find new description in the next index
        if (temp != report[i+1].first) {
            // change timeStep of the end of current anomaly
            anomalyRange.back().second = report[i].second;
            i++;
            // add another anomaly range.
            anomalyRange.emplace_back(report[i].second, report[i].second);
        }
    }

    return anomalyRange;
}



correlatedFeatures::correlatedFeatures(Circle circle) : circle(circle) {

}
