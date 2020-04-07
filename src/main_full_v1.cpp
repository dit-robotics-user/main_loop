//20200406 apdate main  //main old
#include "ros/ros.h"

#include "std_msgs/String.h"
#include <std_msgs/Int32MultiArray.h>
#include <main_loop/AddTwoInts.h>  //test_goap
#include "main_loop/goap.h"
#include "main_loop/path.h"
#include "main_loop/agent.h"
#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
#include <stdlib.h>

using namespace std;
int my_pos_x_ = 300 ;
int my_pos_y_ = 300 ;

class state{
public:
    state(){};
    state(int action_status, int my_x, int my_y, bool block){
        status = action_status;
        my_pos_x = my_x;
        my_pos_y = my_y;
        is_blocked = block;
    }
    void ChangeActionStatus(int state){
        if(state != 0 || state != 1 || state != 2){
            status = 0;
        }
        else{
            status = state;
        }
        //0 = nothing going on
        //1 = undergoing
        //2 = finished
    }
    void KillMissionTrue(){
        kill_mission = true;
    }
    void ReplanMissionTrue(){
        replan_mission = true;
    }
    void ActionWaitTrue(){
        action_wait = true;
    }
    void ReplanPathTrue(){
        replan_path = true;
    }
    void KillMissionFalse(){
        kill_mission = false;
    }
    void ReplanMissionFalse(){
        replan_mission = false;
    }
    void ActionWaitFalse(){
        action_wait = false;
    }
    void ReplanPathFalse(){
        replan_path = false;
    }
    void StateChageFalse(){
        state_change = false;
    }
    int MyActionStatus(){
        return status;
    }
    int MyPosX(){
        return my_pos_x;
    }
    int MyPosY(){
        return my_pos_y;
    }
    bool KillMission(){
        return kill_mission;
    }
    bool ReplanMission(){
        return replan_mission;
    }
    bool IsBlocked(){
        return is_blocked;
    }
    bool ActionWait(){
        return action_wait;
    }
    bool ReplanPath(){
        return replan_path;
    }
    void RefreshActionState(){
        status = 0;
        kill_mission = false;
        replan_mission = false;
        replan_path = false;
        is_blocked = false;
        action_wait = false;
    }

private:
    int status;
    bool kill_mission;
    bool replan_mission;
    bool replan_path;
    int my_pos_x;
    int my_pos_y;
    bool is_blocked;
    bool action_wait;
    bool state_change;
};

class action{
public:
    action(int x, int y, int movement_num, double what_angle, int how_fast, bool is_wait, int serial){
        goal_pos_x = x;
        goal_pos_y = y;
        movement = movement_num;
        angle = what_angle;
        speed = how_fast;
        wait = is_wait;
        serial_num = serial;
    }
    action(){};
    int PosX(){
        return goal_pos_x;
    }
    int PosY(){
        return goal_pos_y;
    }
    int Movement(){
        return movement;
    }
    int Speed(){
        return speed;
    }
    bool Wait(){
        return wait;
    }
    double Angle(){
        return angle;
    }
    int SerialNum(){
        return serial_num;
    }
private:
    int goal_pos_x;
    int goal_pos_y;
    int movement;
    int speed;
    double angle;
    bool wait;
    int serial_num;
};

enum class RobotState {AT_POS, ON_THE_WAY, BLOCKED};

class sub_state{
	public:
		void callback(const main_loop::agent::ConstPtr& msg);
		void change_value_my_pos_x(int &my_pos_x);
		void change_value_my_pos_y(int &my_pos_y);
		main_loop::path srv_to_path;
        int status;
        int robot_degree;
		

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
    status = msg->task_state;
    ROS_INFO("my_pos_x in main_with_class: %d", pub_to_goap.my_pos_x);
    ROS_INFO("my_pos_y in main_with_class: %d", pub_to_goap.my_pos_y);


    //pub_to_goap.emergency.push_back(msg->emergency[j]);
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
    ros::ServiceClient client_goap = nh.serviceClient<main_loop::goap>("goap_test_v1");
    main_loop::AddTwoInts srv_1; //test
    main_loop::goap goap_srv;
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
    int goal_covered_counter = 0;
    int cover_limit = 20;
    
    //change mode 

    float switch_mode_distance = 350 ; 
    float last_degree = 0 ;
    float now_degree = 0 ;
    float return_degree = 0 ;

