#include "ros/ros.h"
#include "std_msgs/String.h"
#include <std_msgs/Int32.h>
#include <std_msgs/Int32MultiArray.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <stdlib.h>
/*enum class Action {
	hand1_out_put = 0x10;
	hand1_out_suck = 0x11;
	hand1_in = 0x12;
	hand1_origin = 0x13;
	hand1_finish = 0x1A;
	hand2_out_put = 0x20;
	hand2_out_suck = 0x21;
	hand2_in = 0x22;
	hand2_origin = 0x23;
	hand2_finish = 0x2A;
	hand3_out_put = 0x30;
	hand3_out_suck = 0x31;
	hand3_in = 0x32;
	hand3_origin = 0x33;
	hand3_finish = 0x3A;
	hand4_out_put = 0x40;
	hand4_out_suck = 0x41;
	hand4_in = 0x42;
	hand4_origin = 0x43;
	hand4_finish = 0x4A;
	fix1_off = 0x50;
	fix1_on = 0x51;
	fix1_finish = 0x5A;
	fix2_off = 0x60;
	fix2_on = 0x61;
	fix2_finish = 0x6A;
	fix3_off = 0x70;
	fix3_on = 0x71;
	fix3_finish = 0x7A;
	fix4_off = 0x80;
	fix4_on = 0x81;
	fix4_finish = 0x8A;
	suck1_off = 0x90;
	suck1_on = 0x91;
	suck1_fail = 0x92;
	suck1_finish = 0x9A;
	suck2_off = 0xA0;
	suck2_on = 0xA1;
	suck2_fail = 0xA2;
	suck2_finish = 0xAA;
	suck3_off = 0xB0;
	suck3_on = 0xB1;
	suck3_fail = 0xB2;
	suck3_finish = 0xBA;
	suck4_off = 0xC0;
	suck4_on = 0xC1;
	suck4_fail = 0xC2;
	suck4_finish = 0xCA;
	stepper_low = 0xD0;
	stepper_high = 0xD1;
	stepper_finish = 0xD3;
	flag_out = 0xE0;
	flag_finish = 0xE1;
};*/
class sub_class{
    public:
        void ST1_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg);
        void ST2_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg); 
        //sub_class(int my_pos_x_ = 700, int my_pos_y_ = 300, int ini_status = 0);
        ros::NodeHandle n;

	    ros::Subscriber ST1_sub = n.subscribe<std_msgs::Int32MultiArray>("rxST1", 1, &sub_class::ST1_sub_callback,this);
        ros::Subscriber ST2_sub = n.subscribe<std_msgs::Int32MultiArray>("rxST2", 1, &sub_class::ST2_sub_callback,this);
        ros::Publisher pub_st1 = n.advertise<std_msgs::Int32MultiArray>("txST1", 1);
		ros::Publisher pub_st2 = n.advertise<std_msgs::Int32MultiArray>("txST2", 1);

		ros::ServiceClient client_path = n.serviceClient<main_loop::path>("path_plan");
	    ros::ServiceClient client_goap = n.serviceClient<main_loop::goap_>("goap_test_v1");

		~sub_class(){};
};

int hand=0;
int suck=0; 
int stepper=0;
int flag=0;  

int rx0;
int rx1;
int rx2;
int rx3;

int current_action[4] = {hand, suck, stepper, flag};
int desire_action[4] = {rx0, rx1, rx2, rx3};

int my_pos_x=0;
int my_pos_y=0;
int my_degree=0;

int switch_mode_distance = 4000000;//square
int distance_square = 0;

void sub_class::ST1_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg){
    my_pos_x = msg->data[0] ;
    my_pos_y = msg->data[1] ;
    my_degree = msg->data[2] ;
}

void sub_class::ST2_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg){
    hand = msg->data[0] ;
    suck = msg->data[1] ; 
    stepper = msg->data[2] ;
    flag = msg->data[3] ;   
}

bool command_finish(){
	int count = 0;
	int k = 0;
	for(k=0; k<4; k++){
		if (current_action[k] == desire_action[k]){
			count++;
		}	
	}
	if(count == 4){
		return true;
	}
	else{
		return false;
	}
}

