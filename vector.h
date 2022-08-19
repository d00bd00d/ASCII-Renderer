#ifndef VECTOR_H
#define VECTOR_H

#include <math.h> 
#include <iostream> 

struct Vector {
    float x, y, z; 

    Vector(float x = 0, float y = 0, float z = 0) : 
        x(x), y(y), z(z) {}

    Vector(const Vector& v) : 
        x(v.x), y(v.y), z(v.z) {} 

    Vector operator+(const Vector& v) {
        return Vector(x + v.x, y + v.y, z + v.z); 
    }

    Vector operator-(const Vector& v) {
        return Vector(x - v.x, y - v.y, z - v.z); 
    } 

    friend std::istream& operator>>(std::istream& in, Vector& v) {
        in >> v.x >> v.y >> v.z; 
        return in; 
    } 

    friend std::ostream& operator<<(std::ostream& out, Vector& v) {
        out << v.x << " " << v.y << " " << v.z << " "; 
        return out; 
    } 

    float length() {
        return sqrt(dot(*this)); 
    } 

    Vector scale(float k) {
        return Vector(x * k, y * k, z * k); 
    } 

    float dot(Vector v) {
        return x * v.x + y * v.y + z * v.z; 
    } 

    Vector cross(Vector v) {
        float a = y * v.z - z * v.y; 
        float b = z * v.x - x * v.z; 
        float c = x * v.y - y * v.x; 
        return Vector(a, b, c); 
    } 

    Vector proj(Vector v) {
        // projection of this vector onto v is: 
        // (this dot v) * v.unit() 
        float k = this->dot(v) / v.dot(v); 
        return v.scale(k); 
    } 

    Vector unit() {
        return this->scale(1.0f / length()); 
    } 
}; 

#endif 
