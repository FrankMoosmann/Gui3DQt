/*!
 *  Gui3DQt - a lightweight, modular Gui framework for displaying 3D content
 *  https://github.com/FrankMoosmann/Gui3DQt.git
 * 
 *  \file   VisualizerPassat.hpp
 *  \brief  A visualizer module for drawing the passat model
 *  \author Frank Moosmann
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
#ifndef GUI3DQT_VISUALIZERPASSAT_HPP_
#define GUI3DQT_VISUALIZERPASSAT_HPP_

#include <list>
#include <vector>
#include <GL/glut.h>
#include <QTimer>

#include "Visualizer.hpp"

namespace Ui { class VisualizerPassatClass; } // forward declaration to avoid including the ui_ header

namespace Gui3DQt {

class VisualizerPassat : public Visualizer
{
  Q_OBJECT

public:
  // if several VisualizerPassat are to be used or other calls to PassatModel::draw exist, choosing a different modelindex allows to have several cars with different color
  VisualizerPassat(double r = 0.5, double g = 0.5, double b = 0.5, unsigned int modelindex = 0, bool rotateVelodyne = true, QWidget *parent = 0);
  virtual ~VisualizerPassat();
  
  void setPose(double x, double y, double z, double yawRad); // set other pose

  static const double translate_to_velobase_x;
  static const double translate_to_velobase_y;
  static const double translate_to_velobase_z;
  static const double translate_to_rearaxle_x;
  static const double translate_to_rearaxle_y;
  static const double translate_to_rearaxle_z;
  static const double translate_to_groundcenter_x;
  static const double translate_to_groundcenter_y;
  static const double translate_to_groundcenter_z;
  
  virtual void paintGLOpaque();
  virtual void paintGLTranslucent();

private:
  Ui::VisualizerPassatClass *ui;
  QTimer veloTurner;
  const unsigned int modelindex;
  const double veloAngleIncDEG;
  const double veloAngleUpMSec;
  double x, y, z, yawRAD; // position of rear axle
  double veloAngleDEG;
  double wheelAngleDEG;
  
private slots:
  void timerAction();
  void activateTimer(bool);
};

} // namespace

#endif // GUI3DQT_VISUALIZERPASSAT_HPP_
