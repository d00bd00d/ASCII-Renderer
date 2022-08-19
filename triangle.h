#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h" 
#include "rotater.h" 
#include <iostream> 

class Triangle {
public:
    Triangle(Vector, Vector, Vector); 
    ~Triangle(); 

    bool contains(Vector); 
    float intersect(Vector, Vector); 
    float angle(Vector); 
    void rotate(Rotater, float); 

    Vector* getVertices(); 

    friend std::ostream& operator<<(std::ostream&, Triangle&); 

protected: 
private: 
    Vector _vertices[3]; 
    Vector _legs[2]; 
    Vector _normal; 
}; 

#endif 
