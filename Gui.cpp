#include "Gui.hpp"

#include <QtGui/QApplication>
#include <QtCore/QString>

using namespace std;

namespace Gui3DQt {
  
Gui::Gui(string title, int argc, char *argv[], MainWindow::GuiMode gMode, MainWindow::VisualizerMode vMode)
{
  app = new QApplication(argc, argv);
  app->connect(app, SIGNAL(lastWindowClosed()), app, SLOT(quit()));

  mainWin = new MainWindow(gMode, vMode);

  mainWin->setWindowTitle(QString(title.c_str()));
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


void Gui::registerVisualizer(Visualizer *vis, std::string title)
{
  mainWin->registerVisualizer(vis, title);
}

MNavWidget* Gui::getQGlWidget()
{
  return mainWin->getMNavWidget();
}

}
