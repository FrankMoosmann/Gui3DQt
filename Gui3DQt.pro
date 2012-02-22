TEMPLATE = lib
#CONFIG += staticlib
CONFIG -= debug_and_release
#CONFIG += debug
CONFIG += release
QMAKE_CXXFLAGS_RELEASE += -O3
VERSION = 0.1
TARGET = Gui3DQt
QT += core \
    gui \
    opengl
HEADERS += \
    graphics.hpp \
    spline.hpp \
    models3d.hpp \
    passatmodel.hpp \
    Gui.hpp \
    MainWindow.hpp \
    MNavWidget.hpp \
    Visualizer.hpp \
    VisualizerPassat.hpp \
    VisualizerCamControl.hpp \
    VisualizerGrid.hpp
SOURCES += \
    graphics.cpp \
    model3dpassat.cpp \
    model3dpassatwagon.cpp \
    model3dtire.cpp \
    model3dvelodyne.cpp \
    passatmodel.cpp \
    Gui.cpp \
    MainWindow.cpp \
    MNavWidget.cpp \
    VisualizerPassat.cpp \
    VisualizerCamControl.cpp \
    VisualizerGrid.cpp
FORMS += \
    VisualizerPassat.ui \
    VisualizerCamControl.ui \
    MainWindow.ui
RESOURCES += 
LIBS += \
    -lboost_filesystem \
    -lboost_system \
    -lglut
