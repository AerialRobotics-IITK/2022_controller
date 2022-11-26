#include <ros/ros.h>
#include <husky_pos_vel/position.h>
#include <string>

int main(int argc, char **argv){
    ros::init(argc, argv, "pub"); 
    int x = 10, y = 12;

    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<husky_pos_vel::position>("/husky/reference_pos", 100);

    ros::Rate r(10);
    

    while(ros::ok()){
        husky_pos_vel::position msg;

        nh.getParam("/ref_pos/x", x);
        nh.getParam("/ref_pos/y", y);
        // std:: cin>>x>>y;

        msg.x = x;
        msg.y = y;

        pub.publish(msg);
        r.sleep();
    }
    
    return 0;
}