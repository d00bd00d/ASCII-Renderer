#ifndef CANVAS_H
#define CANVAS_H 

#include <ncurses.h>
#include <vector> 

#include "triangle.h" 

class Canvas {
public: 
    Canvas(int, int, WINDOW*, const char*); 
    ~Canvas(); 

    void redraw(); 

protected: 
private: 
    // helper struct to store info about each pixel
    // on the screen 
    struct Pixel {
        float dist = 0; 
        float angle = 0; 
    }; 

    Pixel** _screen; 
    int _width, _height; 
    WINDOW* _win; 

    std::vector<Triangle> _objects; 

    // vectors that establish viewing angles, the viewing 
    // plane, etc.; rendering constants. 
    Vector _camera; 
    Vector _camera_angle; 
    float _camera_distance = 80; 

    Vector _origin; 
    float _pixel_size = 1; 

    Vector _light; 

    // helper methods to render triangles on the screen.
    static void _updateRow(Canvas*, int); 
    void _updateTriangle(Triangle); 
    static void _updateTriangles(Canvas*, int, int); 

    // conversions to and from the absolute coordinates of
    // the ambient space and the local coordinates of the 
    // screen itself. 
    Vector _pixelToVector(int, int); 
    int _getVectorX(Vector); 
    int _getVectorY(Vector); 
}; 

#endif 
