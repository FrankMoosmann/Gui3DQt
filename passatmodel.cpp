/*
 *  Gui3DQt - a lightweight, modular Gui framework for displaying 3D content
 *  https://github.com/FrankMoosmann/Gui3DQt.git
 * 
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
#include "passatmodel.hpp"

#include <cmath>
#include <stdexcept>
#include "models3d.hpp"

static GLint *passatwagon = NULL; // pointer to array of size [PASSAT_MODEL_COUNT]
static GLint tire = 0;
static GLint velodyne = 0;

using namespace std;

// helper function for angle conversion
double radians_to_degrees(double rad) {
  return rad/M_PI*180.;
}

// helper function to check model index
void checkFailIndexRange(unsigned int model_index) {
  if (model_index >= Gui3DQt::PassatModel::PASSAT_MODEL_COUNT)
    throw invalid_argument("passat model index invalid");
}

// helper function to init/verify static variables, colors are only used if model was not already loaded
void assertPassatModel(unsigned int model_index, double r, double g, double b) {
  if (passatwagon == NULL) { // init storage
    passatwagon = new GLint[Gui3DQt::PassatModel::PASSAT_MODEL_COUNT];
    for (unsigned int i=0; i<Gui3DQt::PassatModel::PASSAT_MODEL_COUNT; ++i) 
      passatwagon[model_index] = 0;
  }
  if (!passatwagon[model_index])
    passatwagon[model_index] = generate_passatwagon(r, g, b);
  if (!tire)
    tire = generate_tire();
  if (!velodyne)
    velodyne = generate_velodyne();
}


void Gui3DQt::PassatModel::setColor(unsigned int model_index, double r, double g, double b) {
  checkFailIndexRange(model_index);
  // if model exists, delete it
  if (passatwagon != NULL) { // memory initialized
    if (passatwagon[model_index] != 0) { // specified model was already loaded
      glDeleteLists(passatwagon[model_index], 1);
      passatwagon[model_index] = 0;
    }
  }
  // generate model
  assertPassatModel(model_index, r, g, b);
}


void Gui3DQt::PassatModel::draw(unsigned int model_index, double wheel_angle, double velodyne_angle)
{
  checkFailIndexRange(model_index);
  assertPassatModel(model_index, 0.5, 0.5, 0.5);
  GLboolean depthtestState, lightingState;
  glGetBooleanv(GL_DEPTH_TEST, &depthtestState);
  glGetBooleanv(GL_LIGHTING, &lightingState);
  if (!depthtestState) glEnable(GL_DEPTH_TEST);
  if (!lightingState) glEnable(GL_LIGHTING);
  /* draw the license plates */
  glPushMatrix();
  glScalef(5.0, 5.0, 5.0);
  glRotatef(180,0,0,1);
  glCallList(passatwagon[model_index]);
  glPopMatrix();
  glScalef(0.65, 0.65, 0.65);
  // Front Right Tire
  glPushMatrix();
  glTranslatef(2.27, -1.17, -0.86);
  glRotatef( radians_to_degrees(wheel_angle), 0, 1, 0 );
  glCallList(tire);
  glPopMatrix();
  // Front Left Tire
  glPushMatrix();
  glTranslatef(2.27, 1.17, -0.86);
  glRotatef(180, 0, 0, 1);
  glRotatef( -radians_to_degrees(wheel_angle), 0, 1, 0 );
  glCallList(tire);
  glPopMatrix();
  // Rear Right Tire
  glPushMatrix();
  glTranslatef(-2.05, -1.17, -0.86);
  glRotatef( radians_to_degrees(wheel_angle), 0, 1, 0 );
  glCallList(tire);
  glPopMatrix();
  // Rear Left Tire
  glPushMatrix();
  glTranslatef(-2.05, 1.17, -0.86);
  glRotatef(180, 0, 0, 1);
  glRotatef( -radians_to_degrees(wheel_angle), 0, 1, 0 );
  glCallList(tire);
  glPopMatrix();
  glScalef(0.35, 0.35, 0.35);
  // Velodyne laser
  glPushMatrix();
  glTranslatef(-0.34, 0, 3.6);
  glRotatef(radians_to_degrees(velodyne_angle) + 180, 0, 0, 1);
  glCallList(velodyne);
  glPopMatrix();
  if (!depthtestState) glDisable(GL_DEPTH_TEST);
  if (!lightingState) glDisable(GL_LIGHTING);
}
