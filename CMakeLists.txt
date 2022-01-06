cmake_minimum_required(VERSION 3.0.0)
project(assist VERSION 0.1.0)
add_subdirectory(/mnt/data/proj/code/backend/cpp/fmt fmt EXCLUDE_FROM_ALL)

set(Boost_DEBUG ON)
find_package(Boost COMPONENTS program_options )
message(STATUS "BOOST: ${Boost_LIBRARIES}")

find_package(Threads REQUIRED)
# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)

message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

add_executable(assist src/main.cpp src/mjpeg_server.cpp )

# add_library(simpletcp SHARED  )
include_directories(simpletcp PRIVATE tcp)
target_link_libraries(assist ${OpenCV_LIBS}  fmt::fmt-header-only Threads::Threads ${Boost_LIBRARIES})