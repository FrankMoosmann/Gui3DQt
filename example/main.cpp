#include <iostream>
#include <boost/thread.hpp>

#include <Gui3DQt/Gui.hpp>
#include <Gui3DQt/VisualizerGrid.hpp>
#include <Gui3DQt/VisualizerCamControl.hpp>
#include "Visualizer1.hpp"
#include "Visualizer2.hpp"

#ifdef WIN32
#include <windows.h>
#endif

using namespace std;
using namespace Gui3DQt;

void myWorkingThread()
{
  cout << "entering working thread" << endl;
#ifndef WIN32
  sleep(3);
#endif
  cout << "start working" << flush;
  for (unsigned int i=0; i<10; i++) {
    cout << "." << flush;
#ifndef WIN32
    sleep(1);
#endif
  } 
  cout << "working finished" << endl;
}

int main(int argc, char *argv[])
{
  boost::thread wt(myWorkingThread);
  Gui myGui("Example GUI", argc, argv, MainWindow::GM_3D);
  myGui.registerVisualizer(new VisualizerGrid(), "Ground Plane Grid"); // myGui will take ownership
  myGui.registerVisualizer(new Visualizer1(), "Visualizer 1"); // myGui will take ownership
  myGui.registerVisualizer(new Visualizer2(), "Visualizer 2"); // myGui will take ownership
  myGui.registerVisualizer(new VisualizerCamControl(*myGui.getQGlWidget()), "Camera Control", MainWindow::VM_Plain); // myGui will take ownership
  myGui.exec();
  return 0;
}

