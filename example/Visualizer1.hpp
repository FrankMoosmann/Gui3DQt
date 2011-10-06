/*!
    \file   Visualizer1.hpp
    \brief  A simple visualizer module
    \author Frank Moosmann
    \date   2011
 
    Copyright: Karlsruhe Institute of Technology (KIT)
               Institute of Measurement and Control Systems
               All rights reserved
               http://www.mrt.kit.edu
*/
#ifndef VISUALIZER1_HPP
#define VISUALIZER1_HPP

#include <QtGui/QWidget>
#include <Gui3DQt/Visualizer.hpp>
#include "ui_Visualizer1.h"

class Visualizer1 : public Gui3DQt::Visualizer
{
  Q_OBJECT

public:
  Visualizer1(QWidget *parent = 0);
  virtual ~Visualizer1();

  virtual void paintGLOpaque();
  virtual void paintGLTranslucent();

private:
  Ui::Visualizer1Class ui;
  double circ_radius1;
  double circ_radius2;
  
private slots:
  void update();
  void rand_radius();
};

#endif // VISUALIZER1_HPP