	while(ros::ok()){
        state current_state(A.status,A.srv_to_path.request.my_pos_x,A.srv_to_path.request.my_pos_y,0);//<--------get undergoing, finish, my_x, my_y, block from other nodes
        state action_state;
        if(action_state.MyActionStatus() == 2){
            goal_covered_counter = 0;
        }
        action_state = current_state;//----->goap subscribe action state





	    double begin_time =ros::Time::now().toSec();
		//calculate time
		
        B.request.my_pos_x = A.srv_to_path.request.my_pos_x;
        B.request.my_pos_y = A.srv_to_path.request.my_pos_y ;
        B.request.enemy1_x = 1600 ;
        B.request.enemy1_y = 2400 ;
        B.request.enemy2_x = 800 ;
        B.request.enemy2_y = 1500 ;
        B.request.ally_x = 1400 ;
        B.request.ally_y = 1800 ; 
        B.request.goal_pos_x = 1600;
        B.request.goal_pos_y = 2400;

        srv_1.request.a = 2;
        srv_1.request.b = 3;
        goap_srv.request.replan=0;
        goap_srv.request.action_done=0;
        goap_srv.request.pos.push_back(A.srv_to_path.request.my_pos_x);
        goap_srv.request.pos.push_back(A.srv_to_path.request.my_pos_y);
        goap_srv.request.my_degree = A.robot_degree ; 



        if (client_goap.call(goap_srv)){
            ROS_INFO("test_goap: %ld", (long int)goap_srv.response.degree);
        }else{
            ROS_ERROR("Failed to call goap_test");
        }


        //////////////////////////////////////////////////




        action act;//<----------get from goap
        int switch_mode_distance = 100;
        int desire_pos_x = act.PosX();
        int desire_pos_y = act.PosY();
        int desire_movement = act.Movement();
        double desire_angle = act.Angle();
        int desire_speed = act.Speed();
        bool desire_wait = act.Wait();
        action_state.RefreshActionState();

        bool at_desire_pos;
        RobotState robot;
        if(current_state.MyPosX() == desire_pos_x && current_state.MyPosY() == desire_pos_y){
            at_desire_pos = true;
        }
        else{
            at_desire_pos = false;
        }
        if(at_desire_pos == true){
            robot = RobotState::AT_POS;
        }
        else{
            if(current_state.IsBlocked()){
                robot = RobotState::BLOCKED;
            }
            else{
                robot = RobotState::ON_THE_WAY;
            }
        }






    std_msgs::Int32MultiArray for_st1 ;

switch(robot){
    case RobotState::AT_POS:
        if(current_state.MyActionStatus() == 0){ //nothing done
            return desire_movement; //<-----------------CHANGE
        }
        else if(current_state.MyActionStatus() == 1){
            if(desire_wait){

            }
            else{
                action_state.ChangeActionStatus(2); //finished
            }
        }
        /*else if(){/////////////////////////////////
            action_state.KillMissionTrue();
        }*/
        break;

    case RobotState::BLOCKED:
        //return stop; //<-------------tell stm to stop
        action_state.ReplanMissionTrue();
        action_state.ReplanPathTrue();
        break;

    case RobotState::ON_THE_WAY:
        if (client.call(B)){
            double clustering_time = ros::Time::now().toSec () - begin_time; 
            ROS_INFO ("%f secs for path plan .", clustering_time);
            ROS_INFO("next_pos_x: %ld", (long int)B.response.next_pos_x);
            ROS_INFO("next_pos_y: %ld", (long int)B.response.next_pos_y);
            return_degree = B.response.degree ; 
        }else{
            ROS_ERROR("Failed to call service path plan");
        }
        if(now_degree<0){
            now_degree = last_degree;
        }
        ROS_INFO("next_degree: %ld", (long int)now_degree);
        float distance_square = (B.request.my_pos_x - B.request.goal_pos_x)*(B.request.my_pos_x - B.request.goal_pos_x) + (B.request.my_pos_y - B.request.goal_pos_y)*(B.request.my_pos_y - B.request.goal_pos_y);
//        int distance_square = (current_state.MyPosX() - desire_pos_x)*(current_state.MyPosX() - desire_pos_x) + (current_state.MyPosY() - desire_pos_y)*(current_state.MyPosY() - desire_pos_y);
        if(distance_square < switch_mode_distance){
            for_st1.data.push_back(0x4000);
            for_st1.data.push_back(desire_pos_x);
            for_st1.data.push_back(desire_pos_y);
            for_st1.data.push_back(desire_angle);
            //return pos_mode; //<----------------
        }
        else{
            //r1 = desire_speed;
            //r2 = A*();//<-----------------
            //return speed_mode;//<-----------------
            for_st1.data.push_back(0x3000);
            for_st1.data.push_back(200);
            for_st1.data.push_back(now_degree);
            for_st1.data.push_back(0);
            /*--->check if enemy block 
            if( A*()cover == true){ //<-------------
                goal_covered_counter ++;
            }
            if( A*()cover == false){ //<-------------
                goal_covered_counter = 0;
            }
            if(goal_covered_counter > cover_limit){
                action_state.KillMissionTrue();
                goal_covered_counter = 0;
            }
            */
            //return speed_mode;//<-----------------
        }
        break;
    }

    ///////////////////////////////////////


    //publish to serial node 
 
    std_msgs::Int32MultiArray msg_2 ;
    msg_2.data.push_back(2);
    msg_2.data.push_back(1);
    msg_2.data.push_back(31);
    
    pub.publish(for_st1);
    pub_2.publish(msg_2);
    last_degree = now_degree ; 


		ros::spinOnce();
	}
	return 0;
}
