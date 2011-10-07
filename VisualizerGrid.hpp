/*!
    \file   VisualizerGrid.hpp
    \brief  A simple visualizer module drawing a grid on the ground plane (z=0)
    \author Frank Moosmann
    \date   2009
 
    Copyright: Karlsruhe Institute of Technology (KIT)
               Institute of Measurement and Control Systems
               All rights reserved
               http://www.mrt.kit.edu
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
  virtual ~VisualizerGrid();

  virtual void paintGLOpaque();
  virtual void paintGLTranslucent();

private slots:
  void update();
};

} //namespace

#endif // GUI3DQT_VISUALIZERGRID_HPP_
