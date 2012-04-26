/*!
 *  Gui3DQt - a lightweight, modular Gui framework for displaying 3D content
 *  https://github.com/FrankMoosmann/Gui3DQt.git
 * 
 *  \file   models3d.hpp
 *  \brief  Provides functions to generate 3d models
 *  \author Benjamin Pitzer
 *  \author Frank Moosmann (<moosmann@mrt.uka.de>),
 *  \date   2012
 *  \copyright  Karlsruhe Institute of Technology (KIT)
 *              Institute of Measurement and Control Systems
 *              http://www.mrt.kit.edu
 *              
 *              This program is free software: you can redistribute it and/or
 *              modify it under the terms of the GNU General Public License
 *              version 3 as published by the Free Software Foundation.
 *              Other licenses are available on demand.
 *
 *              This program is distributed in the hope that it will be useful,
 *              but WITHOUT ANY WARRANTY; without even the implied warranty of
 *              MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *              GNU General Public License for more details.
 *
 *              You should have received a copy of the GNU General Public License
 *              along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MODELS3D_H
#define MODELS3D_H

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

GLint generate_passat(double r, double g, double b); // implemented in model3dpassat.cpp
GLint generate_passatwagon(double passat_r, double passat_g, double passat_b); // implemented in model3dpassatwagon.cpp
GLint generate_tire(void); // implemented in model3dtire.cpp
GLint generate_velodyne(void); // implemented in model3dvelodyne.cpp

#endif
