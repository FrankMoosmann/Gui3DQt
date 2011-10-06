/*!
    \file   VisualizerCamControl.hpp
    \brief  A visualizer module for storage/recall from viewing positions, including automated interpolated flight through these 
    \author Frank Moosmann
    \date   2010
 
    Copyright: Karlsruhe Institute of Technology (KIT)
               Institute of Measurement and Control Systems
               All rights reserved
               http://www.mrt.kit.edu
*/
#ifndef GUI3DQT_VISUALIZERCAMCONTROL_HPP_
#define GUI3DQT_VISUALIZERCAMCONTROL_HPP_

#include <list>
#include <vector>
#include <GL/glut.h>
#include <QTimer>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "Visualizer.hpp"
#include "MNavWidget.hpp"
#include "ui_VisualizerCamControl.h"

namespace Gui3DQt {
  
class VisualizerCamControl : public Visualizer
{
  Q_OBJECT

public:
  VisualizerCamControl(MNavWidget &mNav, QWidget *parent = 0);
  virtual ~VisualizerCamControl();

  virtual void paintGLOpaque() {};
  virtual void paintGLTranslucent() {};

private:
//  struct CamPos {
//  	double pan;
//  	double tilt;
//  	double range;
//  	double x_offset;
//  	double y_offset;
//  	double z_offset;
//  	CamPos(double pan_, double tilt_, double range_, double x, double y, double z)
//			: pan(pan_), tilt(tilt_), range(range_), x_offset(x), y_offset(y), z_offset(z) {};
//  };
  typedef boost::numeric::ublas::vector<double> CamPos;
  CamPos camPos(double pan_, double tilt_, double range_, double x, double y, double z) {
  	CamPos c(6); c[0]=pan_; c[1]=tilt_; c[2]=range_; c[3]=x; c[4]=y; c[5]=z;
  	return c;
  };

  Ui::VisualizerCamControlClass ui;
  MNavWidget &mMav;
  std::vector<CamPos> camPositions;
  std::vector<CamPos> trajectory;
  unsigned int trajectoryIdx;
  QTimer *pilot;
  QString lastFilePath;

  void generateLinearTrajectory(unsigned int startIdx, unsigned int endIdx, unsigned int trajSize);
  void generateSplineTrajectory(unsigned int startIdx, unsigned int endIdx, unsigned int trajSize);
  void updateSlider(); // update slider in case camPositions buffer changed
  unsigned int getSliderIdx() {return ui.hsStorage->value()-1;}; // get current index on camPositions buffer from slider
  void setSliderIdx(unsigned int idx) {ui.hsStorage->setValue(idx+1);}; // set slider position based on index on camPositions

private slots:
//public slots:
  void update3D();

  void saveCamBuff();
  void loadCamBuff();
  void addCamPos();
  void replaceCamPos();
  void remCamPos();
  void clearCamPos();
  void shiftLeft();
  void shiftRight();
  void camPosSelect(int val = 0);
  void setStartPos();
  void setEndPos();
  void fly(bool down);

  void flyToNextPos();

};

} // namespace

#endif // GUI3DQT_VISUALIZERCAMCONTROL_HPP_
