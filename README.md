### ASCII Renderer 

A curses-based 3D renderer. It's a very original project idea that I used to hone my development skills and refresh my linear algebra knowledge. It also looks very cool. 

## How it works 

The scene is drawn from the perspective of a camera, and the screen is a plane located some distance "in front" of the camera. Each triangle is then radially projected onto the screen, provided it's on the right side of the screen, and the screen is updated. 

Currently, the renderer only draws triangles, which are supplied to the renderer via `.vert` files. `parametric.cpp` allows you to triangulate parametrically defined surfaces; it was used to produce `torus.vert` and `sphere.vert`. In the `.vert` file itself, each line specifies a triangle: the nine numbers describe the x, y, and z-coordinates of the three vertices. 

There is currently a bit of head-scratching to do with the coordinate system. On one hand, data are stored and computations are performed using an arbitrarily set absolute coordinate system for the entire space. However, the screen itself has its own local coordinate system, with the additional pain of being only two-dimensional. This coordinate system is also arbitrary, and this makes it hard to reconcile the scale, location, and orientation of each triangle being drawn. Suggestions (read: assistance) are very welcome. 

## Next steps 

Currently, the project is missing a lot of functions that I would like to add at one point or another, including (but of course not limited to): 

 - Camera movement controls, as well as controlling the field of view. 
 - Providing a way of specifying movement. Currently, everything just rotates about some arbitrary axis in the "middle" of the scene; it would be nice to have different groups of triangles move independently of each other. 
 - Drawing point clouds instead of just triangles. This is so that the renderer can draw dynamical systems that can be approximated or simulated by a collection of points, such as fluids or magnetic filings in wacky environments. However, this requires some topological background that I don't have yet. 
 - Fixing the Makefile's dependencies. They're super scuffed at the moment. 
 - Implementing marching cubes algorithm to generate `.vert` files for implicitly defined surfaces instead of parametric surfaces. 
