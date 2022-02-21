#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------
//class Point{
//public:
//	float x,y;
//	Point(float x,float y):x(x),y(y){}
//};

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}

};
// --------------------------------------
Circle createCircle(Point p, Point q);

Circle createCircle(Point p1, Point p2, Point p3);

bool isValidCircle(Circle c, const vector<Point> &points);

Circle basicMinCircle(const std::vector<Point> &s);

Circle getMinCircle(Point **points, vector<Point> s, size_t size);

Circle findMinCircle(Point **points, size_t size);


#endif /* MINCIRCLE_H_ */