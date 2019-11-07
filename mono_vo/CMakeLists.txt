cmake_minimum_required(VERSION 2.8)
project( mono_vo )

find_package(OpenCV REQUIRED COMPONENTS core highgui imgproc features2d video calib3d)
if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

add_executable( mono_vo mono_vo.cpp )
target_link_libraries( mono_vo ${OpenCV_LIBS} )
