#include "ros/ros.h"
#include "std_msgs/String.h"
#include <std_msgs/Int32.h>
#include <std_msgs/Int32MultiArray.h>

#include "main_loop/path.h"
#include "main_loop/goap_.h"

#include "lidar_2020/alert_range.h"  //是什麼

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <stdlib.h>

#define pi 3.14159265359

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
        //void status_sub_callback(const std_msgs::Int32::ConstPtr& msg);
        void lidarmsg_sub_callback(const lidar_2020::alert_range::ConstPtr& msg);//subscribe "ranging_alert" (lidar_2020::alert_range)
        //sub_class(int my_pos_x_ = 700, int my_pos_y_ = 300, int ini_status = 0);
        ros::NodeHandle n;

	    ros::Subscriber ST1_sub = n.subscribe<std_msgs::Int32MultiArray>("rxST1", 1, &sub_class::ST1_sub_callback,this);
        ros::Subscriber ST2_sub = n.subscribe<std_msgs::Int32MultiArray>("rxST2", 1, &sub_class::ST2_sub_callback,this);
        ros::Publisher pub_st1 = n.advertise<std_msgs::Int32MultiArray>("txST1", 1);
		ros::Publisher pub_st2 = n.advertise<std_msgs::Int32MultiArray>("txST2", 1);

		ros::ServiceClient client_path = n.serviceClient<main_loop::path>("path_plan");
	    ros::ServiceClient client_goap = n.serviceClient<main_loop::goap_>("goap_test_v1");

		//ros::Publisher status_pub = n.advertise<std_msgs::Int32>("pub_status",1);  //publish to gui
	    //ros::Subscriber status_sub = n.subscribe<std_msgs::Int32>("update_status", 1, &sub_class::status_sub_callback,this);
	    //ros::Subscriber strategy_sub = n.subscribe<std_msgs::Int32>("strategy", 1, &sub_class::strategy_sub_callback,this);

	    ros::Subscriber lidarmsg_sub= n.subscribe<lidar_2020::alert_range>("ranging_alert", 1, &sub_class::lidarmsg_sub_callback,this);

		~sub_class(){};
};

enum class RobotState {AT_POS, ON_THE_WAY, BLOCKED};

RobotState robot;

int hand=5;
int suck=5; 
int stepper=5;
int flag=5;  

int rx0=0;
int rx1=0;
int rx2=0;
int rx3=0;

int r0=0;
int r1=0;
int r2=0;
int r3=0;

int current_action[4] = {hand, suck, stepper, flag};
int desire_action[10];
int temp_desire_action[10];
int old_action[10] = {9,9,9,9,6,6,6,6,1,6};
int command_action[4];
int desire_speed = 0;

int my_pos_x=700;
int my_pos_y=2800;
int my_degree=180;

int margin = 15;
int angle_margin = 3;

int switch_mode_distance = 4000000;//square
int distance_square = 0;

int sub_GUI_status = 0;
std_msgs::Int32 stat;

bool plug_out = false;

bool action_done = false;

int count = 0;

float pass_time = 0;

bool emergency[8] = {0,0,0,0,0,0,0,0};

main_loop::path path_srv;
main_loop::goap_ goap_srv;

void sub_class::lidarmsg_sub_callback(const lidar_2020::alert_range::ConstPtr& msg){  //subscribe "ranging_alert" (lidar_2020::alert_range)
    if(msg->header.seq>2){
        for(int j=0 ;j<8;j++){
            emergency[j] = msg->alert[j];
        }  
    }
}

/*void sub_class::status_sub_callback(const std_msgs::Int32::ConstPtr& msg){//subscribe "update_status" (std_msgs::Int32)

	sub_GUI_status = msg->data;

}
*/
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

bool lidar_be_blocked(){
    bool blockk=false;
    for(int i=0;i<8;i++){
        if(emergency[i]==true){
            blockk=true;
        }
    }
    return blockk;
}

