#include <ros/ros.h>
#include <cmath>
#include <geometry_msgs/Twist.h>
#include "geometry_msgs/Vector3.h"
#include "geometry_msgs/Quaternion.h"
#include <nav_msgs/Odometry.h>
#include <husky_pos_vel/position.h>




#include "tf/transform_datatypes.h"
#include "tf2/LinearMath/Matrix3x3.h"


float ref_x=-5;
float ref_y=-5;
float curr_x,curr_y,curr_z,vel_x,vel_y,vel_z,ang_z,phi=0;

geometry_msgs::Vector3 rpy;

void SubCallback(husky_pos_vel :: position msg){
    ref_x= msg.x;
    ref_y= msg.y;
return;}

void SubCallback_odom(nav_msgs::Odometry data){
    curr_x = data.pose.pose.position.x;
    curr_y = data.pose.pose.position.y;
    curr_z = data.pose.pose.position.z;

    vel_x= data.twist.twist.linear.x;
    vel_y= data.twist.twist.linear.y;
    vel_z= data.twist.twist.linear.z;
    ang_z= data.twist.twist.angular.z;

    tf::Quaternion q(
        data.pose.pose.orientation.x,
        data.pose.pose.orientation.z,
        data.pose.pose.orientation.y,
        data.pose.pose.orientation.w);
    tf::Matrix3x3 m(q);
    m.getRPY(rpy.x, rpy.y, rpy.z);

    
return;}
int main(int argc, char** argv){
	ros::init(argc, argv,"prop_control");
	ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/husky_velocity_controller/cmd_vel", 100);
    ros::Subscriber sub = nh.subscribe("/husky/reference_pos", 1000, SubCallback);
    ros::Subscriber sub_odom = nh.subscribe("/odometry/filtered", 1000,SubCallback_odom);

    // ros::Subscriber sub_odom = nh.subscribe("/husky_velocity_controller/odom", 1000,SubCallback_odom);
    ros::Rate r(10);

    geometry_msgs::Twist msg;
    
    float integral=0,integral_ang=0,diffrentiator=0,prev_error=0,diffrentiator_ang=0,prev_error_ang=0;

    while(ros::ok()){
    float d = sqrt(pow(curr_x - ref_x, 2) + pow(curr_y - ref_y, 2));
    float df= sqrt(pow(ref_x,2)+pow(ref_y,2));
    float di= sqrt(pow(curr_x,2)+pow(curr_y,2));
    


    float psi = rpy.y; //yaw
    // float phi = psi - atan2((curr_y-ref_y),(curr_x-ref_x));

    phi = psi - atan((curr_y-ref_y)/(curr_x-ref_x));

    if(phi > M_PI)          
        phi = phi - 2*M_PI;
    else if(phi < -M_PI)
        phi = phi + 2*M_PI;


    integral_ang+=phi/10;
    diffrentiator_ang= (phi-prev_error_ang)*10 ;
    prev_error_ang=phi;

    if(d<0.4 ) 
    {
        msg.angular.z = 0;
        msg.linear.x=0;
        integral_ang=0;
        integral=0;
    }
    else 
    {
    msg.angular.z =  - 2*phi -0.005*integral_ang - 1*diffrentiator_ang ;

    // PID implimentation
    
    integral +=(df-di)/10;
    diffrentiator=(df-di-prev_error)*10;
    prev_error=df-di;
    if(phi<=0.01 && phi >=-0.01) 
    {
        if((curr_y-ref_y)>0 &&(curr_x-ref_x)>0)
        {
            msg.linear.x = -2*d - 0.1*integral -1*diffrentiator;
        }
        else if((curr_y-ref_y)<=0 &&(curr_x-ref_x)>0)
        {
            msg.linear.x = -2*d -0.1*integral -1*diffrentiator;
        }
        else
        {
        // if((curr_x -ref_x<0)<0 && (curr_y-ref_y)<0) msg.linear.x = -2*(df-di)*cos(phi) ;
        msg.linear.x = 2*d +0.1*integral +1*diffrentiator;
        // integral_ang=0;
        // msg.angular.z=0;
        }

    }
    }
    pub.publish(msg); 

    ROS_INFO("\n\n\nd:%f\ncurr_x:%f\t curr_y:%f \t ref_x:%f\t ref_y:%f\nvel_x:%f\tvel_y:%f\t vel_z:%f\tphi:%f\t tan:%f\nyaw:%f \t ang_z:%f\n input_vel:%f", d, curr_x,  curr_y, ref_x, ref_y, vel_x, vel_y,vel_z,phi, atan((ref_y-curr_y)/(ref_x-curr_x)), rpy.y, ang_z, msg.linear.x);
    ros::spinOnce();
    r.sleep();
    }

}


