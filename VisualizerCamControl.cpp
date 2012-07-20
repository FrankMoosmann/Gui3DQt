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
#include "VisualizerCamControl.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <boost/foreach.hpp>
#include <QFileDialog>

#include "spline.hpp"
#include "ui_VisualizerCamControl.h"

using namespace std;
using namespace magnet::math;

namespace Gui3DQt {
  
VisualizerCamControl::VisualizerCamControl(MNavWidget &mNav_, QWidget *parent)
  : Visualizer(parent)
  , mMav(mNav_)
{
  pilot = new QTimer(this);
  pilot->setSingleShot(false);
  connect(pilot, SIGNAL(timeout()), this, SLOT(flyToNextPos()) );

  ui = new Ui::VisualizerCamControlClass();
  ui->setupUi(this);
  ui->hsStorage->setMinimum(1);
  updateSlider();
  setSliderIdx(0);

  connect(ui->pbSave, SIGNAL(pressed()), this, SLOT(saveCamBuff()) );
  connect(ui->pbLoad, SIGNAL(pressed()), this, SLOT(loadCamBuff()) );
  connect(ui->pbAdd, SIGNAL(pressed()), this, SLOT(addCamPos()) );
  connect(ui->pbReplace, SIGNAL(pressed()), this, SLOT(replaceCamPos()) );
  connect(ui->pbGoto, SIGNAL(pressed()), this, SLOT(camPosSelect()) );
  connect(ui->pbClear, SIGNAL(pressed()), this, SLOT(clearCamPos()) );
  connect(ui->pbLeft, SIGNAL(pressed()), this, SLOT(shiftLeft()) );
  connect(ui->pbRight, SIGNAL(pressed()), this, SLOT(shiftRight()) );
  connect(ui->pbRemove, SIGNAL(pressed()), this, SLOT(remCamPos()) );
  connect(ui->hsStorage, SIGNAL(valueChanged(int)), this, SLOT(camPosSelect(int)) );

  connect(ui->pbSetStart, SIGNAL(pressed()), this, SLOT(setStartPos()) );
  connect(ui->pbSetStop, SIGNAL(pressed()), this, SLOT(setEndPos()) );
  connect(ui->pbFly, SIGNAL(toggled(bool)), this, SLOT(fly(bool)) );
}


VisualizerCamControl::~VisualizerCamControl()
{
  pilot->stop();
  //delete pilot; don't delete as parent will delete it automatically
  delete ui;
}

void VisualizerCamControl::updateSlider()
{
  unsigned int cnt = camPositions.size();
  ui->hsStorage->setMaximum(max(1u,cnt));
  ui->hsStorage->setEnabled(camPositions.size() > 0);
}

unsigned int VisualizerCamControl::getSliderIdx()
{
  return ui->hsStorage->value()-1;
}

void VisualizerCamControl::setSliderIdx(unsigned int idx)
{
  ui->hsStorage->setValue(idx+1); 
}

void VisualizerCamControl::update3D()
{
  emit stateChanged();
}

void VisualizerCamControl::saveCamBuff()
{
  QString file = QFileDialog::getSaveFileName(this, "Save camera buffer ...", lastFilePath, "Camera buffer file (*.cam)");
  if (file.isEmpty()) return;
  lastFilePath = file.section('/', 0, -2);
  cout << endl <<"saving cambuffer to " << file.toStdString() << endl;
  ofstream out(file.toStdString().c_str());
  BOOST_FOREACH(const CamPos &pose, camPositions) {
    out << pose[0] << " " << pose[1] << " " << pose[2] << " "
        << pose[3] << " " << pose[4] << " " << pose[5] << endl;
  }
  cout << "done" << flush;
  update3D();
}

void VisualizerCamControl::loadCamBuff(string filename)
{
  ifstream in(filename.c_str());
  if (!in.good())
    throw runtime_error("VisualizerCamControl::loadCamBuff: problem opening the file for reading");
  camPositions.clear();
  string line;
  CamPos p(6);
  while (getline(in, line)) { // read one line into string
    istringstream istr;   // streaming the string containing a line
    try {
      istr.str(line);
      istr >> p[0];
      istr >> p[1];
      istr >> p[2];
      istr >> p[3];
      istr >> p[4];
      istr >> p[5];
      camPositions.push_back(p);
    } catch (exception&) {
      break;
    }
  }
  updateSlider();
  setSliderIdx(0);
  cout << "done" << flush;
  update3D();
}

void VisualizerCamControl::loadCamBuff()
{
  QString file = QFileDialog::getOpenFileName(this, "Load camera buffer", lastFilePath, "Camera buffer file (*.cam)");
  if (file.isEmpty()) return;
  lastFilePath = file.section('/', 0, -2);
  cout << endl << "loading cambuffer from " << file.toStdString() << "..." << flush;
  loadCamBuff(file.toStdString());
}

void VisualizerCamControl::addCamPos()
{
  unsigned int idx = getSliderIdx(); // --> camPositions[idx]
  double pan, tilt, range, x_offset, y_offset, z_offset;
  mMav.getCameraPos(pan, tilt, range, x_offset, y_offset, z_offset);
  if (camPositions.empty())
    camPositions.push_back(camPos(pan, tilt, range, x_offset, y_offset, z_offset));
  else
    camPositions.insert(camPositions.begin()+idx+1, camPos(pan, tilt, range, x_offset, y_offset, z_offset));
  updateSlider();
//  if (getSliderIdx() == (int)camPositions.size()-1) // last pose was selected -> increment
  //setSliderIdx(camPositions.size()-1);
  setSliderIdx(idx+1);
}

void VisualizerCamControl::replaceCamPos()
{
  if (camPositions.empty()) {
    addCamPos();
    return;
  }
  unsigned int idx = getSliderIdx(); // --> camPositions[idx]
  double pan, tilt, range, x_offset, y_offset, z_offset;
  mMav.getCameraPos(pan, tilt, range, x_offset, y_offset, z_offset);
  camPositions[idx] = camPos(pan, tilt, range, x_offset, y_offset, z_offset);
}

void VisualizerCamControl::remCamPos()
{
  if (camPositions.empty())
    return;
  camPositions.erase(camPositions.begin()+getSliderIdx());
  updateSlider();
}

void VisualizerCamControl::clearCamPos()
{
  camPositions.clear();
  updateSlider();
}

void VisualizerCamControl::shiftLeft()
{
  if (camPositions.empty())
    return;
  unsigned int idx = getSliderIdx(); // --> camPositions[idx]
  if (idx == 0) // already first
    return;
  CamPos tmp = camPositions[idx];
  camPositions[idx] = camPositions[idx-1];
  camPositions[idx-1] = tmp;
  setSliderIdx(idx-1);
}

void VisualizerCamControl::shiftRight()
{
  if (camPositions.empty())
    return;
  unsigned int idx = getSliderIdx(); // --> camPositions[idx]
  if (idx == camPositions.size()-1) // already last
    return;
  CamPos tmp = camPositions[idx];
  camPositions[idx] = camPositions[idx+1];
  camPositions[idx+1] = tmp;
  setSliderIdx(idx+1);
}

void VisualizerCamControl::camPosSelect(int val)
{
  if (camPositions.empty())
    return;
  (void)val;
  unsigned int idx = getSliderIdx(); // val-1
  CamPos &campos = camPositions[idx];
  mMav.setCameraPos(campos[0], campos[1], campos[2], campos[3], campos[4], campos[5]);
  // apply selected camera pose to current visualization
  update3D();
}

void VisualizerCamControl::setStartPos()
{
  ui->sbStart->setValue(ui->hsStorage->value());
}

void VisualizerCamControl::setEndPos()
{
  ui->sbStop->setValue(ui->hsStorage->value());
}

void VisualizerCamControl::fly(bool down)
{
  if (down) {
    trajectory.clear();
    double stepTimeMS = (double)(ui->sbStepTime->value());
    double totalTimeMS = (double)(ui->dsbDuration->value())*1000.0;
    unsigned int trajSize = totalTimeMS/stepTimeMS;
    unsigned int startIdx = 0;
    if (ui->rbStartCurrent->isChecked())  startIdx = ui->hsStorage->value();
    if (ui->rbStartFirst->isChecked())  startIdx = 1;
    if (ui->rbStartLast->isChecked()) startIdx = ui->hsStorage->maximum();
    if (ui->rbStartSpecific->isChecked()) startIdx = ui->sbStart->value();
    unsigned int endIdx = 0;
    if (ui->rbStopCurrent->isChecked())  endIdx = ui->hsStorage->value();
    if (ui->rbStopFirst->isChecked())  endIdx = 1;
    if (ui->rbStopLast->isChecked()) endIdx = ui->hsStorage->maximum();
    if (ui->rbStopSpecific->isChecked()) endIdx = ui->sbStop->value();
    // generate trajectory dependent on selected method
    if (ui->rbInterpolLinear->isChecked())
      generateLinearTrajectory(startIdx-1, endIdx-1, trajSize);
    if (ui->rbInterpolSpline->isChecked())
      generateSplineTrajectory(startIdx-1, endIdx-1, trajSize);
    //cout << endl << "generated trajectory with " << trajectory.size() << " elements, " << trajSize << " requested" << flush;
    // start pilot
    trajectoryIdx = 0;
    pilot->start(stepTimeMS); // even works when trajectory is empty
    cout << endl << "pilot takes off" << flush;
  } else {
    pilot->stop();
    cout << endl << "pilot landed" << flush;
  }
}

void VisualizerCamControl::flyToNextPos()
{
  if (trajectoryIdx >= trajectory.size()) {
    ui->pbFly->setChecked(false); // this should deactivate timer
  } else {
    // apply current trajectory position
    CamPos &campos = trajectory[trajectoryIdx];
    //cout << endl << campos << flush;
    mMav.setCameraPos(campos[0], campos[1], campos[2], campos[3], campos[4], campos[5]);
    // apply selected camera pose to current visualization
    update3D();
    // increment index
    ++trajectoryIdx;
  }
}

void VisualizerCamControl::generateLinearTrajectory(unsigned int startIdx, unsigned int endIdx, unsigned int nbTarget)
{
  unsigned int nbGiven = endIdx-startIdx+1;
  unsigned int nbIntervals = nbGiven - 1;
  unsigned int nbSubintervalsPerInterval = (nbTarget-1)/(nbIntervals); // -1 due to additional end pose
  // ^^ priority: specified poses are exactly hit -> might result in less poses than nbTarget
  trajectory.clear();
  for (unsigned int i=startIdx; i<endIdx; ++i) {
    CamPos &cPos = camPositions[i];
    CamPos &nPos = camPositions[i+1];
    CamPos step = (nPos-cPos)/(double)nbSubintervalsPerInterval;
    for (unsigned int j=0; j<nbSubintervalsPerInterval; ++j) {
      trajectory.push_back(cPos + (double)j*step);
    }
  }
  trajectory.push_back(camPositions[endIdx]); // push back last pose
}


double at(vector<Spline> &splines, unsigned int d, double s) {
  double v = splines[d](s);
  // limit values, because splines cannot be easily limited directly
  if (d == 1) // tilt must be <90°
    v = min(89.9, v);
  if (d == 2) // range must not be negative
    v = max(0.0, v);
  return v;
};
void VisualizerCamControl::generateSplineTrajectory(unsigned int startIdx, unsigned int endIdx, unsigned int nbTarget)
{
  const unsigned int nbSubintervalsPerInterval = 10;
  const double offset = ui->dsbFixedWeight->value();
  double weights[] = {1.0, 1.0, 0.7, 3.0, 3.0, 3.0}; //pan-tilt-range-x-y-z
  for (int i=3; i<6; ++i) {weights[i] = ui->dsbTransWeight->value();};
  unsigned int nbGiven = endIdx-startIdx+1;
  unsigned int nbIntervals = nbTarget-1;
  unsigned int nbSubIntervals = nbSubintervalsPerInterval*nbIntervals;
  double subStep = (double)(nbGiven-1)/(double)nbSubIntervals;

//  cout << endl << "given index " << startIdx << ".." << endIdx << " -> " << nbGiven << " support points and " << nbIntervals << " target intervals";
//  cout << endl << "stepping with " << subStep << " from 0.0 up to " << subStep*nbSubIntervals;
//  cout << flush;

  // there's no use in interpolating with splines for a low number of targets
  if (nbTarget < 2*nbGiven) {
    cout << endl << "WARNING: using linear interpolation due to low number of support points" << flush;
    generateLinearTrajectory(startIdx, endIdx, nbTarget);
    return;
  }

  // 1) create a spline for each dimension
  vector<Spline> splines;
  for (unsigned int d=0; d<6; ++d) {
    splines.push_back(Spline());
    for (unsigned int idx=startIdx; idx<=endIdx; ++idx/*,++s*/) {
      CamPos &cPos = camPositions[idx];
      splines[d].addPoint(idx-startIdx,cPos[d]);
    }
    //splines[d].setLowBC(Spline::FIXED_2ND_DERIV_BC, 0);
    //splines[d].setHighBC(Spline::FIXED_2ND_DERIV_BC, 0);
    splines[d].setLowBC(Spline::FIXED_1ST_DERIV_BC, 0);
    splines[d].setHighBC(Spline::FIXED_1ST_DERIV_BC, 0);
  }


  // 2) create a sampled "maximum-delta-spline" from the single splines and calculate its integral
  vector<double> maxDelta(nbSubIntervals);
  double integral = 0.0;
  //cout << endl << "integrating over " << nbSubIntervals << " sections" << flush;
  for (unsigned int i=0; i<nbSubIntervals; ++i) {
    double s = (double)i*subStep;
    double sn = (double)(i+1)*subStep;
    double wr = 1.0/sqrt(sqrt(at(splines, 2, s)+at(splines, 2, sn))); // range-weight-multiplier
    maxDelta[i] = 0.0;
//    cout << endl << s << "\t";
    for (unsigned int d=0; d<6; ++d) {
      double v = at(splines, d, s);
      double vn = at(splines, d, sn);
      double delta = weights[d]*wr*fabs(v-vn);
//      cout << delta << "\t";
      maxDelta[i] = max(maxDelta[i], delta);
    }
//    cout << "\t--> " << maxDelta[i] << flush;
    maxDelta[i] += offset;
    integral += maxDelta[i];
  }
  //cout << endl << "integral=" << integral << flush;

  // 3) now subsample trajectory according to equal integral chunks
  trajectory.clear();
  trajectory.push_back(camPositions[startIdx]); // push back last pose
  double intStep = integral / (double)nbIntervals;
  //cout << endl << "intStep=" << intStep << flush;
  double accumulator = 0.0;
  for (unsigned int i=0; i<nbSubIntervals; ++i) {
    accumulator += maxDelta[i];
    if (accumulator > intStep) {
      double s = (double)i*subStep;
      //cout << endl << "sampling at " << s << flush;
      trajectory.push_back(camPos(at(splines, 0, s), at(splines, 1, s), at(splines, 2, s),
          at(splines, 3, s), at(splines, 4, s), at(splines, 5, s)));
      accumulator -= intStep;
    }
  }
  trajectory.push_back(camPositions[endIdx]); // push back last pose
}

} // namespace
