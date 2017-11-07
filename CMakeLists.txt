cmake_minimum_required(VERSION 2.8.3)
project(reactive_robot)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  sensor_msgs
)

catkin_package(

)

include_directories(
  ${catkin_INCLUDE_DIRS}
)


add_executable(reactive_robot_node src/reactive_robot.cpp)


add_dependencies(reactive_robot_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

 target_link_libraries(reactive_robot_node
   ${catkin_LIBRARIES}
 )


