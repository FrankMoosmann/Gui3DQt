/*!
 *  Gui3DQt - a lightweight, modular Gui framework for displaying 3D content
 *  https://github.com/FrankMoosmann/Gui3DQt.git
 * 
 *  \file   Gui.hpp
 *  \brief  Provides a wrapper class for easy setup and exec of Gui3DMainWindow
 *  \author Frank Moosmann (<moosmann@mrt.uka.de>),
 *  \date   2007-2009
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
  Gui(std::string title, int argc, char *argv[], MainWindow::GuiMode gMode = MainWindow::GM_3D2D, bool maximized = false);
  virtual ~Gui();

  void registerVisualizer(Visualizer*, std::string title, MainWindow::VisualizerMode vMode = MainWindow::VM_Groupbox, bool active = true);
  MNavWidget* getQGlWidget();
  MainWindow* getMainWindow();
  void exec();
  
private:
  QApplication        *app;
  MainWindow          *mainWin;
};

} // namespace

#endif // GUI3DQT_HPP_
