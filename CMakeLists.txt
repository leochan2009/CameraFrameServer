cmake_minimum_required(VERSION 2.8)
project( CameraFrameServer )
find_package( OpenCV )
include_directories("${OpenCV_INCLUDE_DIRS}")

find_package(OpenIGTLink REQUIRED)
include(${OpenIGTLink_USE_FILE})

SET(VP9_LIB /Users/longquanchen/Desktop/Github/VP9/libvpx/libvpx.a)
SET(VP9_INCLUDE_DIRS /Users/longquanchen/Desktop/Github/VP9/libvpx)
include_directories(${VP9_INCLUDE_DIRS})

add_executable( CameraFrameServer CameraFrameServer.cpp /Users/longquanchen/Desktop/Github/VP9/libvpx/tools_common.h /Users/longquanchen/Desktop/Github/VP9/libvpx/tools_common.c /Users/longquanchen/Desktop/Github/VP9/libvpx/video_writer.h /Users/longquanchen/Desktop/Github/VP9/libvpx/video_writer.c /Users/longquanchen/Desktop/Github/VP9/libvpx/ivfenc.h /Users/longquanchen/Desktop/Github/VP9/libvpx/ivfenc.c)
target_link_libraries( CameraFrameServer ${OpenCV_LIBS} OpenIGTLink ${VP9_LIB})


add_executable( CameraFrameDecoder CameraFrameDecoder.cpp /Users/longquanchen/Desktop/Github/VP9/libvpx/tools_common.h /Users/longquanchen/Desktop/Github/VP9/libvpx/tools_common.c /Users/longquanchen/Desktop/Github/VP9/libvpx/video_reader.h /Users/longquanchen/Desktop/Github/VP9/libvpx/video_reader.c /Users/longquanchen/Desktop/Github/VP9/libvpx/ivfdec.h /Users/longquanchen/Desktop/Github/VP9/libvpx/ivfdec.c)
target_link_libraries( CameraFrameDecoder OpenIGTLink ${VP9_LIB})
