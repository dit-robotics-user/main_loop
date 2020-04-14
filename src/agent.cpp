#include "ros/ros.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "std_msgs/String.h"
#include "std_msgs/Header.h"
#include "main_loop/agent.h"
#include "sensor_msgs/LaserScan.h"
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Int32.h>
#include "lidar_2020/alert_range.h"



class sub_class{
    public:
        void ST1_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg);
        void ST2_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg); 
        void lidarmsg_sub_callback(const lidar_2020::alert_range::ConstPtr& msg);
        void status_sub_callback(const std_msgs::Int32::ConstPtr& msg);
        void publish_(float time);
        void status_publish();   
        
        sub_class(int my_pos_x_ = 700, int my_pos_y_ = 300, int ini_status = 0);
        ~sub_class(){};

    private:
        ros::NodeHandle n;
        
        ros::Publisher status_pub = n.advertise<std_msgs::Int32>("pub_status",1); 
		ros::Publisher agent_pub= n.advertise<main_loop::agent>("agent_msg", 1);
        ros::Subscriber status_sub= n.subscribe<std_msgs::Int32>("update_status", 1, &sub_class::status_sub_callback,this);
		ros::Subscriber ST1_sub = n.subscribe<std_msgs::Int32MultiArray>("rxST1", 1, &sub_class::ST1_sub_callback,this);
        ros::Subscriber ST2_sub = n.subscribe<std_msgs::Int32MultiArray>("rxST2", 1, &sub_class::ST2_sub_callback,this);
        ros::Subscriber lidarmsg_sub= n.subscribe<lidar_2020::alert_range>("ranging_alert", 10, &sub_class::lidarmsg_sub_callback,this);
        
        std_msgs::Int32 status;
        main_loop::agent pub_to_main;
};

sub_class::sub_class(int my_pos_x_,int my_pos_y_, int ini_status){
    pub_to_main.my_pos_x = 700 ; 
    pub_to_main.my_pos_y = 300 ; 
    pub_to_main.my_degree = 90 ; 
	pub_to_main.my_pos_x = 700 ;
    pub_to_main.my_pos_y = 300 ;
    pub_to_main.enemy1_x = 380 ;
    pub_to_main.enemy1_y = 2400 ;
    pub_to_main.enemy2_x = 380 ;
    pub_to_main.enemy2_y = 2300 ;
    pub_to_main.ally_x = 380 ;
    pub_to_main.ally_y = 2200 ; 
    status.data = ini_status;
}

void sub_class::status_sub_callback(const std_msgs::Int32::ConstPtr& msg){
	status.data = msg->data ;
    pub_to_main.status = msg->data ; 
}
void sub_class::status_publish(){
    pub_to_main.status = status.data ;
	status_pub.publish(status);
}

void sub_class::ST1_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg){
    pub_to_main.my_pos_x = msg->data[0] ;
    pub_to_main.my_pos_y = msg->data[1] ;
    pub_to_main.my_degree = msg->data[2] ;
    ROS_INFO("my_pos_x: %d", pub_to_main.my_pos_x);
    ROS_INFO("my_pos_y: %d", pub_to_main.my_pos_y);
    ROS_INFO("ST1");
}

void sub_class::ST2_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg){
    pub_to_main.servo_state = msg->data[0] ;
    pub_to_main.stepper = msg->data[1] ;
    pub_to_main.hand = msg->data[2] ;    
}

void sub_class::lidarmsg_sub_callback(const lidar_2020::alert_range::ConstPtr& msg){
    pub_to_main.emergency={};
    if(msg->header.seq>2){
        for(int j=0 ;j<8;j++){
            pub_to_main.emergency.push_back(msg->alert[j]);
        }  
    }
}

void sub_class::publish_(float time ){
    pub_to_main.time =time ; 
    agent_pub.publish(pub_to_main);
}

int main(int argc, char **argv){
    ros::init(argc,argv, "agent_add_status");
    sub_class temp;
    ros::Time::init();
    float begin_time =ros::Time::now().toSec();
    
    while(ros::ok()){
        float clustering_time = ros::Time::now().toSec () - begin_time ;
        
        temp.publish_(clustering_time);
        temp.status_publish();
        ros::spinOnce();
     
    }

}
