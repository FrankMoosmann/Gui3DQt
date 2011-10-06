/*!
    \file   Gui.hpp
    \brief  Provides a wrapper class for easy setup and exec of Gui3DMainWindow
    \author Frank Moosmann (<moosmann@mrt.uka.de>),
    \date   2007-2009
 
    Copyright: Karlsruhe Institute of Technology (KIT)
               Institute of Measurement and Control Systems
               All rights reserved
               http://www.mrt.kit.edu
*/
#ifndef GUI3DQT_HPP_
#define GUI3DQT_HPP_

#include "MainWindow.hpp"
#include "Visualizer.hpp"

namespace Gui3DQt {
  
/*!
  \class Gui
  \brief Wrapper Class for easy setup and exec of Gui3DMainWindow
  
  This class allows an easy setup of Gui3DMainWindow and its execution as application.
  It accepts new Gui3DVisualizer-Widgets and registers them within Gui3DMainWindow
*/
class Gui
{
public:
  Gui(std::string title, int argc, char *argv[], MainWindow::GuiMode mode = MainWindow::Mode3D2D);
  virtual ~Gui();

  void registerVisualizer(Visualizer*, std::string title);
  MNavWidget* getQGlWidget();
  void exec();
  
private:
  QApplication        *app;
  MainWindow          *mainWin;
};

} // namespace

#endif // GUI3DQT_HPP_
