TEMPLATE = app

#CONFIG += debug
CONFIG += debug_and_release

TARGET = example
QT += core \
    gui \
    opengl
HEADERS += \
  Visualizer1.hpp \
  Visualizer2.hpp
SOURCES += main.cpp \
  Visualizer1.cpp \
  Visualizer2.cpp
FORMS += \
  Visualizer1.ui \
  Visualizer2.ui

INCLUDEPATH += . \
   $(BOOST_INC)
LIBS += \
  -L.. \
  -L$(BOOST_LIB) \
  -lGui3DQt \
  -lglut \
  -lboost_thread

