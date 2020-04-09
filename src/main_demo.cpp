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

//initialize
int my_pos_x_ = 300 ; 
int my_pos_y_ = 300 ;

using namespace std;

class sub_state{
	public:
    	sub_state();
		~sub_state(){};
		void callback(const main_loop::agent::ConstPtr& msg);
        bool lidar_be_blocked(float speed_degree,float car_degree);
		bool emergency[8];
        int movement_from_goap[15];
        int status;
        int task_state;
        float robot_degree;
        main_loop::path srv_to_path;
		
	private:
		ros::NodeHandle n ;
		ros::Subscriber Agent_sub ;		 
};

sub_state::sub_state(){
	Agent_sub = n.subscribe<main_loop::agent>("agent_msg", 1, &sub_state::callback,this);
	for(int j=0;j<8;j++){
        emergency[j]=true;
    }
}

void sub_state::callback(const main_loop::agent::ConstPtr& msg){
    srv_to_path.request.my_pos_x = msg->my_pos_x ;
    srv_to_path.request.my_pos_y = msg->my_pos_y ;
    robot_degree = msg->my_degree ; 
    task_state = msg->task_state;
    status = msg->status;
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
            if( emergency[1]==true || emergency[2]==true || emergency[3]==true ){
                return true ;
            }else{
                return false ;
            }
        }else{
            if( emergency[4]==true || emergency[5]==true || emergency[6]==true ){
                return true ;
            }else{
                return false ;
            }            
        }
    }
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
        status = state;
        //1 = undergoing
        //2 = finished
    }
    void ChangeKillMission(bool tf){
        kill_mission = tf;
    }
    void ChangeReplanMission(bool tf){
        replan_mission = tf;
    }
    void ChangeActionWait(bool tf){
        action_wait = tf;
    }
    void ChangeReplanPath(bool tf){
        replan_path = tf;
    }
    void ChangeStateChange(bool tf){
        state_change = tf;
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
        status = 1;
        kill_mission = false;
        replan_mission = false;
        replan_path = false;
        is_blocked = false;
        action_wait = false;
    }

private:
    int status;
    int my_pos_x;
    int my_pos_y;
    bool kill_mission;
    bool replan_mission;
    bool replan_path;
    bool is_blocked;
    bool action_wait;
    bool state_change;
};

