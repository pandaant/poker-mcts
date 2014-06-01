/*
 * File:   double_operations.h
 * Author: batman
 *
 * Created on Freb 19, 2014, 2:43 PM
 */
// TODO can be deleted i guess

#ifndef DOUBLE_OPERATIONS_H
#define DOUBLE_OPERATIONS_H

#include <cmath>
#include <limits>

inline bool double_cmp_equal( double a, double b, double eps ){
    return std::fabs(a - b) < eps;
}

// a > b
inline bool double_greater( double a, double b, double eps ){
    return !double_cmp_equal(a,b,eps) && (a > b);
}

// a < b
inline bool double_lesser( double a, double b, double eps ){
    return !double_cmp_equal(a,b,eps) && (b > a);
}

#endif /* DOUBLE_OPERATIONS_H */

