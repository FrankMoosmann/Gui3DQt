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
    MNavWidget.hpp \
    Gui.hpp \
    MainWindow.hpp \
    Visualizer.hpp \
    VisualizerCamControl.hpp \
    VisualizerGrid.hpp
SOURCES += \
    graphics.cpp \
    MNavWidget.cpp \
    Gui.cpp \
    MainWindow.cpp \
    VisualizerCamControl.cpp \
    VisualizerGrid.cpp
FORMS += \
    VisualizerCamControl.ui \
    MainWindow.ui
RESOURCES += 
LIBS += \
    -lboost_filesystem \
    -lboost_system \
    -lglut
