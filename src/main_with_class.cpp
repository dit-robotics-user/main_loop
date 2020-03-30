//20200328 apdate main  
#include "ros/ros.h"

#include "std_msgs/String.h"
#include <std_msgs/Int32MultiArray.h>

#include "main_loop/path.h"
#include "main_loop/agent.h"
#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
#include <stdlib.h>

int my_pos_x_ = 300 ;
int my_pos_y_ = 300 ;


class sub_state{
	public:
		void callback(const main_loop::agent::ConstPtr& msg);
		void change_value_my_pos_x(int &my_pos_x);
		void change_value_my_pos_y(int &my_pos_y);
		main_loop::path srv_to_path;
		

		sub_state();
		~sub_state(){};
		
	private:
		ros::NodeHandle n ;
		ros::Subscriber ST1_sub ;
		main_loop::agent pub_to_goap ; 
		 
};

sub_state::sub_state(){
	ST1_sub = n.subscribe<main_loop::agent>("agent_msg", 1, &sub_state::callback,this);
	    for(int j=0;j<8;j++){
        pub_to_goap.emergency.push_back(0);
    }
    pub_to_goap.my_pos_x = 400 ;
    pub_to_goap.my_pos_y = 400 ;
}

void sub_state::callback(const main_loop::agent::ConstPtr& msg){
  srv_to_path.request.my_pos_x = msg->my_pos_x ;
  srv_to_path.request.my_pos_y = msg->my_pos_y ;
  ROS_INFO("my_pos_x in main_with_class: %d", pub_to_goap.my_pos_x);
  ROS_INFO("my_pos_y in main_with_class: %d", pub_to_goap.my_pos_y);
/*
  ROS_INFO("rx[0] in main: %d", msg->task_state);
  ROS_INFO("rx[1] in main: %d", msg->ally_x);
  ROS_INFO("rx[2] in main: %d", msg->ally_y);
*/
  pub_to_goap={};

    //    pub_to_goap.emergency.push_back(msg->emergency[j]);
}


int main(int argc, char **argv)
{        

	//ros setting
 	ros::init (argc, argv, "main_with_class");
 
	sub_state A;
	main_loop::path B ; 
	ros::NodeHandle nh;    
	

    ros::Publisher pub = nh.advertise<std_msgs::Int32MultiArray>("txST1", 1);
	ros::Publisher pub_2 = nh.advertise<std_msgs::Int32MultiArray>("txST2", 1);
	ros::ServiceClient client = nh.serviceClient<main_loop::path>("path_plan");
	main_loop::path srv;
	B.request.my_pos_x = my_pos_x_ ;
    B.request.my_pos_y = my_pos_y_ ;
   

	//loop parameter setting
	int r0;
    int r1;
    int r2;
    int r3;
    int now_my_pos_x;
    int now_my_pos_y;
    
      //change mode 
      float distance_square ; 
      float switch_mode_distance = 350 ; 
      float last_degree = 0 ;
      float now_degree = 0 ;
      float return_degree = 0 ;
	
	while(ros::ok()){
	 double begin_time =ros::Time::now().toSec();
		//calculate time
		

		//path plan service 
		
        B.request.my_pos_x = A.srv_to_path.request.my_pos_x;
        B.request.my_pos_y = A.srv_to_path.request.my_pos_y ;
        ROS_INFO("B.request.my_pos_x: %d", B.request.my_pos_x);
    ROS_INFO("B.request.my_pos_y: %d", B.request.my_pos_y);
        B.request.enemy1_x = 1600 ;
        B.request.enemy1_y = 2400 ;
        B.request.enemy2_x = 800 ;
        B.request.enemy2_y = 1500 ;
        B.request.ally_x = 1400 ;
        B.request.ally_y = 1800 ; 
         
        B.request.goal_pos_x = 1600;
        B.request.goal_pos_y = 2400;



		if (client.call(B)){
			double clustering_time = ros::Time::now().toSec () - begin_time; 
			ROS_INFO ("%f secs for path plan .", clustering_time);
			ROS_INFO("next_pos_x: %ld", (long int)B.response.next_pos_x);
			ROS_INFO("next_pos_y: %ld", (long int)B.response.next_pos_y);
			return_degree = B.response.degree ; 
		}
		else{
			ROS_ERROR("Failed to call service path plan");
		}
        distance_square = (B.request.my_pos_x - B.request.goal_pos_x)*(B.request.my_pos_x - B.request.goal_pos_x) + (B.request.my_pos_y - B.request.goal_pos_y)*(B.request.my_pos_y - B.request.goal_pos_y);
    
    if(now_degree<0){
        now_degree = last_degree;
    }
    ROS_INFO("next_degree: %ld", (long int)now_degree);



    //publish to serial node 
    std_msgs::Int32MultiArray msg_ ;
    if(distance_square < switch_mode_distance){
        msg_.data.push_back(0x4000);
        msg_.data.push_back(B.response.next_pos_x);
        msg_.data.push_back(B.response.next_pos_y);
        msg_.data.push_back(90);
    }else{
        msg_.data.push_back(0x3000);
        msg_.data.push_back(200);
        msg_.data.push_back(now_degree);
        msg_.data.push_back(0);
    
    }   
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
