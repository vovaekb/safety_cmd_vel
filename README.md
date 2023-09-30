# safety_cmd_vel

ROS package for ROS workshop 2016 organized by Robotics Research group on Faculty of Information Technology, Brno University of Technology.

Description: navigating robot based on distance to obstacle. 
We obtain PointXYZ point cloud (no color) from Kinect sensor, filter a PointCloud using a PassThrough filter along Z axis and send commands to topic /cmd_vel_mux/input/teleop.

ROS nodes:
safety_cmd_vel_node 

Dependencies:
- geometry_msgs
- pcl_conversions
- roscpp
- std_msgs




To run node:

