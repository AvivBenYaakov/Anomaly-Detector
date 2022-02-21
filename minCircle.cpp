//
// Created by avivx on 03/01/2021.
//

#include "minCircle.h"
#include <vector>
#include <cmath>

/**
 * create circle from two points.
 * @param p point.
 * @param q point.
 * @return circle.
 */
Circle createCircle(Point p, Point q) {

    // middle point of the circle
    Point mid((p.x + q.x) / 2, (p.y + q.y) / 2);

    // distance between the p and q
    float dist = sqrt(pow((p.x - q.x), 2) + pow((p.y - q.y), 2));

    float r = dist / 2;

    Circle c(mid, r);

    return c;

}

/**
 * create circle from three points.
 * @param p1 point.
 * @param p2 point.
 * @param p3 point.
 * @return circle.
 */
Circle createCircle(Point p1, Point p2, Point p3) {
    float p1x_Pow = pow(p1.x, 2);
    float p2x_Pow = pow(p2.x, 2);
    float p3x_Pow = pow(p3.x, 2);
    float p1y_Pow = pow(p1.y, 2);
    float p2y_Pow = pow(p2.y, 2);
    float p3y_Pow = pow(p3.y, 2);
    float sum_pow_p1 = p1x_Pow + p1y_Pow;
    float sum_pow_p2 = p2x_Pow + p2y_Pow;
    float sum_pow_p3 = p3x_Pow + p3y_Pow;
    float x_numerator = sum_pow_p1 * (p2.y - p3.y) + sum_pow_p2 * (p3.y - p1.y)
                        + sum_pow_p3 * (p1.y - p2.y);

    float y_numerator = sum_pow_p1 * (p3.x - p2.x) + sum_pow_p2 * (p1.x - p3.x)
                        + sum_pow_p3 * (p2.x - p1.x);

    float denominator = 2 * (p1.x * (p2.y - p3.y) - p1.y * (p2.x - p3.x)
                             + p2.x * p3.y - p3.x * p2.y);

    float x = x_numerator / denominator;
    float y = y_numerator / denominator;
    float r = sqrt(pow((x - p1.x), 2) + pow((y - p1.y), 2));
    return {Point(x, y), r};
}


/**
 * check if given all the points is inside the circle.
 * @param c
 * @param points
 * @return true if all the points inside, false otherwise.
 */
bool isValidCircle(Circle c, const std::vector<Point> &points) {
    float dist;
    for (const auto &p: points) {
        dist = sqrt(pow((p.x - c.center.x), 2) + pow((p.y - c.center.y), 2));
        if (dist > c.radius) {
            return false;
        }
    }
    return true;
}

/**
 *
 * @param s Points vector.
 * @return circle.
 */
Circle basicMinCircle(const std::vector<Point> &s) {
    int size = s.size();
    switch (size) {
        case 0:
            return {Point(0.0, 0.0), 0.0};
        case 1:
            return {s[0], 0.0};
        case 2:
            return createCircle(s[0], s[0]);
        default:
            for (int i = 0; i < s.size(); i++) {
                for (int j = 0; j < s.size(); j++) {
                    if (i == j) {
                        continue;
                    }
                    Circle c = createCircle(s[i], s[j]);
                    if (isValidCircle(c, s)) {
                        return c;
                    }
                }
            }

    }
    return createCircle(s[0], s[1], s[2]);
}


/**
 *
 * @param points.
 * @param s vector<Point> s.
 * @param size int size_t
 * @return the minimum circle.
 */
Circle getMinCircle(Point **points, std::vector<Point> s, size_t size) {
    if (size == 0 || s.size() == 3) {
        return basicMinCircle(s);
    }
    Point p(points[size - 1]->x, points[size - 1]->y);
    size--;
    Circle myCircle = getMinCircle(points, s, size);
    float dist = sqrt(pow((p.x - myCircle.center.x), 2) + pow((p.y - myCircle.center.y), 2));
    if (dist <= myCircle.radius) {
        return myCircle;
    }
    s.emplace_back(p);
    return getMinCircle(points, s, size);
}


/**
 *
 * @param points.
 * @param size int size.
 * @return minimum circle.
 */
Circle findMinCircle(Point **points, size_t size) {
    std::vector<Point> s;
    Circle circle = getMinCircle(points, s, size);
    return circle;
}