bool blocking_with_direction(bool blocking_condition, int my_angle, int desire_angle){  //(blocking_condition = current_state.IsBlocked(), my_a = temp.from_agent.my_degree)
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

int adjust_direction(int my_pos_x_,int my_pos_y_,int desire_x,int desire_y){
	
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

/*bool suck_fail(){
	int i=0;
	for(i=0; i<4; i++){
		if(current_action[1]/pow(10, i))
	}
}*/

/*
bool at_pos(){
	if(my_pos_x == path_srv.request.goal_pos_x && my_pos_y == path_srv.request.goal_pos_y && my_degree == goap_srv.response.degree){
		return true;
	}
	else{
		return false;
	}
}*/

bool command_finish(){
	int count = 0;
	int k = 0;
	for(k=0; k<4; k++){
		if (current_action[k] == command_action[k]){
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

bool at_pos(int x, int y, int deg, int c_x, int c_y, int c_deg, int m, int angle_m){
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

int main(int argc, char **argv){

	ros::init(argc, argv, "main_frank");

//---------------------declare---------------------
	sub_class sub;
	
	ros::Time begin_time;
    ros::Time now_time;

    goap_srv.request.action_done=false;

    goap_srv.request.pos.push_back(300);
    goap_srv.request.pos.push_back(300);
   // goap_srv.request.cup_color = {}; 
    goap_srv.request.north_or_south = 0 ; 

    goap_srv.request.time = 0 ;
    goap_srv.request.mission_name = "setting" ;
    goap_srv.request.mission_child_name = "setting" ;

	path_srv.request.goal_pos_x = 1000; 
    path_srv.request.goal_pos_y = 1000; 
    
    begin_time = ros::Time::now();


	while(ros::ok()){
		
				count ++;
				
				now_time = ros::Time::now();
				pass_time = (now_time - begin_time).toSec();

				goap_srv.request.time = pass_time;
				ROS_INFO("passing time: %f" , pass_time);
				
				/*sub.n.getParam("main_frank/rx0", rx0);
				sub.n.getParam("main_frank/rx1", rx1);
				sub.n.getParam("main_frank/rx2", rx2);
				sub.n.getParam("main_frank/rx3", rx3);    */

				/*desire_action[0] = rx0;
				desire_action[1] = rx1;
				desire_action[2] = rx2;
				desire_action[3] = rx3;*/

				if(sub.client_goap.call(goap_srv)){
		                    
		            ROS_INFO("success to call goap_test");

			    	temp_desire_action[0]=goap_srv.response.ST2[0];
		            temp_desire_action[1]=goap_srv.response.ST2[1];
		            temp_desire_action[2]=goap_srv.response.ST2[2];
		            temp_desire_action[3]=goap_srv.response.ST2[3];
		            temp_desire_action[4]=goap_srv.response.ST2[4];
		            temp_desire_action[5]=goap_srv.response.ST2[5];
		            temp_desire_action[6]=goap_srv.response.ST2[6];
		            temp_desire_action[7]=goap_srv.response.ST2[7];
		            temp_desire_action[8]=goap_srv.response.ST2[8];
		            temp_desire_action[9]=goap_srv.response.ST2[9];

		            goap_srv.request.mission_name = goap_srv.response.mission_name ;
		            goap_srv.request.mission_child_name = goap_srv.response.mission_child_name ;
		        }
		        else{
		            ROS_ERROR("Failed to call goap_test");
		        }

				current_action[0] = hand;
				current_action[1] = suck;
				current_action[2] = stepper;
				current_action[3] = flag;
		/*
				ROS_INFO("action_from_goap:");
				for(int i=0; i<10; i++){
					ROS_INFO("%d ", temp_desire_action[i]);
				}
				ROS_INFO("old:");
				for(int i=0; i<10; i++){
					ROS_INFO("%d ", old_action[i]);
				}
		*/
		/*
				//rx0 hand 1234
		        rx0 = 0;
		        for(int i = 0; i < 4; i ++){
		            if(temp_desire_action[i] == -1){
		                desire_action[i] = old_action[i];
		            }
		            else{
				desire_action[i] = temp_desire_action[i];
		            }
		        }
		        for(int i = 0; i < 4; i ++){
		            rx0 += pow(10, 3-i) * desire_action[i]; //可以用嗎
		        }
		        
		        //rx1 suck 1234
		        rx1 = 0;
		        for(int i = 4; i < 8; i ++){
		            if(temp_desire_action[i] == -1){
		                desire_action[i] = old_action[i];
		            }
			    else{
				desire_action[i] = temp_desire_action[i];
		            }
		        }
		        for(int i = 4; i < 8; i ++){
		            rx1 += pow(10, 7-i) * desire_action[i]; //可以用嗎
		        }
		        //rx2
		        rx2 = 0;
		        if(temp_desire_action[8] == -1){
			    desire_action[8] = old_action[8];         //it will let old_action[i] = desire_action[i] work correctlly
			    rx2 = old_action[8];
		        }
		        else{
			    desire_action[8] = temp_desire_action[8];
		            rx2 = temp_desire_action[8];
		        }
		        //rx3
		        rx3 = 0;
		        if(temp_desire_action[9] == -1){
			    desire_action[9] = old_action[9];
			    rx3 = old_action[9];
		            
		        }
		        else{
			    desire_action[9] = temp_desire_action[9];
		            rx3 = temp_desire_action[9];
		        }
		        
		        //command_action
		        command_action[0] = rx0;
		        command_action[1] = rx1;
		        command_action[2] = rx2;
		        command_action[3] = rx3;
				ROS_INFO("command:");
				for(int i=0; i<4; i++){
					ROS_INFO("%d ", command_action[i]);
				}
				ROS_INFO("current:");
				int j=0;
				for(j=0; j<4; j++){
					ROS_INFO("%d ", current_action[j]);
				}
				
				ROS_INFO ("mission: %s ", goap_srv.response.mission_name.c_str());
				ROS_INFO ("child mission: %s ", goap_srv.response.mission_child_name.c_str());
				action_done = false;
				if(command_finish()){
					ROS_INFO("finish");
					//old_action
					for(int i = 0; i < 10; i ++){
					    old_action[i] = desire_action[i];
					}
					action_done = true;
				}
				else{
					ROS_INFO("ing or fail");
				}
				goap_srv.request.action_done = action_done;
		*/
		//-------------------------------------------------------------------------path

				path_srv.request.my_pos_x = my_pos_x;
		        path_srv.request.my_pos_y = my_pos_y;

		        path_srv.request.goal_pos_x = goap_srv.response.pos[0];
		        path_srv.request.goal_pos_y = goap_srv.response.pos[1];

		        path_srv.request.enemy1_x = 13000 ;
		        path_srv.request.enemy1_y = 18000 ;
		        path_srv.request.enemy2_x = 50000 ;
		        path_srv.request.enemy2_y = 50000 ;
		        path_srv.request.ally_x = 13000 ;
		        path_srv.request.ally_y = 13000 ;

		      /*  sub.n.getParam("main_frank/goal_pos_x", path_srv.request.goal_pos_x);
				sub.n.getParam("main_frank/goal_pos_y", path_srv.request.goal_pos_y);
				sub.n.getParam("main_frank/enemy1_x", path_srv.request.enemy1_x);
				sub.n.getParam("main_frank/enemy1_y", path_srv.request.enemy1_y);
				sub.n.getParam("main_frank/enemy2_x", path_srv.request.enemy2_x);
				sub.n.getParam("main_frank/enemy2_y", path_srv.request.enemy2_y);
				sub.n.getParam("main_frank/ally_x", path_srv.request.ally_x);
				sub.n.getParam("main_frank/ally_y", path_srv.request.ally_y);       */

			    /*std_msgs::Int32MultiArray for_st1 ;
			    for_st1.data.push_back(r0);
			    for_st1.data.push_back(r1);
			    for_st1.data.push_back(r2);
			    for_st1.data.push_back(r3); */
				ROS_INFO("my pos x:%d", my_pos_x);
				ROS_INFO("my pos y:%d", my_pos_y);
				ROS_INFO("my degree:%d", my_degree);

			    if(sub.client_path.call(path_srv)){
			   		ROS_INFO("speed:%d",  goap_srv.response.speed);
			    	ROS_INFO("instant degree:%d",  path_srv.response.degree);
		            ROS_INFO("goalx:%d", path_srv.request.goal_pos_x);        //also is the goap_srv.response.pos[0]
		            ROS_INFO("goaly:%d", path_srv.request.goal_pos_y);
		            ROS_INFO("final degree:%d", goap_srv.response.degree);
		        }
		        else{
		            ROS_ERROR("Failed to call path_plan");
		        }

				if(at_pos(my_pos_x, my_pos_y, my_degree, path_srv.request.goal_pos_x, path_srv.request.goal_pos_y, goap_srv.response.degree, margin, angle_margin)){
		            robot = RobotState::AT_POS;
		        }
		        else{
		             robot = RobotState::ON_THE_WAY;
		        }

		        switch(robot){

		        	case RobotState::ON_THE_WAY:
		        		ROS_INFO("---------on the way----------");
		        		distance_square = (my_pos_x - path_srv.request.goal_pos_x)*(my_pos_x - path_srv.request.goal_pos_x) + (my_pos_y - path_srv.request.goal_pos_y)*(my_pos_y - path_srv.request.goal_pos_y);
			   			//switch mode
						if(distance_square < switch_mode_distance){
				            r0 = 0x4000;                              //pos mode
				            r1 = path_srv.request.goal_pos_x;
				            r2 = path_srv.request.goal_pos_y;
				            r3 = goap_srv.response.degree;		      // ?  //final degree
							//for_st1.data.push_back(90);

							int now_desired_angle = adjust_direction(my_pos_x, my_pos_y, path_srv.request.goal_pos_x, path_srv.request.goal_pos_y);
							//ROS_INFO("now_desired_angle:%d",now_desired_angle);
							if(blocking_with_direction(lidar_be_blocked(), my_degree, now_desired_angle)){
								r0 = 0x5000;   //stop
								r1 = 0;
								r2 = 0;
								r3 = 0;
								ROS_INFO ("blocked in pos mode");                
							}
				        }
				        else{
				            r0 = 0x3000;                         	//speed mode
				            r1 = goap_srv.response.speed;			//speed  //goap_srv.response.speed
				            r2 = path_srv.response.degree;	 		//degree //instant
							//for_st1.data.push_back(90);
				            r3 = 0; 								// ?

				         	if(blocking_with_direction(lidar_be_blocked(), my_degree, path_srv.response.degree)){
								r0 = 0x5000;   //stop
								r1 = 0;
								r2 = 0;
								r3 = 0;
								ROS_INFO ("blocked in speed mode");                
							}
				        }

				        break;

		            case RobotState::AT_POS:

		            	r0=0x5000;
						r1=0;
						r2=0;
						r3=0;

		            	ROS_INFO("---------at pos----------");
		            		//rx0 hand 1234
				        rx0 = 0;
				        for(int i = 0; i < 4; i ++){
				            if(temp_desire_action[i] == -1){
				                desire_action[i] = old_action[i];
				            }
				            else{
								desire_action[i] = temp_desire_action[i];
				            }
				        }
				        for(int i = 0; i < 4; i ++){
				            rx0 += pow(10, 3-i) * desire_action[i]; //可以用嗎
				        }
				        
				        //rx1 suck 1234
				        rx1 = 0;
				        for(int i = 4; i < 8; i ++){
				            if(temp_desire_action[i] == -1){
				                desire_action[i] = old_action[i];
				            }
					    	else{
								desire_action[i] = temp_desire_action[i];
				            }
				        }
				        for(int i = 4; i < 8; i ++){
				            rx1 += pow(10, 7-i) * desire_action[i]; //可以用嗎
				        }
				        //rx2
				        rx2 = 0;
				        if(temp_desire_action[8] == -1){
					    	desire_action[8] = old_action[8];         //it will let old_action[i] = desire_action[i] work correctlly
					    	rx2 = old_action[8];
				        }
				        else{
					   		desire_action[8] = temp_desire_action[8];
				            rx2 = temp_desire_action[8];
				        }
				        //rx3
				        rx3 = 0;
				        if(temp_desire_action[9] == -1){
					    	desire_action[9] = old_action[9];
					    	rx3 = old_action[9];
				        }
				        else{
					    	desire_action[9] = temp_desire_action[9];
				            rx3 = temp_desire_action[9];
				        }
				        
				        //command_action
				        command_action[0] = rx0;
				        command_action[1] = rx1;
				        command_action[2] = rx2;
				        command_action[3] = rx3;

						ROS_INFO("command:");
						for(int i=0; i<4; i++){
							ROS_INFO("%d ", command_action[i]);
						}

						ROS_INFO("current:");
						int j=0;
						for(j=0; j<4; j++){
							ROS_INFO("%d ", current_action[j]);
						}
						
						ROS_INFO ("mission: %s ", goap_srv.response.mission_name.c_str());
						ROS_INFO ("child mission: %s ", goap_srv.response.mission_child_name.c_str());

						action_done = false;
						if(count>3 && command_finish()){
							ROS_INFO("finish");
							//old_action
							for(int i = 0; i < 10; i ++){
							    old_action[i] = desire_action[i];
							}
							action_done = true;
							count = 0;
						}
						else{
							ROS_INFO("ing or fail");
						}

						goap_srv.request.action_done = action_done;

						break;
				}
		
/*	    
	    //switch mode
	    std_msgs::Int32MultiArray for_st1 ;
        if(distance_square < switch_mode_distance){
            for_st1.data.push_back(0x4000);                         //pos mode
            for_st1.data.push_back(path_srv.request.goal_pos_x);
            for_st1.data.push_back(path_srv.request.goal_pos_y);
            for_st1.data.push_back(goap_srv.response.degree);		// ?  //final degree
			//for_st1.data.push_back(90);
        }
        else{
            for_st1.data.push_back(0x3000);                         //speed mode
            for_st1.data.push_back(200);					//speed //goap_srv.response.speed
            for_st1.data.push_back(path_srv.response.degree);	    //degree //instant
			//for_st1.data.push_back(90);
            for_st1.data.push_back(0);                              // ?
        }
*/

	/*	ROS_INFO("%d", r0);
		ROS_INFO("%d", r1);
		ROS_INFO("%d", r2);
		ROS_INFO("%d", r3);
		ROS_INFO("%d", rx0);
		ROS_INFO("%d", rx1);
		ROS_INFO("%d", rx2);
		ROS_INFO("%d", rx3);
*/
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

	    sub.pub_st1.publish(for_st1);
	    sub.pub_st2.publish(for_st2);  //sub.要加

	    //sub.status_pub.publish(stat);

	    ros::spinOnce(); //才會callback
	}
	return 0;
}
