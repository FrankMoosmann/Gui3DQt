TEMPLATE = lib
#CONFIG += staticlib

#CONFIG -= debug_and_release
#CONFIG += debug
CONFIG += release
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
    VisualizerGrid.hpp \
    PointCloudRenderer.hpp
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
    VisualizerGrid.cpp \
    PointCloudRenderer.cpp
FORMS += \
    VisualizerPassat.ui \
    VisualizerCamControl.ui \
    MainWindow.ui
RESOURCES += 
INCLUDEPATH += $(BOOST_INC)
LIBS += \
    -L$(BOOST_LIB) \
    -lboost_filesystem \
    -lboost_system \
    -lglut
