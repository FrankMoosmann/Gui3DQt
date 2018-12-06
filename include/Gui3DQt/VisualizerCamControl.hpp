/*!
 *  Gui3DQt - a lightweight, modular Gui framework for displaying 3D content
 *  https://github.com/FrankMoosmann/Gui3DQt.git
 * 
 *  \file   VisualizerCamControl.hpp
 *  \brief  A visualizer module for storage/recall from viewing positions, including automated interpolated flight through these 
 *  \author Frank Moosmann
 *  \date   2010
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

namespace Ui { class VisualizerCamControlClass; } // forward declaration to avoid including the ui_ header

namespace Gui3DQt {

class VisualizerCamControl : public Visualizer
{
  Q_OBJECT

public:
  VisualizerCamControl(MNavWidget &mNav, QWidget *parent = 0);
  virtual ~VisualizerCamControl();

  virtual void paintGLOpaque() {};
  virtual void paintGLTranslucent() {};

  void loadCamBuff(std::string filename);
  
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

  Ui::VisualizerCamControlClass *ui;
  MNavWidget &mMav;
  std::vector<CamPos> camPositions;
  std::vector<CamPos> trajectory;
  unsigned int trajectoryIdx;
  QTimer *pilot;
  QString lastFilePath;

  void generateLinearTrajectory(unsigned int startIdx, unsigned int endIdx, unsigned int trajSize);
  void generateSplineTrajectory(unsigned int startIdx, unsigned int endIdx, unsigned int trajSize);
  void updateSlider(); // update slider in case camPositions buffer changed
  unsigned int getSliderIdx(); // get current index on camPositions buffer from slider
  void setSliderIdx(unsigned int idx); // set slider position based on index on camPositions

public slots:
  void camPosSelect(int val = 0);

private slots:
  void update3D();

  void saveCamBuff();
  void loadCamBuff();
  void addCamPos();
  void replaceCamPos();
  void remCamPos();
  void clearCamPos();
  void shiftLeft();
  void shiftRight();
  void setStartPos();
  void setEndPos();
  void fly(bool down);

  void flyToNextPos();

};

} // namespace

#endif // GUI3DQT_VISUALIZERCAMCONTROL_HPP_
