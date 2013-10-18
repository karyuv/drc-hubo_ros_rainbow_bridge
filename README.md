This repository promotes visualization of the urdf model of the drc-hubo based on the encoder values published from the real robot.
An on-board computer on the humanoid robot sends in the encoder values as UDP packets to a linux computer(running ROS). This program
is developed for robot state visualization while the humanoid robot DRC-HUBO gets into a car.

Method of using this repo:

1. Fork this into your workspace
2. catkin_make your entire workspace and make sure its error free.
3. Open up a separate terminal and enter::$ roslaunch hubo_state_publisher hubo_state.launch
4. In rviz, click the add button and add "Robot model"
5. Set the fixed frame as "hubo_base".
6. To visualize some movements,play the rosbag files.
