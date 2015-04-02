#include "ros/ros.h"
#include <math.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"
#include "gazebo_msgs/ModelState.h"
#include <gazebo_msgs/GetModelState.h>
#include <gazebo_msgs/GetWorldProperties.h>
#define des_x 0
#define des_y -18
#define des_o 0
#define cor_x (getmodelstate.response.pose.position.x - des_x)
#define cor_y (getmodelstate.response.pose.position.y - des_y)
#define or_x getmodelstate.response.pose.orientation.x
#define or_y getmodelstate.response.pose.orientation.y
#define or_z getmodelstate.response.pose.orientation.z
#define or_w getmodelstate.response.pose.orientation.w 
#define cor_o normalizeangle((2*atan2(or_z,or_w)-des_o))
#define MIN_BOT_SPEED 1
#define MAX_BOT_SPEED 10
#define MIN_BOT_ANG 0.3
#define MAX_BOT_ANG 1.5
#define BOT_POINT_THRESH 0.1
#define k1 2
#define k2 1
#define k3 1
#define pi 3.14159


/**
 * This tutorial demonstrates to move bot along a smooth trajactory between 2 points in Gazebo.
 * Initial point (-12,-12,0) --> Final Position (0,-18,0)
 */

/* Class to convert xyz coordinates to pyd and give v and w */
double normalizeangle(double angle) {
angle = fmod(angle, 2*pi); // (0, 2PI)
if(angle > pi) angle -= 2*pi; // (-PI, PI)
return angle;
}
class polar_Cordinate
{

double p;
double y;
double d;
double v;
double w;
double v_curve;
double k;
public:
polar_Cordinate(){}
void set_values(double a,double b, double c)
  {
	p=sqrt(a*a+b*b);
	y=normalizeangle(atan2(b,a)-c+pi);
	d=normalizeangle(y+c);
	v=(k1*p*cos(y));
        if(y==0)
	  w= k2*y+k1*cos(y)*(y+k3*d);
        else
	  w= k2*y+k1*sin(y)*cos(y)*(y+k3*d)/y;
        v_curve=0;
   }
void vel_profile() // TO KEEP CARVATURE WITHIN SOME RANGE //not working 
  {
	 k = ((w/v)>0)?(w/v):(-1)*(w/v); // k = curvature 
	if(k<=0.2)
	{
	   v_curve=1;
	   w=1;
	   v=5;
	}
	else if(k>=5)
	{ 
	v_curve=2;
	v=MIN_BOT_SPEED;
	w=1;
	}

}

double get_v() {return v;}
double get_w() {return w;}
double get_p() {return p;}
double get_y() {return y;}
double get_d() {return d;}
double get_k() {return k;}
double get_vc() {return v_curve;}
};


int main(int argc, char **argv)
{

  ros::init(argc, argv, "SwarmSimu");

  ros::NodeHandle n;

  ros::Publisher vel_pub_0 = n.advertise<geometry_msgs::Twist>("/swarmbot0/cmd_vel", 1);
  ros::Rate loop_rate(20);

  ros::ServiceClient serv_client = n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state"); 
  gazebo_msgs::GetModelState getmodelstate;
  gazebo_msgs::ModelState modelstate;

  std::string s="swarmbot0";
  polar_Cordinate pol_c;
  int count = 0;

  while (ros::ok())
{
    geometry_msgs::Twist cmd_vel;
    getmodelstate.request.model_name=s;
    serv_client.call(getmodelstate);
    pol_c.set_values(cor_x,cor_y,cor_o);// velocities are set
    if(count%20==0) // To decrease the no. of prints 
    ROS_INFO("(%lf,%lf,%lf) (%lf,%lf,%lf)  ",cor_x,cor_y,cor_o,pol_c.get_p(),pol_c.get_y(),pol_c.get_d());
    
  
  if(((pol_c.get_p()>0.5||cor_o >0.1)||count<50))
  {    
    // pol_c.vel_profile(); // for velocity profiling
    if(count%20==0) // To decrease the no. of prints 
    ROS_INFO("(case %lf, k=%lf)   (%lf,%lf) ",pol_c.get_vc(),pol_c.get_k(),pol_c.get_v(),pol_c.get_w());
    
    cmd_vel.linear.x=pol_c.get_v();
    cmd_vel.linear.y = 0;
    cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = pol_c.get_w();
  } 
  else
  {
    cmd_vel.linear.x = 0;
    cmd_vel.linear.y = 0;
    cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = 0;
  }
    vel_pub_0.publish(cmd_vel);

    count++;
    ros::spinOnce();
    loop_rate.sleep();
}



    
  return 0;
}
