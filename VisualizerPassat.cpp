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
#include "VisualizerPassat.hpp"

#include <iostream>
#include <cmath>

#include "passatmodel.hpp"
#include "ui_VisualizerPassat.h"

using namespace std;

namespace Gui3DQt {

const double VisualizerPassat::translate_to_velobase_x = PassatModel::translate_to_velobase_x;
const double VisualizerPassat::translate_to_velobase_y = PassatModel::translate_to_velobase_y;
const double VisualizerPassat::translate_to_velobase_z = PassatModel::translate_to_velobase_z;
const double VisualizerPassat::translate_to_rearaxle_x = PassatModel::translate_to_rearaxle_x;
const double VisualizerPassat::translate_to_rearaxle_y = PassatModel::translate_to_rearaxle_y;
const double VisualizerPassat::translate_to_rearaxle_z = PassatModel::translate_to_rearaxle_z;
const double VisualizerPassat::translate_to_groundcenter_x = PassatModel::translate_to_groundcenter_x;
const double VisualizerPassat::translate_to_groundcenter_y = PassatModel::translate_to_groundcenter_y;
const double VisualizerPassat::translate_to_groundcenter_z = PassatModel::translate_to_groundcenter_z;
  
VisualizerPassat::VisualizerPassat(double r, double g, double b, unsigned int modelindex_, bool rotateVelodyne, QWidget *parent)
	: Visualizer(parent)
  , veloTurner(this)
  , modelindex(modelindex_)
  , veloAngleIncDEG(-45) // turns clockwise, i.e. in mathematically negative direction
  , veloAngleUpMSec(100)
  , x(0)
  , y(0)
  , z(0)
  , yawRAD(0)
  , veloAngleDEG(0)
  , wheelAngleDEG(0)
{
  PassatModel::setColor(modelindex, r, g, b);
  
  veloTurner.setSingleShot(false);
  connect(&veloTurner, SIGNAL(timeout()), this, SLOT(timerAction()) );

  ui = new Ui::VisualizerPassatClass();
  ui->setupUi(this);
  ui->cbVeloTurn->setChecked(rotateVelodyne);
  connect(ui->cbVeloTurn, SIGNAL(toggled(bool)), this, SLOT(activateTimer(bool)) );
  
  activateTimer(rotateVelodyne);
}


VisualizerPassat::~VisualizerPassat()
{
  veloTurner.stop();
  delete ui;
}

void VisualizerPassat::paintGLOpaque()
{
  glPushMatrix();
  glTranslatef(x, y, z);
  glRotatef(yawRAD/M_PI*180,0,0,1); // angle(DEG), x, y, z (rotation axis)
  PassatModel::draw(modelindex, wheelAngleDEG/180*M_PI, veloAngleDEG/180*M_PI);
  glPopMatrix();
}

void VisualizerPassat::paintGLTranslucent()
{
}

void VisualizerPassat::setPose(double newx, double newy, double newz, double newyawRAD)
{
  x = newx;
  y = newy;
  z = newz;
  yawRAD = newyawRAD;
  emit stateChanged();
}

void VisualizerPassat::timerAction()
{
  veloAngleDEG += veloAngleIncDEG;
  if (veloAngleDEG < 0) veloAngleDEG += 360;
  if (veloAngleDEG >= 360) veloAngleDEG -= 360;
  emit stateChanged();
}

void VisualizerPassat::activateTimer(bool active)
{
  if (active)
    veloTurner.start(veloAngleUpMSec);
  else
    veloTurner.stop();
}

} // namespace
