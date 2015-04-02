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
 * Bot path (-12,-12) -> (2,-12) -> (2,-20)
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

  while (ros::ok()&&cor_x<2)
  {
    geometry_msgs::Twist cmd_vel;
    getmodelstate.request.model_name=s;
    serv_client.call(getmodelstate);

    cmd_vel.linear.x = (cor_x<2)?2:-2;
    cmd_vel.linear.y = 0;
    cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = 0;
    ROS_INFO("%lf ,%lf",cor_x,cor_y);
    vel_pub_0.publish(cmd_vel);
    ros::spinOnce();
    loop_rate.sleep();

  } geometry_msgs::Twist cmd_vel;

    cmd_vel.linear.x = 0;
    cmd_vel.linear.y = 0;
    cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = -0.5;
    vel_pub_0.publish(cmd_vel);

while (ros::ok())
  {
    ROS_INFO("%lf ,",or_w);
    serv_client.call(getmodelstate);    
    ros::spinOnce();
	if(or_w<=0.71)
	{
	cmd_vel.angular.z=0;
	vel_pub_0.publish(cmd_vel);
	break;
	}
     loop_rate.sleep();
  }
   ROS_INFO("MOVING IN Y-DIRECTION/n");
 while (ros::ok()&&(cor_y>-20))
  {
    geometry_msgs::Twist cmd_vel;
    getmodelstate.request.model_name=s;
    serv_client.call(getmodelstate);
    cmd_vel.linear.x = -2;
    cmd_vel.linear.y = 0;
    cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = 0;
    ROS_INFO("%lf ,%lf",cor_x,cor_y);
    vel_pub_0.publish(cmd_vel);
    ros::spinOnce();
    loop_rate.sleep();

  }
    getmodelstate.request.model_name=s;
    serv_client.call(getmodelstate);
    cmd_vel.linear.x = 1;
    cmd_vel.linear.y = 0;
    cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = 0;
    ROS_INFO("%lf ,%lf",cor_x,cor_y);
    vel_pub_0.publish(cmd_vel);
  return 0;
}
