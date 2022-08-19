#ifndef ROTATER_H
#define ROTATER_H

#include <math.h> 

// having a struct like this heavily reduces the amount
// of redundant computations needed. Most importantly, 
// this class only needs to find the orthonormal basis
// once rather than finding a new basis every time! 
struct Rotater {
    Vector normal, centre; 

    // basis vectors 
    Vector v1, v2; 

    Rotater(Vector n, Vector c) {
        normal = n.unit(); 
        centre = c; 

        // let v1 be any vector that's not parallel to
        // the normal vector. 
        v1 = Vector(1, 0, 0); 
        if(normal.y == 0 && normal.z == 0) 
            v1 = Vector(0, 1, 0); 

        // perform Graham-Schmidt. 
        v1 = (v1 - v1.proj(normal)).unit(); 
        v2 = normal.cross(v1); 
    } 

    Vector rotate(Vector v, float theta) {
        // compute coordinates with respect to [n v1 v2]. 
        v = v - centre; 
        float b = v.dot(v1); 
        float c = v.dot(v2); 

        // rotate in the v1-v2 plane
        return centre + v.proj(normal) + 
               v1.scale(b * cos(theta) - c * sin(theta)) + 
               v2.scale(c * cos(theta) + b * sin(theta)); 
    } 
}; 

#endif 
