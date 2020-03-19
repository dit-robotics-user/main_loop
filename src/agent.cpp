#include "ros/ros.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "std_msgs/String.h"
#include "std_msgs/Header.h"
#include "main_loop/string_with_header.h"
#include "main_loop/path_planning.h"
#include "main_loop/agent.h"
#include <std_msgs/Int32MultiArray.h>
#include "lidar_2020/alert_range.h"



class sub_class{
    public:
        void ST1_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg);
        void ST2_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg); 
        void lidarmsg_sub_callback(const lidar_2020::alert_range::ConstPtr& msg);
        void publish_(float time);
        sub_class(int my_pos_x_ = 250, int my_pos_y_ = 250);
        ~sub_class(){};

    private:
        ros::NodeHandle n;
		ros::Publisher agent_pub= n.advertise<main_loop::agent>("agent_msg", 1);
		ros::Subscriber ST1_sub = n.subscribe<std_msgs::Int32MultiArray>("rxST1", 1, &sub_class::ST1_sub_callback,this);
        ros::Subscriber ST2_sub = n.subscribe<std_msgs::Int32MultiArray>("rxST2", 1, &sub_class::ST2_sub_callback,this);
        ros::Subscriber lidarmsg_sub= n.subscribe<lidar_2020::alert_range>("range_alert", 1, &sub_class::lidarmsg_sub_callback,this);
        main_loop::agent pub_to_main;

};

sub_class::sub_class(int my_pos_x_,int my_pos_y_){
    pub_to_main.my_pos_x = my_pos_x_ ; 
    pub_to_main.my_pos_y = my_pos_y_ ; 
  
}

void sub_class::ST1_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg){
    
    pub_to_main.my_pos_x = msg->data[0] ;
    pub_to_main.my_pos_y = msg->data[1] ;
    ROS_INFO("my_pos_x: %d", pub_to_main.my_pos_x);
    ROS_INFO("my_pos_y: %d", pub_to_main.my_pos_y);
}
void sub_class::ST2_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg){
    
    pub_to_main.my_pos_x = msg->data[0] ;
    pub_to_main.my_pos_y = msg->data[1] ;
    ROS_INFO("task_state_1: %d", pub_to_main.my_pos_x);
    ROS_INFO("task_state_2: %d", pub_to_main.my_pos_y);
}
void sub_class::lidarmsg_sub_callback(const lidar_2020::alert_range::ConstPtr& msg){

    pub_to_main.emergency = msg->alert ; 
}
void sub_class::publish_(float time ){
    pub_to_main.time =time ; 
    agent_pub.publish(pub_to_main);
}


int main(int argc, char **argv){
    ros::init(argc,argv, "simple_octagonal_ranging");
    sub_class A;
    ros::Time::init();
    float begin_time =ros::Time::now().toSec();
    ros::Rate loop_rate(50);
    while(ros::ok()){
        float clustering_time = ros::Time::now().toSec () - begin_time ;
        A.publish_(clustering_time);

        ros::spinOnce();
        loop_rate.sleep();
    }

}
