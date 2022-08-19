#include "canvas.h" 
#include <math.h> 
#include <fstream> 
#include <iostream> 
#include <thread> 

namespace CanvasUtils {
    int strLen(const char* s) {
        int len = -1; 
        while(s[++len] != '\0') {} 
        return len; 
    } 

    char getShade(float angle) {
        // helper function to get the character needed to shade
        // a cell based on the cosine of the viewing angle
        if(angle < 0) angle *= -1; 
        angle = std::min(angle, 0.9999f); 
        angle *= angle; 

        static const char* shades = ".',\";+=o#W"; 
        static int length = strLen(shades); 

        return shades[(int)(angle * length)];
    } 
}

Canvas::Canvas(int width, int height, WINDOW* win, 
        const char* file) : 
    _width(width), _height(height), _win(win) {

    _screen = new Pixel*[_height]; 
    for(int y = 0; y < _height; y++) {
        _screen[y] = new Pixel[_width]; 
    } 

    // initialise rendering constants 
    _camera = Vector(0, 0, -100); 
    _camera_angle = Vector(0, 0, 1).unit(); 
    _origin = _camera + _camera_angle.scale(_camera_distance); 
    _light = Vector(1, 1, 4); 

    // read objects in from file
    std::ifstream in(file); 

    if(!in.is_open()) {
        endwin(); 
        printf("There was an error opening the file %s\n", file); 
        exit(-1); 
    } 

    Vector v1, v2, v3; 
    while(!in.eof()) {
        in >> v1 >> v2 >> v3; 
        _objects.push_back(Triangle(v1, v2, v3)); 
        in.ignore(100, '\n'); 
    } 

    in.close(); 
} 

Canvas::~Canvas() {
    for(int y = 0; y < _height; y++) 
        delete[] _screen[y]; 

    delete[] _screen; 
} 

void Canvas::redraw() {
    wmove(_win, 0, 0); 

    // update distances and angles 
    Vector o(0, 0, 13); 
    Vector n(1, 1, 0); 
    Rotater r(n, o); 

    for(unsigned int i = 0; i < _objects.size(); i++) 
        _objects[i].rotate(r, 0.04); 

    // threadification time
    for(int y = 0; y < _height; y++) 
        for(int x = 0; x < _width; x++) 
            _screen[y][x].dist = 0; 

    int numThreads = 25; 
    std::thread* threads[numThreads];
    for(int i = 0; i < numThreads; i++) 
        threads[i] = new std::thread(_updateTriangles, 
                                   this, i, numThreads); 

    for(int i = 0; i < numThreads; i++) {
        threads[i]->join(); 
        delete threads[i]; 
    }
    
    // draw everything on the screen 
    for(int y = 0; y < _height; y++) {
        for(int x = 0; x < _width; x++) {
            Pixel p = _screen[_height - y - 1][x]; 

            if(p.dist == 0) 
                waddch(_win, ' '); 
            else 
                waddch(_win, CanvasUtils::getShade(p.angle)); 
        } 
    } 

    wrefresh(_win); 
    refresh(); 
} 

// helper function to update a triangle only 
void Canvas::_updateTriangle(Triangle t) {
    // to reduce computation (especially when the screen is
    // big), we'll project the vertices of the triangle onto
    // the viewing plane and create a bounding box. 
    Vector* v = t.getVertices(); 

    // for each vertex v[i], we know the line through v[i] is
    // given by: 
    // l(s) = _camera + s * (v[i] - _camera). 
    // Solving for _camera_angle.dot(l(t) - _origin) = 0
    // gives us that: 
    // s = _cam_dir.dot(_origin) / _cam_dir.dot(v[i] - _cam) 
    //      ^^ just _camera_distance
    int minX = _width, minY = _height; 
    int maxX = 0, maxY = 0; 

    for(int i = 0; i < 3; i++) {
        float s = _camera_distance; 
        s /= _camera_angle.dot(v[i] - _camera); 

        Vector p = _camera.scale(1 - s) + v[i].scale(s); 
        int x = _getVectorX(p); 
        int y = _getVectorY(p); 

        if(x < minX) minX = x; 
        if(x > maxX) maxX = x; 
        if(y < minY) minY = y; 
        if(y > maxY) maxY = y; 
    }

    // if the bounding box is outside the screen, tone it down
    // also give a 2-pixel leeway so the bounding boxes aren't 
    // just zero, which can occur due to trunctations and whatnot
    // for 1-by-1 bounding boxes.
    if(--minX < 0) minX = 0; 
    if(--minY < 0) minY = 0; 
    if(++maxX >= _width) maxX = _width - 1; 
    if(++maxY >= _height) maxY = _height - 1; 

    // avoid a memory leak :x 
    delete[] v; 

    // now update everything in the bounding box only. 
    for(int y = minY; y <= maxY; y++) {
        for(int x = minX; x <= maxX; x++) {
            Vector b = _pixelToVector(x, y); 
            Vector u = (b - _camera).unit(); 

            float d = t.intersect(b, u); 
            Vector p = b + u.scale(d); 

            if(isinf(d) || !t.contains(p) || d <= 0) 
                continue; 
            else if(_screen[y][x].dist == 0 || 
                    d < _screen[y][x].dist) {
                _screen[y][x].dist = d; 
                _screen[y][x].angle = t.angle(_light); 
            }
        }
    }
} 

// helper function to threadify triangle rendering 
void Canvas::_updateTriangles(Canvas* c, int start, int offset) {
    for(unsigned int i = start; i < c->_objects.size(); i += offset) 
        c->_updateTriangle(c->_objects[i]); 
} 

// helper function to update a row of pixels 
void Canvas::_updateRow(Canvas* c, int y) {
    for(int x = 0; x < c->_width; x++) {
        c->_screen[y][x].dist = 0; 
        for(Triangle t : c->_objects) {
            Vector b = c->_pixelToVector(x, y); 
            Vector u = (b - c->_camera).unit(); 

            float d = t.intersect(b, u); 
            Vector p = b + u.scale(d); 

            if(isinf(d) || !t.contains(p) || d <= 0) 
                continue; 
            else if(c->_screen[y][x].dist == 0 || 
                    d < c->_screen[y][x].dist) {
                c->_screen[y][x].dist = d; 
                c->_screen[y][x].angle = t.angle(c->_light); 
            } 
        }
    }
} 

// conversion functions 
inline Vector Canvas::_pixelToVector(int x, int y) {
    Vector offset; 
    offset.x = (float)(x - _width / 2) * _pixel_size; 
    offset.y = (float)(_height / 2 - y - 1) * _pixel_size; 

    return offset + _origin; 
} 

// assumes the provided vector is (roughly) coplanar to
// the screen; otherwise will not work. 
inline int Canvas::_getVectorX(Vector v) {
    return (v.x - _origin.x) / _pixel_size + _width / 2; 
}

inline int Canvas::_getVectorY(Vector v) {
    return _height / 2 - (v.y - _origin.y) / _pixel_size + 1; 
}
