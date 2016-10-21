#indicate minimum version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#project name
PROJECT(circle_detector)

#find required packages (look for the package, usually at /usr/share/cmake-2.8/Modules/ or /usr/share/)
FIND_PACKAGE(OpenCV REQUIRED)

#set header directories
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIR})

#Create an executable
ADD_EXECUTABLE(${PROJECT_NAME} src/circle_detector.cpp)

#Link with libraries
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OpenCV_LIBS})
