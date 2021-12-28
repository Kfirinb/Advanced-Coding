//205812290    209013168
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_
#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
#include "anomaly_detection_util.h"
using namespace std;

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};

/*The following two functions are used to find the equation of the circle with three given points.
the method is to get a circle defined by 3 points */
Point get_circle_center(const Point& A, const Point& B,const Point& C);

/* Function that return a circle from three points */
Circle circle_from(const Point& A, const Point& B,const Point& C);

/* Function to return the smallest circle from 2 points by setting
 the center of the circle to be the midpoint of A and B, and the radius to be half the distance AB */
Circle circle_from(const Point& A, const Point& B);

/* Function to check whether a circle encloses *all* the given points, if not - return false */
bool is_valid_circle(const Circle& c, const vector<Point>& points_on_boundary);

/* Function to return the minimum enclosing circle for size of points_on_boundary <= 3, and we check as well
if MEC can be determined by 2 points only */
Circle min_circle_enclosing(vector<Point>& points_on_boundary);

/*This function returns the MEC(Minimum Enclosing Circle) with the Welzl's algorithm.
 The function gets a set of points and a set of points on the boundary's circle.
 "size" is the number of points left to cover.
 First the function checks if all points are covered or size of points_on_boundary is 3
 if so then we call the function min_circle_enclosing,if not,
 we take a random point from points and then swap it with the end point of points
 as a way to delete the random point from the points array.
 Then we check if the random point is in the current minimal circle, if not
 then we push it to the points_on_boundary vector.
*/
Circle welzlAlgorithm(Point** points,vector<Point> points_on_boundary, size_t size);

//This function finds the minimal radius circle for given points using the Welzl algorithm
Circle findMinCircle(Point** points, size_t size);

#endif /* MINCIRCLE_H_ */