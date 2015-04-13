#include "ros/ros.h"
#include <math.h>
#include <utility>
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"
#include "gazebo_msgs/ModelState.h"
#include <gazebo_msgs/GetModelState.h>
#include <gazebo_msgs/GetWorldProperties.h>
#include<string.h>

#define VError (vel.linear.x-getmodelstate.response.twist.linear.x)
#define AError (vel.angular.z-getmodelstate.response.twist.angular.z)

/*
Reads data from cmd_vel and publishes to cmd_vel2, the PID velocities at 100 times the rate of publishing to cmd_vel
*/
void pid(geometry_msgs::Twist vel)
{
	double v,w;
	char *name="swarmbot0";
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<gazebo_msgs::GetWorldProperties>("/gazebo/get_world_properties");
	ros::ServiceClient serv_client = n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
	ros::Publisher vel_pub_0 = n.advertise<geometry_msgs::Twist>("/swarmbot0/cmd_vel2", 1);
	gazebo_msgs::GetModelState getmodelstate;
	getmodelstate.request.model_name=name;
	geometry_msgs::Twist vel2;
	ros::Rate loop_rate(5000);
	double IVerror=0;
	double IAerror=0;
	double lastVerror=0;
	double lastAerror=0;
	double P=D=1;
	double I=0.1;
	for(int i=0;i<100;i++)
	{
		serv_client.call(getmodelstate); 
	
	
		/*calculate pid velocities.*/
		double verror=  P*VError +I*IVerror + D*(VError-lastVError);
		double aerror=  P*AError +I*IAerror + D*(AError-lastAError)
		IVerror+=verror;
		IAerror+=aerror;
		lastVerror=verror;
		lastAerror=aerror;
		vel2.linear.x=  vel.linear.x +verror;
		vel2.angular.z= vel.angular.z+aerror;
		vel2.linear.y=vel2.linear.z=vel.angular.x=vel2.angular.y=0;		
		
		vel_pub_0.publish(vel2);
		ros::spinOnce();
    		loop_rate.sleep();
    	}
}
int main(int argc, char **argv)
{
	 ros::init(argc, argv, "control");
	 ros::NodeHandle n;
	 ros::Subscriber sub = n.subscribe("cmd_vel", 1000, pid);
	 ros::spin();
	 return 0;
} 
