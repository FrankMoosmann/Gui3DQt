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
#include "VisualizerGrid.hpp"
#include "graphics.hpp"

#include <cmath>

namespace Gui3DQt {
  
VisualizerGrid::VisualizerGrid(QWidget *parent)
    : Visualizer(parent), x(0), y(0), z(0), yawRad(0)
{
}

VisualizerGrid::VisualizerGrid(double x_, double y_, double z_, double yawRad_, QWidget *parent)
    : Visualizer(parent), x(x_), y(y_), z(z_), yawRad(yawRad_)
{
}

VisualizerGrid::~VisualizerGrid()
{

}


void VisualizerGrid::paintGLOpaque()
{
  glPushMatrix();
  glTranslatef(x, y, z);
  glRotatef(yawRad/M_PI*180,0,0,1); // angle(DEG), x, y, z (rotation axis)
  Graphics::draw_grid(0,0);
  Graphics::draw_coordinate_frame(1.0);
  glPopMatrix();
}

void VisualizerGrid::paintGLTranslucent()
{
}

void VisualizerGrid::update()
{
  emit stateChanged();
}

void VisualizerGrid::setOrigin(double x_, double y_, double z_, double yawRad_) {
  x = x_;
  y = y_;
  z = z_;
  yawRad = yawRad_;
  emit stateChanged();
}

} // namespace
