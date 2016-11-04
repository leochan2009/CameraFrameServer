cmake_minimum_required(VERSION 2.8)
project( CameraFrameServer )
find_package( OpenCV )

get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)
message("${PARENT_DIR}/OpenIGTLink-XcodeBuild/Testing/OpenH264")
set(OpenH264_INCLUDE "${PARENT_DIR}/OpenIGTLink-XcodeBuild/Testing/OpenH264" CACHE STRING "OpenH264 source file")
set(OpenH264_LIB "${PARENT_DIR}/OpenIGTLink-XcodeBuild/Testing/OpenH264/Debug" CACHE STRING "OpenH264 library")

include_directories("${OpenCV_INCLUDE_DIRS}")
include_directories("${OpenH264_INCLUDE}/codec")
include_directories("${OpenH264_INCLUDE}/res")
include_directories("${OpenH264_INCLUDE}/test")
LINK_DIRECTORIES("${OpenH264_LIB}")

set(OpenIGTLink_INCLUDE "${PARENT_DIR}/Slicer-build/OpenIGTLink/Source" CACHE STRING "OpenIGTLink source file")
set(OpenIGTLink_LIB "${PARENT_DIR}/Slicer-build/OpenIGTLink-build" CACHE STRING "OpenIGTLink library")

include_directories("${OpenCV_INCLUDE_DIRS}")
include_directories("${OpenIGTLink_INCLUDE}")
include_directories("${OpenIGTLink_INCLUDE}/igtlutil")
include_directories("${OpenIGTLink_LIB}")
LINK_DIRECTORIES("${OpenIGTLink_LIB}/bin")


find_package(VideoStreamOpenIGTLink REQUIRED)
include(${VideoStreamOpenIGTLink_USE_FILE})

add_executable( CameraFrameServer CameraFrameServer.cpp )
target_link_libraries( CameraFrameServer ${OpenCV_LIBS} OpenIGTLink ${VideoStreamOpenIGTLink_LIBRARIES})

#add_executable( CameraFrameServer2 CameraFrameServer2.cpp )
#target_link_libraries( CameraFrameServer2 ${VideoStreamOpenIGTLink_LIBRARIES})
