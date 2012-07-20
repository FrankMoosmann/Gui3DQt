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
#include "Gui.hpp"

#include <QtGui/QApplication>
#include <QtCore/QString>

using namespace std;

namespace Gui3DQt {
  
Gui::Gui(string title, int argc, char *argv[], MainWindow::GuiMode gMode, bool maximized)
{
  app = new QApplication(argc, argv);
  app->connect(app, SIGNAL(lastWindowClosed()), app, SLOT(quit()));

  mainWin = new MainWindow(gMode);

  mainWin->setWindowTitle(QString(title.c_str()));
  if (maximized)
    mainWin->showMaximized();
  else
    mainWin->show();
}

Gui::~Gui()
{
  if (mainWin) delete mainWin; // only top level widget must be destroyed, rest gets destroyed by parent-child-relationship
  if (app) delete app;
}


void Gui::exec()
{
  app->exec();
}


void Gui::registerVisualizer(Visualizer *vis, std::string title, MainWindow::VisualizerMode vMode, bool active)
{
  mainWin->registerVisualizer(vis, title, vMode, active);
}

MNavWidget* Gui::getQGlWidget()
{
  return mainWin->getMNavWidget();
}

MainWindow* Gui::getMainWindow()
{
  return mainWin;
}

}