class action{
public:
    action(int x, int y, int movement_num[], double what_angle, int how_fast, bool is_wait, int what_mode){
        goal_pos_x = x;
        goal_pos_y = y;
        for(int i = 0; i < 15; i ++){
            movement[i] = movement_num[i];
        }
        angle = what_angle;
        speed = how_fast;
        wait = is_wait;
        mode = what_mode;
    }
    action(){};
    int PosX(){
        return goal_pos_x;
    }
    int PosY(){
        return goal_pos_y;
    }
    int * Movement(){
        return  movement;
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
    int Mode(){
        return mode;
    }
private:
    int goal_pos_x;
    int goal_pos_y;
    int movement[15];
    int speed;
    double angle;
    bool wait;
    int mode;
};

enum class Mode {SPEED_MODE, POSITION_MODE};
enum class Status {SET_STRATEGY, RESET, SET_INITIAL_POS, STARTING_SCRIPT, READY, RUN, STOP, IDLE};
enum class RobotState {AT_POS, ON_THE_WAY, BLOCKED};

int main(int argc, char **argv){        

	//ros setting
 	ros::init (argc, argv, "main_with_class");
	ros::NodeHandle nh;    
	

    ros::Publisher pub_st1 = nh.advertise<std_msgs::Int32MultiArray>("txST1", 1);
	ros::Publisher pub_st2 = nh.advertise<std_msgs::Int32MultiArray>("txST2", 1);
	ros::ServiceClient client_path = nh.serviceClient<main_loop::path>("path_plan");
    ros::ServiceClient client_goap = nh.serviceClient<main_loop::goap_>("goap_test_v1");
    sub_state temp;
	main_loop::path path_srv;
    main_loop::goap_ goap_srv;
	path_srv.request.my_pos_x = my_pos_x_ ;
    path_srv.request.my_pos_y = my_pos_y_ ;
   

	//loop parameter setting
    int r0;
    int r1;
    int r2;
    int r3;
    int rx0;
    int rx1;
    int rx2;
    int rx3;
    int out = 0;
    int goal_covered_counter = 0;
    int cover_limit = 20;
    int old_grab_status[12] = {0};
    int distance_square = 0;
    int now_my_pos_x;
    int now_my_pos_y;
    

    float last_degree = 0 ;
    float now_degree = 0 ;
	
	while(ros::ok()){
        //calculate time
	    double begin_time =ros::Time::now().toSec();
        //service and topic parameter setting 
        std_msgs::Int32MultiArray for_st1 ;
        std_msgs::Int32MultiArray for_st2 ;


        int s = temp.status; //<----------get from command
        Status stat  = static_cast<Status>(s);
        bool lidar_blocked=temp.lidar_be_blocked(now_degree,temp.robot_degree);//<------------lidar

        state current_state(temp.status,temp.srv_to_path.request.my_pos_x,temp.srv_to_path.request.my_pos_y,lidar_blocked);//<--------get undergoing, finish, my_x, my_y, block from other nodes ()********
        state action_state(temp.status,temp.srv_to_path.request.my_pos_x,temp.srv_to_path.request.my_pos_y,lidar_blocked);
        if(action_state.MyActionStatus() == 2){
            goal_covered_counter = 0;
        }
        action_state = current_state;//----->goap subscribe action state, then cover it with current

        //PATH PLAN
        path_srv.request.my_pos_x = temp.srv_to_path.request.my_pos_x;
        path_srv.request.my_pos_y = temp.srv_to_path.request.my_pos_y ;
        path_srv.request.enemy1_x = 1600 ;
        path_srv.request.enemy1_y = 2400 ;
        path_srv.request.enemy2_x = 800 ;
        path_srv.request.enemy2_y = 1500 ;
        path_srv.request.ally_x = 1400 ;
        path_srv.request.ally_y = 1800 ; 
        path_srv.request.goal_pos_x = 1600;
        path_srv.request.goal_pos_y = 2400;
        //GOAP
        goap_srv.request.replan=0;
        goap_srv.request.action_done=0;
        goap_srv.request.pos.push_back(temp.srv_to_path.request.my_pos_x);
        goap_srv.request.pos.push_back(temp.srv_to_path.request.my_pos_y);
        goap_srv.request.my_degree = temp.robot_degree ; 
        
        if(client_goap.call(goap_srv)){
            ROS_INFO("test_goap: %ld", (long int)goap_srv.response.degree);
            ROS_INFO("test_goap: %ld", (long int)goap_srv.response.speed);
            ROS_INFO("test_goap: %ld", (long int)goap_srv.response.mode);
            temp.movement_from_goap[0]=goap_srv.response.ST2[0];
            temp.movement_from_goap[1]=goap_srv.response.ST2[1];
            temp.movement_from_goap[2]=goap_srv.response.ST2[2];
            temp.movement_from_goap[3]=goap_srv.response.ST2[3];
            temp.movement_from_goap[4]=goap_srv.response.ST2[4];
            temp.movement_from_goap[5]=goap_srv.response.ST2[5];
            temp.movement_from_goap[6]=goap_srv.response.ST2[6];
            temp.movement_from_goap[7]=goap_srv.response.ST2[7];
            temp.movement_from_goap[8]=goap_srv.response.ST2[8];
            temp.movement_from_goap[9]=goap_srv.response.ST2[9];
            temp.movement_from_goap[10]=goap_srv.response.ST2[10];
            temp.movement_from_goap[11]=goap_srv.response.ST2[11];
            temp.movement_from_goap[12]=goap_srv.response.ST2[12];
            temp.movement_from_goap[13]=goap_srv.response.ST2[13];
            temp.movement_from_goap[14]=goap_srv.response.ST2[14];
            ROS_INFO("ST2: %ld", (long int)goap_srv.response.ST2[0]);
        }else{
            ROS_ERROR("Failed to call goap_test");
        }


        action act(goap_srv.response.pos[0] ,goap_srv.response.pos[1] ,temp.movement_from_goap ,goap_srv.response.degree ,goap_srv.response.speed ,true ,goap_srv.response.mode);//<----------get from goap(x, y, movement_num, what_angle, how_fast, is_wait, what_mode)*********
        int switch_mode_distance = 100;
        int desire_pos_x = act.PosX();
        int desire_pos_y = act.PosY();
        int * desire_movement;
        desire_movement = act.Movement(); //[15]
        int desire_speed = act.Speed();
        int desire_mode = act.Mode();
        bool desire_wait = act.Wait();
        double desire_angle = act.Angle();
        action_state.RefreshActionState();

        switch(stat){
            case Status::SET_STRATEGY: //0
                for_st1.data.push_back(0x6000);
                for_st1.data.push_back(0);
                for_st1.data.push_back(0);
                for_st1.data.push_back(0);
                break;
            case Status::RUN:{ //5
                Mode m;
                if(desire_mode == 1){
                    m = Mode::POSITION_MODE;
                }
                else{
                    m = Mode::SPEED_MODE;
                }
                switch(m){
                    case Mode::POSITION_MODE:
                        RobotState robot;

                        if(current_state.MyPosX() == desire_pos_x && current_state.MyPosY() == desire_pos_y){
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
                                // give orders to STM2
                                for_st2.data.push_back(desire_movement[12]);
                                for_st2.data.push_back(desire_movement[13]);
                                for(int i = 0; i < 12; i ++){
                                    if(desire_movement[i] == 1){
                                        old_grab_status[i] = 1;
                                    }
                                    else if(desire_movement[i] == 0){
                                        old_grab_status[i] = 0;
                                    }
                                }
                                for(int i = 0; i < 12; i ++){
                                    out += old_grab_status[i];
                                    out = out << 2;
                                }
                                out = out << 6;
                                for_st2.data.push_back(out);
                                for_st2.data.push_back(desire_movement[14]);
                                /////////////////////////////////////////////////////////////////////////////************
                                break;

                            case RobotState::BLOCKED:
                                //return stop; //<-------------tell STM to stop
                                for_st1.data.push_back(0x5000);
                                for_st1.data.push_back(0);
                                for_st1.data.push_back(0);
                                for_st1.data.push_back(0);                    
                                action_state.ChangeReplanMission(true);   //---------------> GOAP replan == True
                                break;

                            case RobotState::ON_THE_WAY:
                                if (client_path.call(path_srv)){
                                    double clustering_time = ros::Time::now().toSec () - begin_time; 
                                    ROS_INFO ("%f secs for path plan .", clustering_time);
                                    ROS_INFO("next_pos_x: %ld", (long int)path_srv.response.next_pos_x);
                                    ROS_INFO("next_pos_y: %ld", (long int)path_srv.response.next_pos_y);
                                    now_degree = path_srv.response.degree ; 
                                }else{
                                    ROS_ERROR("Failed to call service path plan");
                                }
                                if(now_degree<0){
                                    now_degree = last_degree;
                                }
                                distance_square = (current_state.MyPosX() - desire_pos_x)*(current_state.MyPosX() - desire_pos_x) + (current_state.MyPosY() - desire_pos_y)*(current_state.MyPosY() - desire_pos_y);
                                if(distance_square < switch_mode_distance){
                                    for_st1.data.push_back(0x4000);
                                    for_st1.data.push_back(desire_pos_x);
                                    for_st1.data.push_back(desire_pos_y);
                                    for_st1.data.push_back(desire_angle);
                                    //return pos_mode; //<----------------
                                }
                                else{
                                    for_st1.data.push_back(0x3000);
                                    for_st1.data.push_back(desire_speed);
                                    for_st1.data.push_back(path_srv.response.degree);;//<-----------------
                                    for_st1.data.push_back(0);
                                    /*
                                    r0 = 0x3000;
                                    r1 = desire_speed;
                                    r2 = A*();//<-----------------
                                    r3 = 0;
                                    */
                                    if( path_srv.response.blocked == true){ //<-------------
                                        goal_covered_counter ++;
                                    }
                                    if( path_srv.response.blocked == false){ //<-------------
                                        goal_covered_counter = 0;
                                    }
                                    if(goal_covered_counter > cover_limit){
                                        action_state.ChangeKillMission(true);
                                        goal_covered_counter = 0;
                                    }   
                                    //return speed_mode;//<-----------------
                                }
                                break;
                        }
                        break;

                    case Mode::SPEED_MODE:
                        for_st2.data.push_back(desire_movement[12]);
                        for_st2.data.push_back(desire_movement[13]);
                        for(int i = 0; i < 12; i ++){
                            if(desire_movement[i] == 1){
                                old_grab_status[i] = 2;
                            }
                            else if(desire_movement[i] == 0){
                                old_grab_status[i] = 0;
                            }
                        }
                        for(int i = 0; i < 12; i ++){
                            out += old_grab_status[i];
                            out = out << 2;
                        }
                        out = out << 6;
                        for_st2.data.push_back(out);
                        for_st2.data.push_back(desire_movement[14]);
                        if(current_state.MyPosX() != desire_pos_x && current_state.MyPosY() != desire_pos_y){
                           if(current_state.MyActionStatus() == 1){ //if not at pos and hasn't got cup, keep going
                                if (client_path.call(path_srv)){
                                    double clustering_time = ros::Time::now().toSec () - begin_time; 
                                    ROS_INFO ("%f secs for path plan .", clustering_time);
                                    ROS_INFO("next_pos_x: %ld", (long int)path_srv.response.next_pos_x);
                                    ROS_INFO("next_pos_y: %ld", (long int)path_srv.response.next_pos_y);
                                    now_degree = path_srv.response.degree ; 
                                }else{
                                    ROS_ERROR("Failed to call service path plan");
                                }
                                if(now_degree<0){
                                    now_degree = last_degree;
                                }                                
                                for_st1.data.push_back(0x3000);
                                for_st1.data.push_back(desire_speed);
                                for_st1.data.push_back(path_srv.response.degree);;//<-----------------
                                for_st1.data.push_back(0);
                                if( path_srv.response.blocked == true){ //<-------------
                                    goal_covered_counter ++;
                                }
                                if( path_srv.response.blocked == false){ //<-------------
                                    goal_covered_counter = 0;
                                }
                                if(goal_covered_counter > cover_limit){
                                    //action_state.ChangeKillMission(true);
                                    action_state.ChangeActionStatus(2); //should kill mission but fuck that.. this should still work
                                    goal_covered_counter = 0;
                                    for(int i = 0; i < 12; i ++){
                                       if(desire_movement[i] == 2){
                                            old_grab_status[i] = 0;
                                        }
                                    }
                            	} //if get cup, action_done is then true(MyActionStatus()==2), GOAP will automatically give next action
                           }
                            else{
                                for(int i = 0; i < 12; i ++){
                                       if(desire_movement[i] == 2){
                                            old_grab_status[i] = 1;
                                        }
                                    }
                            }
                        }
                        else{ //at pos, mission has failed..
                            //action_state.ChangeKillMission(true);
                            action_state.ChangeActionStatus(2); //should kill mission but fuck that.. this should still work
                            for(int i = 0; i < 12; i ++){
                               if(desire_movement[i] == 2){
                                    old_grab_status[i] = 0;
                                }
                            }
                        }
                        break;
                }
                break;
                }
            case Status::STOP: //6
                for_st1.data.push_back(0x5000);
                for_st1.data.push_back(0);
                for_st1.data.push_back(0);
                for_st1.data.push_back(0);
                break;  
        }
        pub_st1.publish(for_st1);
        pub_st2.publish(for_st2);
        last_degree = now_degree ; 
        ros::spinOnce();
	}
	return 0;
}

