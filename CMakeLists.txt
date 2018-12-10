cmake_minimum_required(VERSION 3.1.0)

project(Gui3DQt)

# As moc files are generated in the binary dir, tell CMake to look for includes there
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(OpenGL REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets Core OpenGL)
find_package(GLUT REQUIRED)

add_library(${PROJECT_NAME} 
    include/Gui3DQt/graphics.hpp
    include/Gui3DQt/Gui.hpp
    include/Gui3DQt/MainWindow.hpp
    include/Gui3DQt/MNavWidget.hpp
    include/Gui3DQt/passatmodel.hpp
    include/Gui3DQt/PointCloudRenderer.hpp
    include/Gui3DQt/Visualizer.hpp
    include/Gui3DQt/VisualizerCamControl.hpp
    include/Gui3DQt/VisualizerGrid.hpp
    include/Gui3DQt/VisualizerPassat.hpp
    graphics.cpp
    Gui.cpp
    MainWindow.cpp
    MainWindow.ui
    MNavWidget.cpp
    models3d.hpp
    model3dpassat.cpp
    model3dpassatwagon.cpp
    model3dtire.cpp
    model3dvelodyne.cpp
    passatmodel.cpp
    PointCloudRenderer.cpp
    spline.hpp
    VisualizerCamControl.cpp
    VisualizerCamControl.ui
    VisualizerGrid.cpp
    VisualizerPassat.cpp
    VisualizerPassat.ui
)

qt5_use_modules(${PROJECT_NAME} Widgets Core OpenGL)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(${PROJECT_NAME} 
    ${Boost_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)


add_subdirectory(example)
