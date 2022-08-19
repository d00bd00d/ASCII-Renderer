#include "triangle.h" 

Triangle::Triangle(Vector v1, Vector v2, Vector v3) {
    _vertices[0] = v1; 
    _vertices[1] = v2; 
    _vertices[2] = v3; 

    _legs[0] = v2 - v1; 
    _legs[1] = v3 - v1; 

    _normal = _legs[0].cross(_legs[1]).unit(); 
}

Triangle::~Triangle() {} 

bool Triangle::contains(Vector p) { 
    // This assumes that the provided vector p is coplanar
    // to the triangle itself. 

    // assuming this, we may express the vector 
    // p - v1 = t * (v2 - v1) + s (v3 - v1) 
    // for 0 <= t + s <= 1. 
    p = p - _vertices[0]; 
        
    // Now we have p = t * u1 + s * u2. Then, we have: 
    // p = t * u1 + s * (u2 - u2.proj(u1) + u2.proj(u1))
    // If w = u2 - u2.proj(u1), then we get 
    // p.dot(w) = s * w.dot(w) 
    // allowing us to recover s. Repeat with t. 
    Vector w1 = _legs[0] - _legs[0].proj(_legs[1]); 
    float t = p.dot(w1); 
    if(t < 0) return false; 

    Vector w2 = _legs[1] - _legs[1].proj(_legs[0]); 
    float s = p.dot(w2); 
    if(s < 0) return false; 

    return s / w2.dot(w2)+ t / w1.dot(w1) <= 1; 
} 

float Triangle::intersect(Vector b, Vector m) {
    // gets the intersection of the line l(t) = b + mt with 
    // the plane containing this triangle by solving for t. 
    // returns infinity if no solution exists. 
    // The plane through the triangle is 
    // _normal.dot(_vertices[0] - x) = 0
    // Substitute x = l(t), distribute the dot, and solve. 
    return _normal.dot(_vertices[0] - b) / _normal.dot(m); 
} 

float Triangle::angle(Vector p) {
    // computes the cosine of the angle that the given vector 
    // makes with the plane of the triangle. This is given
    // by cos(angle) = _normal.dot(p.unit()); 
    return _normal.dot(p.unit()); 
} 

void Triangle::rotate(Rotater r, float theta) {
    for(int i = 0; i < 3; i++) 
        _vertices[i] = r.rotate(_vertices[i], theta); 

    _legs[0] = _vertices[1] - _vertices[0]; 
    _legs[1] = _vertices[2] - _vertices[0]; 
    _normal = _legs[0].cross(_legs[1]).unit(); 
} 

Vector* Triangle::getVertices() {
    // WATCH OUT FOR MEMORY LEAK
    // returing _vertices may allow other classes to modify 
    // the triangle accidentally, and v would get deleted 
    // after returning if it's initialised on the stack. 
    Vector* v = new Vector[3]; 
    for(int i = 0; i < 3; i++)
        v[i] = _vertices[i]; 

    return v; 
} 

// for debugging 
std::ostream& operator<<(std::ostream& out, Triangle& t) {
    for(int i = 0; i < 3; i++) 
        out << t._vertices[i]; 

    out << '\n'; 
    return out; 
}
