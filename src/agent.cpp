#include "ros/ros.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "std_msgs/String.h"
#include "std_msgs/Header.h"
#include "main_loop/string_with_header.h"
#include "main_loop/agent.h"
#include "sensor_msgs/LaserScan.h"
#include <std_msgs/Int32MultiArray.h>
#include "lidar_2020/alert_range.h"




class sub_class{
    public:
        void ST1_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg);
        void ST2_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg); 
        void lidarmsg_sub_callback(const lidar_2020::alert_range::ConstPtr& msg);
        void publish_(float time);
        sub_class(int my_pos_x_ = 300, int my_pos_y_ = 300,int my_degree_ = 300);
        ~sub_class(){};

    private:
        ros::NodeHandle n;
		ros::Publisher agent_pub= n.advertise<main_loop::agent>("agent_msg", 1);
		ros::Subscriber ST1_sub = n.subscribe<std_msgs::Int32MultiArray>("rxST1", 1, &sub_class::ST1_sub_callback,this);
        ros::Subscriber ST2_sub = n.subscribe<std_msgs::Int32MultiArray>("rxST2", 1, &sub_class::ST2_sub_callback,this);
        ros::Subscriber lidarmsg_sub= n.subscribe<lidar_2020::alert_range>("ranging_alert", 10, &sub_class::lidarmsg_sub_callback,this);
        main_loop::agent pub_to_main;

};

sub_class::sub_class(int my_pos_x_,int my_pos_y_,int my_degree_){
    pub_to_main.my_pos_x = my_pos_x_ ; 
    pub_to_main.my_pos_y = my_pos_y_ ; 
    pub_to_main.my_degree = my_degree_ ; 
  
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
    
    pub_to_main.task_state = msg->data[0] ;
    pub_to_main.ally_x = msg->data[1] ;
    pub_to_main.ally_y = msg->data[2] ;
    ROS_INFO("rx[0]=: %d", pub_to_main.task_state); 
    ROS_INFO("rx[1]=: %d", pub_to_main.ally_x);
    ROS_INFO("rx[2]=: %d", pub_to_main.ally_y);
    ROS_INFO("ST2");
}

void sub_class::lidarmsg_sub_callback(const lidar_2020::alert_range::ConstPtr& msg){

    pub_to_main.emergency={};
    if(msg->header.seq>2){
        for(int j=0 ;j<8;j++){
            pub_to_main.emergency.push_back(msg->alert[j]);
            ROS_INFO("lidar[%d] in agent: %d",j, pub_to_main.emergency[j]);
        }  
    }
}

void sub_class::publish_(float time ){
    pub_to_main.time =time ; 
    agent_pub.publish(pub_to_main);
}


int main(int argc, char **argv){
    ros::init(argc,argv, "agent");
    sub_class A;
    ros::Time::init();
    float begin_time =ros::Time::now().toSec();
    
    while(ros::ok()){
        float clustering_time = ros::Time::now().toSec () - begin_time ;
        
        A.publish_(clustering_time);
        ros::spinOnce();
     
    }

}
