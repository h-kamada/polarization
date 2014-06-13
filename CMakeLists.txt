cmake_minimum_required (VERSION 2.6)

project (diff_of_picture)
find_package( OpenCV REQUIRED )
add_executable (diff_of_picture diff_of_picture.cpp)
target_link_libraries( diff_of_picture ${OpenCV_LIBS} )
