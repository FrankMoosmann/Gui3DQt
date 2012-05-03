/*!
 *  Gui3DQt - a lightweight, modular Gui framework for displaying 3D content
 *  https://github.com/FrankMoosmann/Gui3DQt.git
 * 
 *  \file   Visualizer.h
 *  \brief  Provides a base class for viszalization
 *  \author Frank Moosmann (<moosmann@mrt.uka.de>),
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
#ifndef GUI3DQT_VISUALIZER_HPP_
#define GUI3DQT_VISUALIZER_HPP_

#include <QtGui/QWidget>
#include <QColor>
#include <GL/glut.h>

namespace Gui3DQt {
  
/*!
 * \class Visualizer
 * \brief This is the base class for any customized visualization module for Gui3DMainWindow/Gui3DQt
 * 
 * Deriving from Visualizer allows to register the class in Gui3DQt as a visualizing module.
 * This automatically draws the visualizer's front end withing the Gui and calls the paintGL
 * method each time the GLWidget repaints its contens.
 * 
 * To create a custom visualizer within Eclipse:
 * 		Choose File -> New -> Other -> Qt Gui Class, choose QWidget as base class
 * 		Then include "Visualizer.h" in the header file of your new class and change the base class to "Visualizer"
 * 		Usw Qt Designer to create your front end, and implement your own paintGL function.
 * As an example see VisualizerGrid.h/.cpp
 * 
 * ATTENTION: If you derive from Visualizer, there might be exceptions if you pass 2 pointers as parameter in the constructor!!!
 * ATTENTION: If your visualizer accesses data structures from other threads, don't forget to synchronize with mutex variables!  
 */
class Visualizer : public QWidget
{
  Q_OBJECT
public:
  Visualizer(QWidget *parent = 0) : QWidget(parent) {};
	virtual ~Visualizer() {};
	
	virtual void paintGLOpaque() = 0; //!< will be called on rendering. use OpenGL calls here. can be used to draw opaque objects
  virtual void paintGLTranslucent() = 0; //!< will be called on rendering. use OpenGL calls here. can be used to draw transparent objects
	
  inline void HSV2RGB(int H, int S, int V, int &r, int &g, int &b) { // H = 0..360; S,V=0..255
          QColor c = QColor::fromHsv(H, S, V);
          c.getRgb(&r, &g, &b);
  }

  inline void RGB2HSV(int R, int G, int B, int &h, int &s, int &v) { // R/G/B = 0..255
          QColor c(R, G, B);
          c.getHsv(&h, &s, &v);
  }

  inline void glColorHSV(int H, int S, int V) { // H = 0..360; S,V=0..255
          int r,g,b; HSV2RGB(H, S, V, r,g,b);
          glColor3f(r/255.0f,g/255.0f,b/255.0f); // color according to "speed"
  }

  inline void glColorHSV(int H, int S, int V, float alpha) { // H = 0..360; S,V=0..255,alpha=0..1
          int r,g,b; HSV2RGB(H, S, V, r,g,b);
          glColor4f(r/255.0f,g/255.0f,b/255.0f,alpha); // color according to "speed"
  }

signals:
	void stateChanged(); //!< emit this signal when state of the visualizer changes and thus a GL redraw is necessary (in return the above paintGL methods will be called)
  void redraw2D(QImage&); //!< emit this signal to redraw the 2D image
};

} // namespace

#endif // GUI3DQT_VISUALIZER_HPP_
