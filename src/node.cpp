#include "ros/ros.h"
#include "pcl_conversions/pcl_conversions.h"
#include <pcl/pcl_base.h>
#include <sstream>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/filters/passthrough.h>
#include <pcl/common/common.h>
#include <geometry_msgs/Twist.h>

typedef pcl::PointXYZ PointType;
typedef pcl::PointCloud<PointType> PointCloud;
typedef PointCloud::Ptr PointCloudPtr;

ros::Publisher pcd_pub_, cmd_vel_pub_;

void pcd_cb(const sensor_msgs::PointCloud2ConstPtr& pcd) {
    ROS_INFO_STREAM_ONCE("Point cloud arrived");
    PointCloudPtr pcd_pcl = PointCloudPtr(new PointCloud), pcd_filtered = PointCloudPtr(new PointCloud);
    PointType pt_min, pt_max;
    pcl::fromROSMsg(*pcd, *pcd_pcl);

    pcl::PassThrough<PointType> pass;
    pass.setInputCloud(pcd_pcl);
    pass.setFilterFieldName("y");
    pass.setFilterLimits(-0.25,0.20);
    pass.filter(*pcd_filtered);

    pass.setInputCloud(pcd_filtered);
    pass.setFilterFieldName("x");
    pass.setFilterLimits(-0.3,0.3);
    pass.filter(*pcd_pcl);

    pcl::getMinMax3D(*pcd_pcl, pt_min, pt_max);

    geometry_msgs::Twist vel;
    if (pt_min.z > 1.0) {
        vel.linear.x = 0.2;
    } else if (pt_min.z > 0.5) {
        vel.linear.x = 0.1;
    } else {
        vel.linear.x = 0.0;
    }

    cmd_vel_pub_.publish(vel);


    sensor_msgs::PointCloud2 pcd_out;
    pcl::toROSMsg(*pcd_pcl, pcd_out);




    pcd_pub_.publish(pcd_out);
}

int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "ed_safety_cmd_vel");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  ros::Subscriber pcd_sub = n.subscribe("/camera/depth/points", 1, pcd_cb);



  pcd_pub_ = n.advertise<sensor_msgs::PointCloud2>("/output", 1);
  cmd_vel_pub_ = n.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/teleop", 1);

  ros::spin();

  return 0;
}

