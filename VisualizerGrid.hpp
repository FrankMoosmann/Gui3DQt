/*!
 *  Gui3DQt - a lightweight, modular Gui framework for displaying 3D content
 *  https://github.com/FrankMoosmann/Gui3DQt.git
 * 
 *  \file   VisualizerGrid.hpp
 *  \brief  A simple visualizer module drawing a grid on the ground plane (z=0)
 *  \author Frank Moosmann
 *  \date   2009
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
#ifndef GUI3DQT_VISUALIZERGRID_HPP_
#define GUI3DQT_VISUALIZERGRID_HPP_

#include <QtGui/QWidget>
#include "Visualizer.hpp"

namespace Gui3DQt {
  
class VisualizerGrid : public Visualizer
{
  Q_OBJECT

public:
  VisualizerGrid(QWidget *parent = 0);
  VisualizerGrid(double x, double y, double z, double yawRad, QWidget *parent = 0);
  virtual ~VisualizerGrid();

  virtual void paintGLOpaque();
  virtual void paintGLTranslucent();

  void setOrigin(double x, double y, double z, double yawRad); // specify center

private:
  double x, y, z, yawRad; // position of center
private slots:
  void update();
};

} //namespace

#endif // GUI3DQT_VISUALIZERGRID_HPP_
