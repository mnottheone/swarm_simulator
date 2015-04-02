#include "ros/ros.h"
#include <math.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"
#include "gazebo_msgs/ModelState.h"
#include <gazebo_msgs/GetModelState.h>
#include <gazebo_msgs/GetWorldProperties.h>
#define cor_x getmodelstate.response.pose.position.x
#define cor_y getmodelstate.response.pose.position.y
#define or_x getmodelstate.response.pose.orientation.x
#define or_y getmodelstate.response.pose.orientation.y
#define or_z getmodelstate.response.pose.orientation.z
#define or_w getmodelstate.response.pose.orientation.w


/**
 * This tutorial demonstrates simple sending of velocity commands to the IRobot Create in Gazebo.
 * Bot will follow 8 trajactory
 */
int main(int argc, char **argv)
{

  ros::init(argc, argv, "SwarmSimu");
  ros::NodeHandle n;
  ros::Publisher vel_pub_0 = n.advertise<geometry_msgs::Twist>("/swarmbot0/cmd_vel", 1);
  ros::Rate loop_rate(1);
  ros::ServiceClient serv_client = n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state"); 
  gazebo_msgs::GetModelState getmodelstate;
  gazebo_msgs::ModelState modelstate;

  std::string s="swarmbot0";
  int count = 0;

  while (ros::ok())
  {
    geometry_msgs::Twist cmd_vel;
    getmodelstate.request.model_name=s;
    serv_client.call(getmodelstate);

    cmd_vel.linear.x = 5;
    cmd_vel.linear.y = 0;
    cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = 1;
    ROS_INFO("%lf ,%lf ,%lf",cor_x,cor_y,or_w);
    vel_pub_0.publish(cmd_vel);
    ros::spinOnce();
    if(or_w<-0.99)break;
    loop_rate.sleep();
  }
    geometry_msgs::Twist cmd_vel;
    cmd_vel.linear.x = 5;
    cmd_vel.linear.y = 0;
    cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = -1;
    vel_pub_0.publish(cmd_vel);

   while (ros::ok())
  {    
    ROS_INFO("%lf ,%lf ,%lf",cor_x,cor_y,or_w);
    serv_client.call(getmodelstate);    
    ros::spinOnce();
	if(or_w>0.99)
	{
	cmd_vel.linear.x = 0;
	cmd_vel.angular.z=0;
	vel_pub_0.publish(cmd_vel);
	break;
	}
    loop_rate.sleep();
  }
    cmd_vel.linear.x = 0;
    cmd_vel.angular.z=0;
    vel_pub_0.publish(cmd_vel);
  return 0;
}
