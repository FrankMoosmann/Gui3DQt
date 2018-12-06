
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
1) mkdir build
2) cd build
3) cmake ..
4) make

Usage
=====
- Either just include a MNavWidget into your own Gui and provide OpenGL paint functions to it
- Or create your own visualization modules for usage in a MainWindow (see VisualizerGrid or example/Visualizer*)

In general, you should be familiar with Qt and OpenGL. If no, visit e.g. these pages:
http://doc.qt.nokia.com/stable/tutorials.html
http://wiki.delphigl.com/index.php/GlBegin (intended for Delphi, but same syntax in C++)
