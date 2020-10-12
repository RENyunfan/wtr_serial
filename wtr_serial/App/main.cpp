//
// Created by kevin on 2020/10/12.
//

#include <ros/ros.h>
#include <serial/serial.h>
#include <iostream>
#include <geometry_msgs/PoseStamped.h>
#include <queue>
#include <std_msgs/Float32MultiArray.h>
#include <vector>
#include "fstream"
#include "std_msgs/UInt8.h"
#include "geometry_msgs/Twist.h"
const double PI = 3.14159265;
using namespace std;
//创建一个serial类
serial::Serial sp;
queue<geometry_msgs::PoseStamped> msgQueue;
queue<std_msgs::Float32MultiArray> angQueue;
ros::Publisher encoderPub;
ros::Publisher anglePub;
ros::Subscriber twistSub;


uint8_t twistSend[8]={0xff,0xaa,0x00,0x00,0x00,0x00,0x00,0x00};

void  sendSpeed(double vx,double vy,double va){
    short speed_x, speed_y, speed_a;
    speed_x = (short)(vx* 100);
    speed_y = (short)(vy * 100);
    speed_a = (short)(va * 100);

    twistSend[2] =(uint8_t)( speed_x >> 8);
    twistSend[3] =(uint8_t)( speed_x);
    twistSend[4] =(uint8_t)( speed_y >> 8);
    twistSend[5] =(uint8_t)( speed_y);
    twistSend[6] =(uint8_t)( speed_a >> 8);
    twistSend[7] =(uint8_t)( speed_a);
}

void twistCallback(const geometry_msgs::TwistConstPtr &msg){
    short speed_x, speed_y, speed_a;
    speed_x = (short)(msg->linear.x * 100);
    speed_y = (short)(msg->linear.y * 100);
    speed_a = (short)(msg->angular.z * 100);

    twistSend[2] =(uint8_t)( speed_x >> 8);
    twistSend[3] =(uint8_t)( speed_x);
    twistSend[4] =(uint8_t)( speed_y >> 8);
    twistSend[5] =(uint8_t)( speed_y);
    twistSend[6] =(uint8_t)( speed_a >> 8);
    twistSend[7] =(uint8_t)( speed_a);
}

void chassisStop(){
	sendSpeed(0,0,0);
int stop_cnt = 100;
while(stop_cnt--){
 int writeBytes  = sp.write(twistSend,sizeof(twistSend));
            if(writeBytes == 8){
               //  for(int i = 0 ; i < 8 ; i ++){
               //     printf("%x",twistSend[i]);
               // }
            }
usleep(1000);
}
cout<<"Shutting down the chassis"<<endl;
	
}

int main(int argc, char** argv)//argc是命令行总的参数个数
//argv[]为保存命令行参数的字符串指针，其中第0个参数是程序的全名，以后的参数为命令行后面跟的用户输入的参数
{
    ros::init(argc, argv, "serial_port");
    //创建句柄（虽然后面没用到这个句柄，但如果不创建，运行时进程会出错）
    ros::NodeHandle n;
    //创建timeout
    serial::Timeout to = serial::Timeout::simpleTimeout(100);
    //设置要打开的串口名称
    sp.setPort("/dev/ttyUSB0");
    //设置串口通信的波特率
    sp.setBaudrate(115200);
    //串口设置timeout
    sp.setTimeout(to);
    twistSub = n.subscribe("/wtr/chassis/twist",1,twistCallback);
    try
    {
        //打开串口
        sp.open();
    }
    catch(serial::IOException& e)//捕捉输入输出异常
    {
        ROS_ERROR_STREAM("Unable to open port.");
        return -1;
    }

    //判断串口是否打开成功
    if(sp.isOpen())
    {
        ROS_INFO_STREAM("/dev/ttyUSB0 is opened.");
    }
    else
    {
        return -1;
    }
    ros::Rate loop_rate(1000);
    int serial_cnt = 0;
    while(ros::ok())
    {
        // 为了通讯稳定，发送频率限制在50Hz
        if(serial_cnt ++ > 20 ){
            serial_cnt = 0;
            int writeBytes  = sp.write(twistSend,sizeof(twistSend));
            if(writeBytes == 8){
               //  for(int i = 0 ; i < 8 ; i ++){
               //     printf("%x",twistSend[i]);
               // }
            }
            //cout<<endl;
        }
        ros::spinOnce();
        loop_rate.sleep();
    }
    // 
    chassisStop();
    sp.close();
    return 0;
}
