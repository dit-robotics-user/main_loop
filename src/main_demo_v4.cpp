//==========================
//對應版本:
//goap --->main_demo_4.py
//srv  --->goap_demo_2.srv
//20200604 apdate main(new from C)  
// close lidar
//==========================
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <std_msgs/Int32MultiArray.h>
#include "main_loop/path.h"
#include "main_loop/agent.h"
#include "main_loop/goap_demo_2.h"
#include <main_loop/from_goap.h>
#include "main_loop/main_state.h"
#include <main_loop/goap_debug.h>
#include "main_loop/main_debug.h"
#include "main_loop/world_state.h"
#include "main_loop/set_strategy.h"

#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
#include <stdlib.h>

#define pi 3.14159265359
using namespace std;
bool masterDelayStartTime = 0;

class state{
public:
    state(){};
    state(int my_x, int my_y, int deg,  bool block, int tx0_, int tx1_, int tx2_){
        my_pos_x = my_x;
        my_pos_y = my_y;
        is_blocked = block;
        tx0 = tx0_;
        tx1 = tx1_;
        tx2 = tx2_;
        degree = deg;
        a_done = false;
        kill_mission = false;
        replan_mission = false;
        replan_path = false;
        is_blocked = block;
        action_wait = false;
    }
    void ChangeActionDone(bool tf){
        a_done = tf;
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
    void ChangeIsBlocked(bool tf){
        is_blocked = tf;
    }
    int MyActionDone(){
        return a_done;
    }
    int MyPosX(){
        return my_pos_x;
    }
    int MyPosY(){
        return my_pos_y;
    }
    int MyDegree(){
        return degree;
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
    int MyTx0(){
        return tx0;
    }
    int MyTx1(){
        return tx1;
    }
    int MyTx2(){
        return tx2;
    }
    void RefreshActionState(){
        a_done = false;
        kill_mission = false;
        replan_mission = false;
        replan_path = false;
        is_blocked = false;
        action_wait = false;
    }

private:
    bool a_done;
    int my_pos_x;
    int my_pos_y;
    bool kill_mission;
    bool replan_mission;
    bool replan_path;
    bool is_blocked;
    bool action_wait;
    long int tx0;
    long int tx1;
    long int tx2;
    int degree;
};

class action{
public:
    action(int x, int y, int movement_num[], int what_angle, int how_fast, bool is_wait, int what_mode){
        goal_pos_x = x;
        goal_pos_y = y;
        for(int i = 0; i < 16; i ++){
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
    int Angle(){
        return angle;
    }
    int Mode(){
        return mode;
    }
private:
    int goal_pos_x;
    int goal_pos_y;
    int movement[16];
    int speed;
    int angle;
    bool wait;
    int mode;
};

enum class ActionMode {SPEED_MODE, POSITION_MODE};
enum class Status {SET_STRATEGY, RESET, SET_INITIAL_POS, STARTING_SCRIPT, READY, RUN, STOP, IDLE};
enum class RobotState {AT_POS, ON_THE_WAY, BLOCKED};

class sub_state{    //--->定義輸出輸入所需參數
	public:
    	sub_state();
		~sub_state(){};
		void callback(const main_loop::agent::ConstPtr& msg);
        bool lidar_be_blocked();
        bool blocking_with_direction(bool blocking_condition, int my_angle, int desire_angle);  //(blocking_condition = current_state.IsBlocked(), my_a = temp.from_agent.my_degree)
        int adjust_direction(int my_pos_x,int my_pos_y,int desire_x,int desire_y);
        int movement_from_goap[15];
        int cup_color[5]; 
        main_loop::agent from_agent;
        bool emergency[8];
		
	private:
		ros::NodeHandle n ;
		ros::Subscriber Agent_sub ;		
};

sub_state::sub_state(){
    //servo 初始值
    from_agent.servo_state = -2 ;
    from_agent.stepper = -2 ;
    from_agent.hand = -2 ;
	Agent_sub = n.subscribe<main_loop::agent>("agent_msg", 1, &sub_state::callback,this);
	for(int j=0;j<8;j++){
        emergency[j]=false;
    }
}

void sub_state::callback(const main_loop::agent::ConstPtr& msg){
    from_agent.my_pos_x = msg->my_pos_x ;
    from_agent.my_pos_y = msg->my_pos_y ;
    from_agent.my_degree = msg->my_degree ; 
    from_agent.status = msg->status;
    from_agent.servo_state = msg->servo_state;
    from_agent.stepper = msg -> stepper ;
    from_agent.hand = msg->hand ; 
    from_agent.enemy1_x = msg->enemy1_x ;
    from_agent.enemy1_y = msg->enemy1_y ;
    from_agent.enemy2_x = msg->enemy2_x ;
    from_agent.enemy2_y = msg->enemy2_y ;
	from_agent.ally_x = msg->ally_x;
	from_agent.ally_y = msg->ally_y;
    from_agent.time = msg->time ; 
    from_agent.ns = msg->ns;

	emergency[0]=msg->emergency[0];
    emergency[1]=msg->emergency[1];
    emergency[2]=msg->emergency[2];
    emergency[3]=msg->emergency[3];
    emergency[4]=msg->emergency[4];
    emergency[5]=msg->emergency[5];
    emergency[6]=msg->emergency[6];
    emergency[7]=msg->emergency[7];

    cup_color[0]=msg->cup_color[0];
    cup_color[1]=msg->cup_color[1];
    cup_color[2]=msg->cup_color[2];
    cup_color[3]=msg->cup_color[3];
    cup_color[4]=msg->cup_color[4];
  	
}


bool sub_state::lidar_be_blocked(){
    bool blockk=false;
    for(int i=0;i<8;i++){
        if(emergency[i]==true){
            blockk=true;
        }
    }
    return blockk;
}

int sub_state::adjust_direction(int my_pos_x_,int my_pos_y_,int desire_x,int desire_y){
	
	int delta_x = desire_x - my_pos_x_;
	int delta_y = desire_y - my_pos_y_;
	int desire_angle_ = atan2(delta_y,delta_x);
	desire_angle_ = (desire_angle_/pi)*180;
	if(desire_angle_<0){
		desire_angle_=-desire_angle_;
	}else{
		desire_angle_=desire_angle_;
	}	
	//ROS_INFO("desire_angle_ in adjust_function =%d",desire_angle_)	;
	return desire_angle_;
}

bool sub_state::blocking_with_direction(bool blocking_condition, int my_angle, int desire_angle){  //(blocking_condition = current_state.IsBlocked(), my_a = temp.from_agent.my_degree)
    bool blocked = false;
    if(blocking_condition){
        int angle_a = my_angle - 90;
        int angle_b = my_angle + 90;
        if(angle_a < 0){
            angle_a += 360;
        }
        if(angle_b < 0){
            angle_b += 360;
        }
        if(angle_a > 360){
            angle_a -= 360;
        }
        if(angle_b > 360){
            angle_b -= 360;
        }
        if(angle_b < angle_a){
			/*
			int temp;
			temp = angle_b;
			angle_b = angle_a;
			angle_a = temp;
			*/
			if(desire_angle < angle_a && desire_angle > angle_b){
				// forward motion
				for(int i=0;i<=3;i++){
					if(emergency[i]==true){
						/*
						ROS_INFO("desire_angle:%d",desire_angle);
						ROS_INFO("angle_a:%d",angle_a);
						ROS_INFO("angle_b:%d",angle_b);
						ROS_INFO("backword blocked,b<a,a~b");
						*/
						blocked=true;
					}
				}
			}else{
				// backward motion 
				for(int i=4;i<=7;i++){
					if(emergency[i]==true){
						/*
						ROS_INFO("desire_angle:%d",desire_angle);
						ROS_INFO("angle_a:%d",angle_a);
						ROS_INFO("angle_b:%d",angle_b);
						ROS_INFO("forward blocked,b<a");
						*/
						blocked=true;
					}
				}
			} 
		}else{
			
			if(desire_angle < angle_b && desire_angle > angle_a){
				// forward motion
				for(int i=4;i<=7;i++){
					if(emergency[i]==true){
						/*
						ROS_INFO("desire_angle:%d",desire_angle);
						ROS_INFO("angle_a:%d",angle_a);
						ROS_INFO("angle_b:%d",angle_b);
						ROS_INFO("forward blocked,b>a,a~b");
						* */
						blocked=true;
					}
				}
			}
			else{
				// backward motion 
				for(int i=0;i<=3;i++){
					if(emergency[i]==true){
						/*
						ROS_INFO("desire_angle:%d",desire_angle);
						ROS_INFO("angle_a:%d",angle_a);
						ROS_INFO("angle_b:%d",angle_b);
						ROS_INFO("backward blocked,b>a");
						*/
						blocked=true;
					}
				}
			}   
				
		}

	ROS_INFO("lidar in function []:%d,%d,%d,%d,%d,%d,%d,%d",emergency[0],emergency[1],emergency[2],emergency[3],emergency[4],emergency[5],emergency[6],emergency[7]);
    }
    
    return blocked ;
}

bool at_pos(int x, int y, int deg, int c_x, int c_y, int c_deg, int m, int angle_m){    //計算誤差
    bool at_p = false;
    int exact_angle_difference=0 ;
    int angle_difference_1=abs(c_deg-deg);
    int angle_difference_2=360-abs(c_deg-deg);
    if(angle_difference_1 > angle_difference_2){
        exact_angle_difference = angle_difference_2;
    }else{
        exact_angle_difference = angle_difference_1;
    }    
    if(abs(x - c_x) < m && abs(y - c_y) < m && exact_angle_difference < angle_m){
        at_p = true;
    }
    return at_p;
}

int main(int argc, char **argv)
{

    //ROS的topic和service定義處
 	ros::init (argc, argv, "main_demo_v4");
	ros::NodeHandle nh;    
	
    ros::Publisher pub_st1 = nh.advertise<std_msgs::Int32MultiArray>("txST1", 1);
	ros::Publisher pub_st2 = nh.advertise<std_msgs::Int32MultiArray>("txST2", 1);
    ros::Publisher pub_goap_response = nh.advertise<main_loop::goap_debug>("Goap_response", 1);
    ros::Publisher pub_main_state = nh.advertise<main_loop::main_debug>("Main_state", 1);
	ros::ServiceClient client_path = nh.serviceClient<main_loop::path>("path_plan");
    ros::ServiceClient client_goap = nh.serviceClient<main_loop::goap_demo_2>("goap_test_v1");
    ros::ServiceClient client_set_goap = nh.serviceClient<main_loop::set_strategy>("set");   

    // give default value here
    long int r0=0x6000;
    long int r1=0;
    long int r2=0;
    long int r3=0;
    long int rx0=0;
    long int rx1=0;
    long int rx2=0;
    long int rx3=0;

    int goal_covered_counter = 0;
    int cover_limit = 20;
    int old_grab_status[12] = {0};
    int distance_square = 0;
    int action_done = false;
    int kill_mission = false;
    int replan_mission = false;
    int margin = 20;
    int speed_mode_margin = 100;
    int angle_margin = 5;
    int speed_mode_angle_margin = 360;
    int switch_mode_distance = 250000;//square
    int left_layer = 0;
    int right_layer = 0;
    int set_goap = 0 ;
    ActionMode m;
    RobotState robot;



    //在subscriber定義的參數則需宣告一個sub_class在此，即可運用在回調函式使用到的參數
    sub_state temp;
    //在main裡定義的topic和service所需的傳輸格式需由此先宣告一次
    //並設定初始值以免service fail
	main_loop::path path_srv;
    main_loop::goap_demo_2 goap_srv;
    main_loop::world_state little_ws; 
    main_loop::set_strategy strategy_srv;
    //ros defult value
    int now_my_pos_x;
    int now_my_pos_y;
    float last_degree = 0 ;
    float now_degree = 0 ;
    path_srv.request.goal_pos_x = 0;
    path_srv.request.goal_pos_y = 0;
	path_srv.request.my_pos_x = 700 ;
    path_srv.request.my_pos_y = 300 ;
    path_srv.request.enemy1_x = 1800 ;
    path_srv.request.enemy1_y = 2400 ;
    path_srv.request.enemy2_x = 1800 ;
    path_srv.request.enemy2_y = 2300 ;
    path_srv.request.ally_x = 1800 ;
    path_srv.request.ally_y = 2200 ; 

    goap_srv.request.action_done=false;
    goap_srv.request.pos.push_back(700);
    goap_srv.request.pos.push_back(300);
    goap_srv.request.cup_color = {}; 
    goap_srv.request.north_or_south = 0; 
    goap_srv.request.my_degree = 0 ; 
    goap_srv.request.time = 0 ;
    goap_srv.request.mission_name = "setting" ;
    goap_srv.request.mission_child_name = "setting" ;


    int count = 0 ;
    


    while(ros::ok()){
    
        //status update
        int s = temp.from_agent.status; 
        Status stat= static_cast<Status>(s);

        //debug
        main_loop::goap_debug debug_1;
        main_loop::main_debug debug_2;
       

        switch(stat){
            case Status::SET_STRATEGY: //0
                r0 = 0x6000;
                r1 = 0;
                r2 = 0;
                r3 = 0;

                //把資料丟入debug topic
                debug_1.desire_degree=0;
                debug_1.desire_speed=0;
                debug_1.desire_mode=r0;
                debug_1.desire_pos={};
                debug_1.desire_servo_state=0;
                debug_1.desire_stepper=0;
                debug_1.desire_hand=0;
                debug_1.is_wait=0;
                debug_1.mission_name= "setting";
                
                break;
                

            case Status::RESET: //1
                r0 = 0x6000;
                r1 = 0;
                r2 = 0;
                r3 = 0;
                break;

            case Status::SET_INITIAL_POS:   //2
                
                r0 = 0x1000;
                r1 = 1280;
                r2 = 130;
                r3 = 270;
                break;            
            case Status::STARTING_SCRIPT:   //3
                
                r0 = 0x2000;
                r1 = 0;
                r2 = 0;
                r3 = 0;
               
                break;

            case Status::READY:{    //4
                r0 = 0x5000;
                r1 = 0;
                r2 = 0;
                r3 = 0;

                strategy_srv.request.strategy = temp.from_agent.strategy;
                strategy_srv.request.set_finish = 1 ;
                strategy_srv.request.init_pos.push_back(temp.from_agent.my_pos_x);
                strategy_srv.request.init_pos.push_back(temp.from_agent.my_pos_y);
                strategy_srv.request.cup_color = {}; 
                strategy_srv.request.cup_color.push_back(temp.cup_color[4]);
                strategy_srv.request.cup_color.push_back(temp.cup_color[3]); 
                strategy_srv.request.cup_color.push_back(temp.cup_color[2]);
                strategy_srv.request.cup_color.push_back(temp.cup_color[1]); 
                strategy_srv.request.cup_color.push_back(temp.cup_color[0]); 
                if(temp.cup_color[0]!=2 && set_goap==0){
                    if(client_set_goap.call(strategy_srv)){
                        if(strategy_srv.response.goap_return==true){
                            set_goap=1;
                            ROS_INFO("set goap finish");
                        }
                    }else{
                        ROS_INFO("Failed to call set_strategy");
                    }
                } 

                break;
            }
            case Status::RUN:{ //5
				if(masterDelayStartTime == 0)
				{
					masterDelayStartTime = 1;
					ros::Duration(5).sleep();
				}			
                count ++;
                //將agent資訊存入current state
                
                state            
                current_state(temp.from_agent.my_pos_x,temp.from_agent.my_pos_y,temp.from_agent.my_degree,false,
                temp.from_agent.servo_state,temp.from_agent.stepper,temp.from_agent.hand);//<--------get undergoing, finish, my_x, my_y, block from other nodes ()********
                state action_state(0,0,0,false,0,0,0);
                action_state = current_state;
                if(action_done){
                    action_state.ChangeActionDone(true);
                    action_done = false;
                    ROS_INFO("action done in action state: %d" , action_state.MyActionDone());
                }
                if(kill_mission){
                    action_state.ChangeKillMission(true);
                    kill_mission = false;
                }

                //更新service所需要的資料
                //path plan
                path_srv.request.my_pos_x = temp.from_agent.my_pos_x;
                path_srv.request.my_pos_y = temp.from_agent.my_pos_y;
                path_srv.request.enemy1_x = temp.from_agent.enemy1_x ;
                path_srv.request.enemy1_y = temp.from_agent.enemy1_y ;
                path_srv.request.enemy2_x = temp.from_agent.enemy2_x ;
                path_srv.request.enemy2_y = temp.from_agent.enemy2_y ;
                path_srv.request.ally_x = temp.from_agent.ally_x  ;
                path_srv.request.ally_y = temp.from_agent.ally_y  ; 
                //goap
                goap_srv.request.time = temp.from_agent.time;
                goap_srv.request.cup_color={};
                goap_srv.request.cup_color.push_back(temp.cup_color[0]);
                goap_srv.request.cup_color.push_back(temp.cup_color[1]); 
                goap_srv.request.cup_color.push_back(temp.cup_color[2]);
                goap_srv.request.cup_color.push_back(temp.cup_color[3]); 
                goap_srv.request.cup_color.push_back(temp.cup_color[4]); 
                goap_srv.request.north_or_south = temp.from_agent.ns ; 
                goap_srv.request.action_done=action_state.MyActionDone();
                goap_srv.request.pos.push_back(action_state.MyPosX());
                goap_srv.request.pos.push_back(action_state.MyPosY());
                goap_srv.request.my_degree = temp.from_agent.my_degree ; 
                //避免在下面被洗掉所以先在此存入Debug 
                debug_2.action_done=action_state.MyActionDone();    
            
                //oap service
                if(client_goap.call(goap_srv)){
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
                    temp.movement_from_goap[15]=goap_srv.response.ST2[15];
                   
                    //for path plan
                    path_srv.request.goal_pos_x = goap_srv.response.pos[0];
                    path_srv.request.goal_pos_y = goap_srv.response.pos[1];
                }else{
                    ROS_ERROR("Failed to call goap_test");
                }
                //goap mission name update
                goap_srv.request.mission_name = goap_srv.response.mission_name ;
                goap_srv.request.mission_child_name = goap_srv.response.mission_child_name ;

                //將goap所需的資料存入action      
                action act(goap_srv.response.pos[0],goap_srv.response.pos[1],temp.movement_from_goap,goap_srv.response.degree,goap_srv.response.speed,goap_srv.response.is_wait,goap_srv.response.mode);
                int desire_pos_x = act.PosX();
                int desire_pos_y = act.PosY();
                int*desire_movement;
                int desire_speed = act.Speed();
                int desire_mode = act.Mode();
                bool desire_wait = act.Wait();
                int desire_angle = act.Angle();
                desire_movement = act.Movement();

                           
                if(desire_mode == 2){
                    m = ActionMode::SPEED_MODE;
                }
                else{
                    m = ActionMode::POSITION_MODE;
                } 

                switch(m){
                    case ActionMode::POSITION_MODE:{
                        debug_2.robot_state="ActionMode::POSITION_MODE";

                        if(current_state.IsBlocked()){
                            robot = RobotState::BLOCKED;
                        }
                        else{
                            if(at_pos(current_state.MyPosX(),current_state.MyPosY(),current_state.MyDegree(), desire_pos_x, desire_pos_y, desire_angle, margin, angle_margin)){
                                robot = RobotState::AT_POS;
								r0 = 0x4000;
								r1 = desire_pos_x;
								r2 = desire_pos_y;
								r3 = desire_angle;
                            }
                            else{
                                robot = RobotState::ON_THE_WAY;
                            }
                        }
                        
                        switch(robot){
                            case RobotState::AT_POS:{
                                debug_2.robot_case="AT_POS";

                                r0 = 0x5000;
                                r1 = 0;
                                r2 = 0;
                                r3 = 0;
                                
                                //rx0
                                long int out = 0;
                                for(int i = 0; i < 12; i ++){
                                    if(desire_movement[i] != -1){
                                        old_grab_status[i] = desire_movement[i];
                                    }
                                }
                                for(int i = 11; i >= 0; i --){
                                    out = out << 2;
                                    out += old_grab_status[i];
                                }
                                rx0 = out;
                                //rx1
                                if(desire_movement[12]!=-1){
                                    left_layer = desire_movement[12];
                                }
                                if(desire_movement[13]!=-1){
                                    right_layer = desire_movement[13];
                                }
                           
                                rx1 = (left_layer << 2) + (right_layer << 6);
                                //rx2
                                if(desire_movement[14]!=-1){
                                    rx2 = desire_movement[14];
                                }
                                //rx3 
                                if(desire_movement[15]!=-1){
                                    rx3 = desire_movement[15];
                                       
                                }
                                break;
						  }
/*
                            case RobotState::BLOCKED:
                                debug_2.robot_case="BLOCKED";
                                //return stop; //<-------------tell STM to stop
                                r0 = 0x5000;
                                r1 = 0;
                                r2 = 0;
                                r3 = 0;
                                action_state.ChangeReplanMission(true);   //---------------> GOAP replan == True
                                break;
*/
                            case RobotState::ON_THE_WAY:
                                debug_2.robot_case="ON_THE_WAY";

 
                                distance_square = (current_state.MyPosX() - desire_pos_x)*(current_state.MyPosX() - desire_pos_x) + (current_state.MyPosY() - desire_pos_y)*(current_state.MyPosY() - desire_pos_y);
                                if(distance_square <= switch_mode_distance){
                                    r0 = 0x4000;
                                    r1 = desire_pos_x;
                                    r2 = desire_pos_y;
                                    r3 = desire_angle;
                                    
                                    //lidar方向判讀
									int now_desired_angle = temp.adjust_direction(action_state.MyPosX(),action_state.MyPosY(),desire_pos_x,desire_pos_y);
									ROS_INFO("now_desired_angle:%d",now_desired_angle);
									current_state.ChangeIsBlocked(temp.blocking_with_direction(temp.lidar_be_blocked(),temp.from_agent.my_degree,now_desired_angle)); 
									if(current_state.IsBlocked() == true){
										debug_2.robot_case="BLOCKED";
										//return stop; //<-------------tell STM to stop
										r0 = 0x5000;
										r1 = 0;
										r2 = 0;
										r3 = 0;
										action_state.ChangeReplanMission(true);
										ROS_INFO ("desire_pos_x:%d ", desire_pos_x);                
										ROS_INFO ("desire_pos_y:%d ", desire_pos_y);
										ROS_INFO ("my_pos_x:%d ", temp.from_agent.my_pos_x);
										ROS_INFO ("my_pos_y:%d ", temp.from_agent.my_pos_y);
									}
                                    
                                    
                                    //return pos_mode; //<----------------
                                }
                                else{
                                    //path service
									if(client_path.call(path_srv)){	
										now_degree = path_srv.response.degree ; 
									}else{
										ROS_ERROR("Failed to call service path plan");
										ROS_INFO ("position mode");
										ROS_INFO ("desire_pos_x:%d ", desire_pos_x);                
										ROS_INFO ("desire_pos_y:%d ", desire_pos_y);
										ROS_INFO ("my_pos_x:%d ", temp.from_agent.my_pos_x);
										ROS_INFO ("my_pos_y:%d ", temp.from_agent.my_pos_y);
									}
									if(now_degree<0){   //--->如果回傳值為負值表示path plan 沒有算出資料 就存取上一次計算出的數值
										now_degree = last_degree;
									} 
							
                                    r0 = 0x3000;
                                    r1 = desire_speed;
                                    r2 = now_degree;
                                    r3 = 0;
                                    //若敵人擋住路徑，增加counter後回傳kill mission
                                    if( path_srv.response.blocked == true){ 
                                        goal_covered_counter ++;
                                    }
                                    if( path_srv.response.blocked == false){ 
                                        goal_covered_counter = 0;
                                    }
                                    if(goal_covered_counter > cover_limit){
                                        action_state.ChangeKillMission(true);
                                        goal_covered_counter = 0;
                                    }
                                    //return speed_mode;//<-----------------
                                    //lidar方向判讀
									current_state.ChangeIsBlocked(temp.blocking_with_direction(temp.lidar_be_blocked(),temp.from_agent.my_degree,now_degree)); 
									if(current_state.IsBlocked() == true){
										debug_2.robot_case="BLOCKED";
										//return stop; //<-------------tell STM to stop
										r0 = 0x5000;
										r1 = 0;
										r2 = 0;
										r3 = 0;
										action_state.ChangeReplanMission(true);
										ROS_INFO ("desire_pos_x:%d ", desire_pos_x);                
										ROS_INFO ("desire_pos_y:%d ", desire_pos_y);
										ROS_INFO ("my_pos_x:%d ", temp.from_agent.my_pos_x);
										ROS_INFO ("my_pos_y:%d ", temp.from_agent.my_pos_y);
									}
                                }
                                break;
							}
							bool c;
							if(rx1==current_state.MyTx1()){
								c = true;
							}
							else{
								c = false;
							}               
							bool b;
							if(rx0==current_state.MyTx0()){
								b = true;
							}
							else{
								b = false;
							}
							bool d;
							if(rx2==current_state.MyTx2()){
								d = true;
							}
							else{
								d = false;
							}
							if(count>3 && at_pos(current_state.MyPosX(),current_state.MyPosY(),current_state.MyDegree(), desire_pos_x, desire_pos_y, desire_angle, margin, angle_margin) && b && c && d){
								ROS_INFO ("rx0:%ld ", rx0);                
								ROS_INFO ("rx1:%ld ", rx1);
								ROS_INFO ("rx2:%ld ", rx2);
								ROS_INFO("complete:%d",count);
								ROS_INFO ("debug_2.robot_case: %s ", debug_2.robot_case.c_str());
								ROS_INFO ("mission: %s ", goap_srv.response.mission_name.c_str());
								ROS_INFO ("temp.from_agent.ns=%d ", temp.from_agent.ns); 
								ROS_INFO ("[0]=%d ", goap_srv.request.cup_color[0]); 
								ROS_INFO ("[1]=%d ", goap_srv.request.cup_color[1]); 
								ROS_INFO ("[2]=%d ", goap_srv.request.cup_color[2]); 
								ROS_INFO ("[3]=%d ", goap_srv.request.cup_color[3]);
								ROS_INFO ("[4]=%d ", goap_srv.request.cup_color[4]); 
                
								action_done = true;
								goal_covered_counter = 0;
								count = 0; 
							}
							break;
						}


                    case ActionMode::SPEED_MODE:{
                        debug_2.robot_state="ActionMode::POSITION_MODE";
                        debug_2.robot_case="ON_THE_WAY";
                        
                        //==給任務及位置==
                        //基本上手跟平台都不需要給新指令，可以直接刪rx1,rx2
                        //rx0 <- desire movement
                        long int out = 0;
                        for(int i = 0; i < 12; i ++){
                            if(desire_movement[i] == 2){//自動夾給２
                                old_grab_status[i] = 2;
                            }
                        }
                        for(int i = 11; i >= 0; i --){
                            out = out << 2;
                            out += old_grab_status[i];
                        }
                        rx0 = out;
                  
                        //==任務完成轉換器===
                        long int finished_out = 0;
                        int copy_old_status[12] = {0};
                        for(int i = 0; i < 12; i ++){
                            copy_old_status[i] = old_grab_status[i];
                        }
                        for(int i = 0; i < 12; i ++){
                            if(desire_movement[i] == 2){//自動夾給２
                                copy_old_status[i] = 1;//轉換成已完成時會變成的１
                            }
                        }
                        for(int i = 11; i >= 0; i --){
                            finished_out = finished_out << 2;
                            if(count>3 &&current_state.MyTx0()== finished_out){
                            action_done = true;
                            ROS_INFO("speed mode action done");
                            //把old_grab_status中有２的換成１（自動夾完成）
                            for(int i = 0; i < 12; i ++){
                                if(old_grab_status[i] == 2){
                                        old_grab_status[i] = 1;
                                    }
                                }
                                count = 0;
                            }finished_out += copy_old_status[i];
                        }
                        //==任務完成==
                        //若自動夾完成則action_done

                        //==到點==
                        //若到點則代表沒完成任務（任務已被他人完成或失敗）
                        if(count>3 && at_pos(current_state.MyPosX(),current_state.MyPosY(), current_state.MyDegree(), desire_pos_x, desire_pos_y, desire_angle, speed_mode_margin, speed_mode_angle_margin)){
                            action_state.ChangeKillMission(true);
                            action_done = true;
                            ROS_INFO("speed mode pos action done");
                            //把old_grab_status中有２的換回０（自動夾取消）
                            for(int i = 0; i < 12; i ++){
                                if(desire_movement[i] == 2){
                                    old_grab_status[i] = 0;
                                }
                            }
                            count = 0; 
                        }
                        //==未到點==
                        //不然算路徑給速度角度
                        else{
                            distance_square = (current_state.MyPosX() - desire_pos_x)*(current_state.MyPosX() - desire_pos_x) + (current_state.MyPosY() - desire_pos_y)*(current_state.MyPosY() - desire_pos_y);
                            if(distance_square <= switch_mode_distance){//--->pos mode
                                r0 = 0x4000;
                                r1 = desire_pos_x;
                                r2 = desire_pos_y;
                                r3 = desire_angle;
                            }else{
                                //path service 
								if(client_path.call(path_srv)){
									now_degree = path_srv.response.degree ; 
								}else{
									ROS_ERROR("Failed to call service path plan");
									ROS_INFO ("speed mode");
									ROS_INFO ("desire_pos_x:%d ", desire_pos_x);                
									ROS_INFO ("desire_pos_y:%d ", desire_pos_y);
									ROS_INFO ("my_pos_x:%d ", temp.from_agent.my_pos_x);
									ROS_INFO ("my_pos_y:%d ", temp.from_agent.my_pos_y);
								}
								if(now_degree<=0){  //--->如果回傳值為負值表示path plan 沒有算出資料 就存取上一次計算出的數值
									now_degree = last_degree;
								} 
								
								r0 = 0x3000;
								r1 = desire_speed;
								r2 = now_degree;
								r3 = 0;
                                //若敵人擋住路徑，增加counter後回傳kill mission
								if( path_srv.response.blocked == true){ 
									goal_covered_counter ++;
								}
								if( path_srv.response.blocked == false){ 
									goal_covered_counter = 0;
								}
								//return speed_mode;//<-----------------
								if(goal_covered_counter > cover_limit){
									action_state.ChangeKillMission(true);
									goal_covered_counter = 0;
									//把old_grab_status中有２的換回０（自動夾取消）
									for(int i = 0; i < 12; i ++){
										if(desire_movement[i] == 2){
											old_grab_status[i] = 0;
										}
									}
								}
							}
                            
                        }
                        //==被阻擋則停車==
                        if(current_state.IsBlocked()){
                        ROS_INFO("speed mode block");
                            r0 = 0x5000;
                            r1 = 0;
                            r2 = 0;
                            r3 = 0;
                            action_state.ChangeReplanMission(true);
                        }
                        break;
                    }


                }

                //把資料丟入debug topic
                debug_1.desire_degree=desire_angle;
                debug_1.desire_speed=desire_speed;
                debug_1.desire_mode=r0;
                debug_1.desire_pos.push_back(desire_pos_x);  
                debug_1.desire_pos.push_back(desire_pos_y); 
                debug_1.desire_servo_state=rx0;
                debug_1.desire_stepper=rx1;
                debug_1.desire_hand=rx2;
                debug_1.is_wait=desire_wait;
                debug_1.mission_name = goap_srv.response.mission_name;
                debug_1.mission_child_name = goap_srv.response.mission_child_name;

                break;
            }

            case Status::STOP: //6
                r0 = 0x5000;
                r1 = 0;
                r2 = 0;
                r3 = 0;

                //把資料丟入debug topic
                debug_1.desire_degree=0;
                debug_1.desire_speed=0;
                debug_1.desire_mode=r0;
                debug_1.desire_pos={};
                debug_1.desire_servo_state=0;
                debug_1.desire_stepper=0;
                debug_1.desire_hand=0;
                debug_1.is_wait=0;
                debug_1.mission_name="NO";
                break;
        }
        //give ST
        std_msgs::Int32MultiArray for_st1 ;
        for_st1.data.push_back(r0);
        for_st1.data.push_back(r1);
        for_st1.data.push_back(r2);
        for_st1.data.push_back(r3);
        std_msgs::Int32MultiArray for_st2 ;
        for_st2.data.push_back(rx0);
        for_st2.data.push_back(rx1);
        for_st2.data.push_back(rx2);
        for_st2.data.push_back(rx3);

        pub_st1.publish(for_st1);
        pub_st2.publish(for_st2);

        
        //把資料丟入debug topic
        debug_2.status=temp.from_agent.status;
        debug_2.pos.push_back(temp.from_agent.my_pos_x);
        debug_2.pos.push_back(temp.from_agent.my_pos_y);
        debug_2.enemy1_x=path_srv.request.enemy1_x;
        debug_2.enemy1_y=path_srv.request.enemy1_y;
        debug_2.enemy2_x=path_srv.request.enemy2_x;
        debug_2.enemy2_y=path_srv.request.enemy2_y;
        debug_2.is_blocked=temp.lidar_be_blocked();
        debug_2.servo_state=temp.from_agent.servo_state;
        debug_2.stepper_state=temp.from_agent.stepper;
        debug_2.hand_state=temp.from_agent.hand;
        debug_2.kill_mission=kill_mission;
        debug_2.goal_covered_counter=goal_covered_counter;
        debug_2.time =temp.from_agent.time;
        pub_goap_response.publish(debug_1);
        pub_main_state.publish(debug_2);


        last_degree = now_degree ; 
        ros::spinOnce();
    }
    return 0;
    
}

