/*
 * anomaly_detection_util.cpp
 *
 * Author: Kfir Inbal
 */

#include <math.h>
#include "anomaly_detection_util.h"

//returns the average of set of numbers
float avg(float* x, int size) {
    float avg = 0;
    for (int i = 0; i < size; i++) {
        avg += x[i];
    }
    avg = avg /size;
    return avg;
}

// returns the variance of X and Y
float var(float* x, int size) {
    float powAvg = 0, muAvg = 0, Var, sqrdX[size];
    muAvg = avg(x,size);
    for (int i = 0 ; i < size; i++) {
        sqrdX[i] = x[i]*x[i];
    }
    powAvg = avg(sqrdX, size);
    Var = powAvg - muAvg*muAvg;
    return Var;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size) {
    float xAvg = 0, yAvg = 0, sum =0;
    xAvg = avg(x,size);
    yAvg = avg(y,size);
    for (int i = 0; i < size; i++) {
        sum += (x[i] - xAvg) * (y[i] - yAvg);
    }
    return sum/size;
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {
    float covXY, sigmaX, sigmaY, final;
    covXY = cov(x,y,size);
    sigmaX = sqrt(var(x,size));
    sigmaY = sqrt(var(y,size));
    final = covXY / (sigmaX*sigmaY);
    return final;
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size) {
    float a, b, x[size], y[size], xAvg = 0, yAvg = 0;
    Point* p;

    for (int i = 0; i < size; i++) {
        p = points[i];
        x[i] = p->x;
        y[i] = p->y;
    }
    xAvg = avg(x, size);
    yAvg = avg(y, size);

    a = cov(x,y,size)/var(x,size);

    b = yAvg - a*xAvg;

    return Line(a,b);

}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size) {
    Line l = linear_reg(points,size);
    return dev(p,l);
}

// returns the deviation between point p and the line
float dev(Point p,Line l) {
    float ans = l.f(p.x) - p.y;
    if (ans < 0)
        ans = ans * -1;
    return ans;
}




