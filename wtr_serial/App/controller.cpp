
#include <ros/ros.h>
#include "geometry_msgs/Twist.h"
const double PI = 3.14159265;
using namespace std;

ros::Publisher twistPub;


int main(int argc, char** argv)
{
    ros::init(argc, argv, "wtr_twist");
    //创建句柄（虽然后面没用到这个句柄，但如果不创建，运行时进程会出错）
    ros::NodeHandle n;
    twistPub = n.advertise<geometry_msgs::Twist>("/wtr/chassis/twist",1);
    ros::Rate loop_rate(100);
    geometry_msgs::Twist speedToPub;
    while(ros::ok())
    {
        speedToPub.angular.z = 0.1;
        speedToPub.linear.x = 0.1;
        speedToPub.linear.y = 0.1;
        twistPub.publish(speedToPub);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