int main(int argc, char **argv){

	ros::init(argc, argv, "main_frank");

	sub_class sub;

    main_loop::path path_srv;
    main_loop::goap_ goap_srv;

	while(ros::ok()){

		sub.n.getParam("main_frank/rx0", rx0);
		sub.n.getParam("main_frank/rx1", rx1);
		sub.n.getParam("main_frank/rx2", rx2);
		sub.n.getParam("main_frank/rx3", rx3);

		desire_action[0] = rx0;
		desire_action[1] = rx1;
		desire_action[2] = rx2;
		desire_action[3] = rx3;

		current_action[0] = hand;
		current_action[1] = suck;
		current_action[2] = stepper;
		current_action[3] = flag;

		ROS_INFO("desire:");
		int i=0;
		for(i=0; i<4; i++){
			ROS_INFO("%d ", desire_action[i]);
		}

		ROS_INFO("current:");
		int j=0;
		for(j=0; j<4; j++){
			ROS_INFO("%d ", current_action[j]);
		}

		if(command_finish()){
			ROS_INFO("finish");
		}
		else{
			ROS_INFO("ing or fail");
		}

//-------------------------------------------------------------------------path

		path_srv.request.my_pos_x = my_pos_x;
        path_srv.request.my_pos_y = my_pos_y;

        path_srv.request.goal_pos_x = goap_srv.response.pos[0]; //測試時用輸入的
        path_srv.request.goal_pos_y = goap_srv.response.pos[1];

        path_srv.request.enemy1_x = 380 ;
        path_srv.request.enemy1_y = 2400 ;
        path_srv.request.enemy2_x = 380 ;
        path_srv.request.enemy2_y = 2300 ;
        path_srv.request.ally_x = 380 ;
        path_srv.request.ally_y = 2200 ;

        sub.n.getParam("main_frank/goal_pos_x", path_srv.request.goal_pos_x);
		sub.n.getParam("main_frank/goal_pos_y", path_srv.request.goal_pos_y);
		sub.n.getParam("main_frank/enemy1_x", path_srv.request.enemy1_x);
		sub.n.getParam("main_frank/enemy1_y", path_srv.request.enemy1_y);
		sub.n.getParam("main_frank/enemy2_x", path_srv.request.enemy2_x);
		sub.n.getParam("main_frank/enemy2_y", path_srv.request.enemy2_y);
		sub.n.getParam("main_frank/ally_x", path_srv.request.ally_x);
		sub.n.getParam("main_frank/ally_y", path_srv.request.ally_y);

	    /*std_msgs::Int32MultiArray for_st1 ;
	    for_st1.data.push_back(r0);
	    for_st1.data.push_back(r1);
	    for_st1.data.push_back(r2);
	    for_st1.data.push_back(r3); */

	    if(sub.client_path.call(path_srv)){
            ROS_INFO("%d", path_srv.request.goal_pos_x);
            ROS_INFO("%d", path_srv.request.goal_pos_y);
            ROS_INFO("%d", path_srv.response.degree);
        }
        else{
            ROS_ERROR("Failed to call path_plan");
        }

 		distance_square = (my_pos_x - path_srv.request.goal_pos_x)*(my_pos_x - path_srv.request.goal_pos_x) + (my_pos_y - path_srv.request.goal_pos_y)*(my_pos_y - path_srv.request.goal_pos_y);
	    
	    //switch mode
	    std_msgs::Int32MultiArray for_st1 ;
        if(distance_square < switch_mode_distance){
            for_st1.data.push_back(0x4000);                         //pos mode
            for_st1.data.push_back(path_srv.request.goal_pos_x);
            for_st1.data.push_back(path_srv.request.goal_pos_y);
            for_st1.data.push_back(90);						        // ?
        }
        else{
            for_st1.data.push_back(0x3000);                         //speed mode
            for_st1.data.push_back(200);							//speed
            for_st1.data.push_back(path_srv.response.degree);	    //degree
            for_st1.data.push_back(0);								// ?
        }

	    std_msgs::Int32MultiArray for_st2 ;
	    for_st2.data.push_back(rx0);
	    for_st2.data.push_back(rx1);
	    for_st2.data.push_back(rx2);
	    for_st2.data.push_back(rx3);

	    sub.pub_st1.publish(for_st1);
	    sub.pub_st2.publish(for_st2);  //sub.要加

	    ros::spinOnce(); //才會callback
	}
	return 0;
}