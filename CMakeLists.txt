cmake_minimum_required(VERSION 2.8)
project( CameraFrameServer )
find_package( OpenCV )

include_directories("${OpenCV_INCLUDE_DIRS}")


find_package(VideoStreamOpenIGTLink REQUIRED)
include(${VideoStreamOpenIGTLink_USE_FILE})

add_executable( CameraFrameServer CameraFrameServer.cpp )
target_link_libraries( CameraFrameServer ${OpenCV_LIBS} ${VideoStreamOpenIGTLink_LIBRARIES})

#add_executable( CameraFrameServer2 CameraFrameServer2.cpp )
#target_link_libraries( CameraFrameServer2 ${VideoStreamOpenIGTLink_LIBRARIES})
