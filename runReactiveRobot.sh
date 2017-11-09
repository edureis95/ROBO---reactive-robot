#!/bin/bash

cd ~/catkin_ws
. devel/setup.bash
gnome-terminal -x sh -c "roslaunch stdr_launchers server_no_map.launch"
gnome-terminal -x sh -c "roslaunch stdr_gui stdr_gui.launch"
catkin_make --pkg reactive_robot
roscd stdr_resources
rosrun stdr_server load_map maps/D.yaml
#D grande
#rosrun stdr_robot robot_handler add resources/robots/pandora_robot.yaml 9 11 1.57
#D pequeno
rosrun stdr_robot robot_handler add resources/robots/pandora_robot.yaml 9 5 1.57
rosrun reactive_robot reactive_robot_node
