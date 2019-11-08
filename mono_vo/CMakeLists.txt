cmake_minimum_required(VERSION 2.8)
project( mono_vo )

find_package(OpenCV REQUIRED COMPONENTS core highgui imgproc features2d video calib3d)
if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

include_directories( include )

add_executable( mono_vo_01 mono_vo_01.cpp)
target_link_libraries( mono_vo_01 ${OpenCV_LIBS} )

add_executable( mono_vo_02 main.cpp src/visual_odometry.cpp src/pinhole_camera.cpp )
target_link_libraries( mono_vo_02 ${OpenCV_LIBS} )
