#include "ros/ros.h"

#include "std_msgs/String.h"
#include <std_msgs/Int32MultiArray.h>

#include "main_loop/path.h"
#include "main_loop/agent.h"
#include "main_loop/path_planning.h"
#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
#include <stdlib.h>


int my_pos_x_ = 200 ;
int my_pos_y_ = 200 ;

void callback(const main_loop::agent::ConstPtr& msg)
{
  my_pos_x_ = msg->my_pos_x ;
  my_pos_y_ = msg->my_pos_y ;
  ROS_INFO("my_pos_x in main: %d", my_pos_x_);
  ROS_INFO("my_pos_y in main: %d", my_pos_y_);
/*
  ROS_INFO("rx[0] in main: %d", msg->task_state);
  ROS_INFO("rx[1] in main: %d", msg->ally_x);
  ROS_INFO("rx[2] in main: %d", msg->ally_y);
  */
    for(int j=0;j<8;j++){
        ROS_INFO("lidar[%d] in main: %d",j, msg->emergency[j]);
    }

}


int main(int argc, char **argv)
{
  ros::init (argc, argv, "main_");

  ros::NodeHandle n;
  
  ros::ServiceClient client = n.serviceClient<main_loop::path>("path_plan");

  //test v1
  ros::Subscriber sub = n.subscribe("agent_msg", 1, callback);
  ros::Publisher pub = n.advertise<std_msgs::Int32MultiArray>("txST1", 1);
  ros::Publisher pub_2 = n.advertise<std_msgs::Int32MultiArray>("txST2", 1);
  
  
  //change mode 
  float distance_square ; 
  float switch_mode_distance = 150 ; 
  float last_degree = 0 ;
  float now_degree = 0 ;
  
  
  

  while(ros::ok()){
    
    double begin_time =ros::Time::now().toSec();
    main_loop::path srv;
    srv.request.my_pos_x = my_pos_x_ ;
    srv.request.my_pos_y = my_pos_y_ ;

    srv.request.enemy1_x = 1600 ;
    srv.request.enemy1_y = 2400 ;
    srv.request.enemy2_x = 800 ;
    srv.request.enemy2_y = 1500 ;
    srv.request.ally_x = 1400 ;
    srv.request.ally_y = 1800 ; 
     
    srv.request.goal_pos_x = 1600;
    srv.request.goal_pos_y = 2400;

    if (client.call(srv))
    {
    double clustering_time = ros::Time::now().toSec () - begin_time; 
    ROS_INFO ("%f secs for path plan .", clustering_time);
    ROS_INFO("next_pos_x: %ld", (long int)srv.response.next_pos_x);
    ROS_INFO("next_pos_y: %ld", (long int)srv.response.next_pos_y);
    
    now_degree = srv.response.degree;
   
    }
    else
    {
    ROS_ERROR("Failed to call service path plan");
    }
    
    distance_square = (srv.request.my_pos_x - srv.request.goal_pos_x)*(srv.request.my_pos_x - srv.request.goal_pos_x) + (srv.request.my_pos_y - srv.request.goal_pos_y)*(srv.request.my_pos_y - srv.request.goal_pos_y);
    
    if(now_degree<0){
        now_degree = last_degree;
    }
    ROS_INFO("next_degree: %ld", (long int)now_degree);
    std_msgs::Int32MultiArray msg_ ;
    if(distance_square < switch_mode_distance){
        msg_.data.push_back(0x4000);
        msg_.data.push_back(srv.response.next_pos_x);
        msg_.data.push_back(srv.response.next_pos_y);
        msg_.data.push_back(90);
    }else{
        msg_.data.push_back(0x3000);
        msg_.data.push_back(200);
        msg_.data.push_back(now_degree);
        msg_.data.push_back(0);
    
    }
  
    ROS_INFO("mode: %lu", msg_.data[0]);

    
    std_msgs::Int32MultiArray msg_2 ;
    msg_2.data.push_back(1);
    msg_2.data.push_back(0);
    msg_2.data.push_back(0);
    msg_2.data.push_back(0);    
    
    pub.publish(msg_);
    pub_2.publish(msg_2);
    last_degree = now_degree ; 

    ros::spinOnce();
  }
  return 0;
}
