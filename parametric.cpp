/** 
 *  A side program that helps generate polygonal meshes for
 *  parametric surfaces, i.e. embeddings of subsets of R2 
 *  into R3. As long as the embedding is smooth, triangles
 *  will get mapped to triangles, more or less, and making 
 *  the resolution fine enough gives a pretty good mesh.
 */ 

#include <fstream> 
#include <math.h>
#include "vector.h"

#define OUTPUT_FILE "torus.vert"
#define PI 3.1415926535

#define U_MIN 0
#define U_MAX 2 * PI + 0.1
#define V_MIN 0 
#define V_MAX 2 * PI + 0.1

#define STEP 0.15

// x, y, and z as functions of u and v. 
#define R1 15
#define R2 6.5

#define X(u, v) (R1 + R2 * cos(u)) * cos(v)
#define Y(u, v) (R1 + R2 * cos(u)) * sin(v)
#define Z(u, v) R1 * sin(u)

const Vector centre(0, 0, 13); 

int main() {
    using namespace std; 
    ofstream out(OUTPUT_FILE); 

    // cut up the u-v domain into squares of length STEP.
    // The image of each square produces 2 triangles. 
    Vector ul, ur, ll, lr; 
    for(float u = U_MIN; u <= U_MAX; u += STEP) {
        for(float v = V_MIN; v <= V_MAX; v += STEP) {
            ll = Vector(X(u, v), Y(u, v), Z(u, v)); 
            ul = Vector(X(u, v + STEP), Y(u, v + STEP), Z(u, v + STEP)); 
            lr = Vector(X(u + STEP, v), Y(u + STEP, v), Z(u + STEP, v)); 
            ur = Vector(X(u + STEP, v + STEP), Y(u + STEP, v + STEP), Z(u + STEP, v + STEP)); 

            ll = ll + centre; 
            ul = ul + centre; 
            lr = lr + centre; 
            ur = ur + centre; 

            out << ll << ul << lr << '\n'; 
            out << ul << lr << ur << '\n'; 
        } 
    } 

    out.close(); 
    return 0; 
} 
