cmake_minimum_required(VERSION 2.8)

project(FeatureViewer)

find_package(OpenCV REQUIRED)

add_executable(FeatureViewer main.cpp)

add_definitions("-std=c++11")

target_link_libraries(FeatureViewer ${OpenCV_LIBS})
