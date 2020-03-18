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
		sub_state();
		~sub_state(){};
	private:
		ros::NodeHandle n ;
		ros::Subscriber ST1_sub ; 
		main_loop::agent sub_from_agent ;
};
sub_state::sub_state(){
	ST1_sub = n.subscribe<main_loop::agent>("agent_msg", 1, &sub_state::callback,this);
}
void sub_state::callback(const main_loop::agent::ConstPtr& msg){
	sub_from_agent.my_pos_x = msg->my_pos_x ; 
	sub_from_agent.my_pos_y = msg->my_pos_y ;

}

class state{
public:
    state(bool under, bool fin, int my_x, int my_y, bool block);
	~state(){};
    void KillMissionTrue(){
        kill_mission = true;
    }
    void ReplanMissionTrue(){
        replan_mission = true;
    }
    void UndergoingTrue(){
        undergoing = true;
    }
    void FinishedTrue(){
        finished = true;
    }
    void ActionWaitTrue(){
        action_wait = true;
    }
    void ReplanPathTrue(){
        replan_path = true;
    }
    void StateChangeTrue(){
        state_change = true;
    }
    void KillMissionFalse(){
        kill_mission = false;
    }
    void ReplanMissionFalse(){
        replan_mission = false;
    }
    void UndergoingStateFalse(){
        undergoing = false;
    }
    void FinishedFalse(){
        finished = false;
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
    bool Undergoing(){
        return undergoing;
    }
    bool Finished(){
        return finished;
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
    bool StateChange(){
        return state_change;
    }
    void RefreshActionState(){
        undergoing = false;
        finished = false;
        kill_mission = false;
        replan_mission = false;
        replan_path = false;
        is_blocked = false;
        action_wait = false;
        state_change = false;
    }

private:
    bool undergoing;
    bool finished;
    bool kill_mission;
    bool replan_mission;
    bool replan_path;
    int my_pos_x;
    int my_pos_y;
    bool is_blocked;
    bool action_wait;
    bool state_change;
};
state::state(bool under, bool fin, int my_x, int my_y, bool block){
	undergoing = under;
	finished = fin;
	my_pos_x = my_x;
	my_pos_y = my_y;
	is_blocked = block; 
	
}

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
    ~action(){};
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
 	ros::init (argc, argv, "main_");
	ros::NodeHandle nh;  
	ros::Publisher pub = nh.advertise<std_msgs::Int32MultiArray>("txST1", 1);
  	ros::ServiceClient goap = nh.serviceClient<main_loop::goap>("goap");
	ros::ServiceClient path_plan = nh.serviceClient<main_loop::path>("path_plan");

	main_loop::goap goap_srv ;
	int r0;
    int r1;
    int r2;
    int r3;
	state current_state(bool under, bool fin, int my_x, int my_y, bool block);//<--------get undergoing, finish, my_x, my_y, block from other nodes
    state action_state(int x, int y, int movement_num, double what_angle, int how_fast, bool is_wait, int serial);
 //   action_state = current_state;//----->goap subscribe action state
	


	//test v1

	while(ros::ok()){
		ROS_INFO("333 ");
		double begin_time =ros::Time::now().toSec();

	
		if (goap.call(goap_srv)){


		}









		main_loop::path srv;
		srv.request.my_pos_x = my_pos_x_ ;
		srv.request.my_pos_y = my_pos_y_ ;
		/*
		srv.request.enemy1_x = 1000 ;
		srv.request.enemy1_y = 500 ;
		srv.request.enemy2_x = 500 ;
		srv.request.enemy2_y = 1750 ;
		srv.request.ally_x = 1800 ;
		srv.request.ally_y = 1400 ; 
		*/  
		srv.request.goal_pos_x = 1600;
		srv.request.goal_pos_y = 1500;

		if (path_plan.call(srv)){
			double clustering_time = ros::Time::now().toSec () - begin_time; 
			ROS_INFO ("%f secs for path plan .", clustering_time);
			ROS_INFO("next_pos_x: %ld", (long int)srv.response.next_pos_x);
			ROS_INFO("next_pos_y: %ld", (long int)srv.response.next_pos_y);
		}
		else{
			ROS_ERROR("Failed to call service path plan");
		}

		std_msgs::Int32MultiArray msg_ ;
		msg_.data.push_back(0x4000);
		msg_.data.push_back(srv.response.next_pos_x);
		msg_.data.push_back(srv.response.next_pos_y);
		msg_.data.push_back(90);

		pub.publish(msg_);

		ros::spinOnce();
	}
	return 0;
}
