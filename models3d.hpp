#ifndef MODELS3D_H
#define MODELS3D_H

#include <GL/gl.h>
#include <GL/glu.h>

GLint generate_passat(double r, double g, double b); // implemented in model3dpassat.cpp
GLint generate_passatwagon(double passat_r, double passat_g, double passat_b); // implemented in model3dpassatwagon.cpp
GLint generate_tire(void); // implemented in model3dtire.cpp
GLint generate_velodyne(void); // implemented in model3dvelodyne.cpp

#endif
