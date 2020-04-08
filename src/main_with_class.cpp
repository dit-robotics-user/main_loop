//20200328 apdate main  
#include "ros/ros.h"

#include "std_msgs/String.h"
#include <std_msgs/Int32MultiArray.h>

#include "main_loop/path.h"
#include "main_loop/agent.h"
#include "main_loop/goap_.h"
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
		bool emergency[8];
        int status;
        int task_state;
        int robot_degree;
        bool lidar_be_blocked(float speed_degree,float car_degree);
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
    robot_degree = msg->my_degree ; 
    task_state = msg->task_state;
    status = msg->status;
    ROS_INFO("my_pos_x in main_with_class: %d", pub_to_goap.my_pos_x);
    ROS_INFO("my_pos_y in main_with_class: %d", pub_to_goap.my_pos_y);
    /*
	ROS_INFO("lidar[0]: %d", msg->emergency[0]);
	emergency[0]=msg->emergency[0];
    emergency[1]=msg->emergency[1];
    emergency[2]=msg->emergency[2];
    emergency[3]=msg->emergency[3];
    emergency[4]=msg->emergency[4];
    emergency[5]=msg->emergency[5];
    emergency[6]=msg->emergency[6];
    emergency[7]=msg->emergency[7];
    */	
}

bool sub_state::lidar_be_blocked(float speed_degree,float car_degree){
    if(car_degree>speed_degree){
        if(car_degree-speed_degree<90){
            if( emergency[1]==false || emergency[2]==false || emergency[3]==false ){
                return false ;
            }else{
                return true ;
            }
        }else{
            if( emergency[4]==false || emergency[5]==false || emergency[6]==false ){
                return false ;
            }else{
                return true ;
            }            
        }
    }
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
    ros::ServiceClient client_goap = nh.serviceClient<main_loop::goap_>("goap_test_v1");
	main_loop::path srv;
    main_loop::goap_ goap_srv;
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

        goap_srv.request.replan=0;
        goap_srv.request.action_done=0;
        goap_srv.request.pos.push_back(A.srv_to_path.request.my_pos_x);
        goap_srv.request.pos.push_back(A.srv_to_path.request.my_pos_y);
        goap_srv.request.my_degree = A.robot_degree ; 

        if (client_goap.call(goap_srv)){
            ROS_INFO("test_goap: %ld", (long int)goap_srv.response.degree);
            ROS_INFO("test_goap: %ld", (long int)goap_srv.response.speed);
            ROS_INFO("test_goap: %ld", (long int)goap_srv.response.mode);
            ROS_INFO("test_goap: %ld", (long int)goap_srv.response.pos[0]);
        }else{
            ROS_ERROR("Failed to call goap_test");
        }



		if (client.call(B)){
			double clustering_time = ros::Time::now().toSec () - begin_time; 
			ROS_INFO ("%f secs for path plan .", clustering_time);
			ROS_INFO("next_pos_x: %ld", (long int)B.response.next_pos_x);
			ROS_INFO("next_pos_y: %ld", (long int)B.response.next_pos_y);
			return_degree = B.response.degree ; 
		}else{
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
        msg_2.data.push_back(2);
        msg_2.data.push_back(1);
        msg_2.data.push_back(31);

        pub.publish(msg_);
        pub_2.publish(msg_2);
        last_degree = now_degree ; 


        ros::spinOnce();
	}
	return 0;
}
