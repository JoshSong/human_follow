cmake_minimum_required(VERSION 2.8.3)
project(human_follow)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp std_msgs tf)

## Declare a catkin package
catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(simple_tracker src/simple_tracker.cpp)
target_link_libraries(simple_tracker ${catkin_LIBRARIES})
