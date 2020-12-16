cmake_minimum_required(VERSION 3.0)
project(PixelBased)



find_package( OpenCV REQUIRED )



set(CMAKE_CXX_STANDARD 14)

add_executable(PixelBased main.cpp)




include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
