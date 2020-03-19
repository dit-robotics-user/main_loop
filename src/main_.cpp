//20200319 apdate main v5 
#include "ros/ros.h"
#include <iostream>
#include "std_msgs/String.h"
#include <std_msgs/Int32MultiArray.h>

#include "main_loop/path.h"
#include "main_loop/agent.h"
#include <main_loop/goap.h>
#include "main_loop/path_planning.h"

#include <cstdlib>

using namespace std;

int my_pos_x_ = 200 ;
int my_pos_y_ = 200 ;


class sub_state{
	public:
		void callback(const main_loop::agent::ConstPtr& msg);
		void change_value();
		sub_state();
		~sub_state(){};
		main_loop::agent pub_to_goap ;
	private:
		ros::NodeHandle n ;
		ros::Subscriber ST1_sub ; 
};
sub_state::sub_state(){
	ST1_sub = n.subscribe<main_loop::agent>("agent_msg", 1, &sub_state::callback,this);
}
void sub_state::callback(const main_loop::agent::ConstPtr& msg){
	pub_to_goap.my_pos_x = msg->my_pos_x ; 
	pub_to_goap.my_pos_y = msg->my_pos_y ;

}

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

int main(int argc, char **argv)
{

	//ros setting
 	ros::init (argc, argv, "main_");
	ros::NodeHandle nh;  
	ros::Publisher pub = nh.advertise<std_msgs::Int32MultiArray>("txST1", 1);
  	ros::ServiceClient goap = nh.serviceClient<main_loop::goap>("goap");
	ros::ServiceClient path_plan = nh.serviceClient<main_loop::path>("path_plan");
	sub_state A ; 

	//loop parameter setting
	int r0;
    int r1;
    int r2;
    int r3;
	
	while(ros::ok()){
		//calculate time
		double begin_time =ros::Time::now().toSec();
		
		//start main
		state current_state;//<--------get undergoing, finish, my_x, my_y, block from other nodes
		state action_state;
		/*
		action_state = current_state;//----->goap subscribe action state
		*/
		action act;//<----------get from goap
		float return_degree;


		//message overwrite 
		

	

		
		/////goap service 
		/*
		main_loop::agent goap_srv;
		if (goap.call(goap_srv)){
		}
		*/


		//path plan service 
		main_loop::path srv;
		srv.request.my_pos_x = A.pub_to_goap.my_pos_x ;
		srv.request.my_pos_y = A.pub_to_goap.my_pos_y ;
		srv.request.enemy1_x = 1000 ;
		srv.request.enemy1_y = 500 ;
		srv.request.enemy2_x = 500 ;
		srv.request.enemy2_y = 1750 ;
		srv.request.ally_x = 1800 ;
		srv.request.ally_y = 1400 ;   
		srv.request.goal_pos_x = 1600;//--->from goap
		srv.request.goal_pos_y = 1500;//-->from goap


		if (path_plan.call(srv)){
			double clustering_time = ros::Time::now().toSec () - begin_time; 
			ROS_INFO ("%f secs for path plan .", clustering_time);
			ROS_INFO("next_pos_x: %ld", (long int)srv.response.next_pos_x);
			ROS_INFO("next_pos_y: %ld", (long int)srv.response.next_pos_y);
			return_degree = srv.response.degree ; 
		}
		else{
			ROS_ERROR("Failed to call service path plan");
		}

		//return
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
			int distance_square = (current_state.MyPosX() - desire_pos_x)*(current_state.MyPosX() - desire_pos_x) + (current_state.MyPosY() - desire_pos_y)*(current_state.MyPosY() - desire_pos_y);
			if(distance_square < switch_mode_distance){
				r0 = 0x4000;
				r1 = desire_pos_x;
				r2 = desire_pos_y;
				r3 = desire_angle;
				//return pos_mode; //<----------------
			}
			else{
				r0 = 0x3000;
				r1 = desire_speed;
				r2 = return_degree;//<-----------------
				r3 = 0;
				//return speed_mode;//<-----------------
			}
			break;
		}

		//pub_msg 
		std_msgs::Int32MultiArray msg_ ;
		msg_.data.push_back(r0);
		msg_.data.push_back(r1);
		msg_.data.push_back(r2);
		msg_.data.push_back(r3);

		pub.publish(msg_);

		ros::spinOnce();
	}
	return 0;
}
