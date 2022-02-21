
/*
 * anomaly_detection_util.cpp
 *
 * Author: Aviv Ben Yaakov
 * ID: 206261695
 */

#include <cmath>

#include "anomaly_detection_util.h"

// returns sum of an array elements
float sum(const float* x, int size) {
    float sum = 0;
    for (int i = 0; i < size; ++i) {
        sum+=x[i];
    }
    return sum;
}

// returns the square sum of an array elements
float sumPow(float* x, int size) {
    float sum = 0;
    for (int i = 0; i < size; ++i) {
        sum+=powf(x[i],2);
    }
    return sum;
}

// returns the average of an array elements
float avg(float* x, int size) {
    return sum(x, size) / size;
}

// returns the variance of X and Y
float var(float* x, int size){
    return ((sumPow(x, size) / size)) - powf(avg(x, size), 2);
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    float avgX = avg(x, size);
    float avgY = avg(y, size);
    float sum = 0;
    for (int i = 0; i < size; ++i) {
        sum+=(x[i]-avgX)*(y[i]-avgY);
    }
    return sum / size;

}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){

    float c = cov(x, y, size);
    float devX = sqrtf(var(x, size));
    float devY = sqrtf(var(y, size));
    return c / (devX * devY);
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
    float x[size];
    float y[size];
    for (int i = 0; i < size ; ++i) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    float a = cov(x, y, size) / var(x, size);
    float b = avg(y, size) - a * avg(x, size);
    Line line(a,b);
    return line;
}

Line linear_reg(float* x, float* y , int size) {
    float a = cov(x, y, size) / var(x, size);
    float b = avg(y, size) - a * avg(x, size);
    Line line(a,b);
    return line;
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
    Line l = linear_reg(points, size);
    return dev(p,l);

}

// returns the deviation between point p and the line
float dev(Point p,Line l){
    return fabsf(l.f(p.x) - p.y);
}