cmake_minimum_required(VERSION 2.8)
project( CameraFrameServer )
find_package( OpenCV )
include_directories("${OpenCV_INCLUDE_DIRS}")

find_package(OpenIGTLink REQUIRED)
include(${OpenIGTLink_USE_FILE})
message("${OpenIGTLink_INCLUDE_DIRS}")

add_executable( CameraFrameServer CameraFrameServer.cpp )
target_link_libraries( CameraFrameServer ${OpenCV_LIBS} OpenIGTLink)
