cmake_minimum_required(VERSION 3.1.0)

project(example)

# As moc files are generated in the binary dir, tell CMake to look for includes there
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

find_package(Boost COMPONENTS thread REQUIRED)

find_package(Qt5 REQUIRED COMPONENTS Widgets)


add_executable(${PROJECT_NAME}
    main.cpp
    Visualizer1.cpp
    Visualizer1.ui
    Visualizer2.cpp
    Visualizer2.ui
)

qt5_use_modules(${PROJECT_NAME} Widgets)

target_link_libraries(${PROJECT_NAME}
    ${Boost_THREAD_LIBRARY}
    Gui3DQt 
)

