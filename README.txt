
Description
===========
This package provides some classes helping to create a Gui for visualizing 3D content with OpenGL 
- QGlMNavWidget is a QGLWidget (provides OpenGL context) extended with mouse navigation. Can be used within any custom Gui/QWidget
- Gui3DMainWindow implements a main windows with a QGlMNavWidget and a docking area for visualization modules
- Gui3DQt is a wrapper class for easy setup and exec of Gui3DMainWindow
- Gui3DVisualizer is the base class for custom visualization modules usable in Gui3DMainWindow
- Gui3DVisualizerCamControl: nice for generating videos, provides storage/restore of viewing positions and an interpolated flight through all stored poses 
- Gui3DVisualizerGrid is a tiny visualization module displaying a grid in the horizontal plane
- graphics provides some paint functions, not directly included in OpenGL (circles, etc)

Prerequisites
=============
libqt4

boost-function
boost-numeric-ublas
boost-....

Compilation
===========
1) auto-create Makefile by "qmake-qt4 Gui3DQt.pro"
2) compile with "Make"

Installation
============
Currently, this has to be done manually
I recommend installing the headers into a separate directory, e.g. /usr/local/include/Gui3DQt/
Thus, includes in your program will look like this: "#include <Gui3DQt/MainWindows.hpp>"
The shared objects should be installed into the LD_LIBRARY_PATH, e.g. /usr/local/lib/
Hint: sym-linking instead of copying the files helps keeping everything consistent!

Usage
=====
- Either just include a MNavWidget into your own Gui and provide OpenGL paint functions to it
- Or create your own visualization modules for usage in a MainWindow (see VisualizerGrid or example/Visualizer*)

