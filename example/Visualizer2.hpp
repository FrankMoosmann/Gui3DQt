/*!
 *    \file   Visualizer2.hpp
 *    \brief  A simple visualizer module
 *    \author Frank Moosmann
 *    \date   2011
 * 
 *    Copyright: Karlsruhe Institute of Technology (KIT)
 *               Institute of Measurement and Control Systems
 *               All rights reserved
 *               http://www.mrt.kit.edu
 */
#ifndef VISUALIZER2_HPP
#define VISUALIZER2_HPP

#include <QtGui/QWidget>
#include <Gui3DQt/Visualizer.hpp>
#include "ui_Visualizer2.h"

class Visualizer2 : public Gui3DQt::Visualizer
{
  Q_OBJECT
  
public:
  Visualizer2(QWidget *parent = 0);
  virtual ~Visualizer2();
  
  virtual void paintGLOpaque();
  virtual void paintGLTranslucent();
  
private:
  Ui::Visualizer2Class ui;
  GLuint glListIndex; // holds OpenGL drawing lists
  
private slots:
  void regenerate();
};

#endif // VISUALIZER2_HPP
