//===================
//加入友軍、敵人位置
//還有NS跟cup color資料
//0614 update
//===================
#include "ros/ros.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "std_msgs/String.h"
#include "std_msgs/Header.h"
#include "main_loop/agent.h"
#include "sensor_msgs/LaserScan.h"
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Int32.h>
#include "lidar_2020/alert_range.h"
#include <main_loop/position.h>
#include <main_loop/cup.h>
#include <main_loop/ns.h>

class sub_class{
    public:
        void ST1_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg);
        void ST2_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg); 
        void lidarmsg_sub_callback(const lidar_2020::alert_range::ConstPtr& msg);
        void camera_sub_callback(const main_loop::position::ConstPtr& msg);
        void status_sub_callback(const std_msgs::Int32::ConstPtr& msg);
        void strategy_sub_callback(const std_msgs::Int32::ConstPtr& msg);
        void publish_(float time);
        void change_status(int z); 
        void change_cup_color(int color[]);
        void change_ns(int ns_);
        void status_publish();
        int now_status(); 
        int now_degree();
        
        sub_class(int my_pos_x_ = 700, int my_pos_y_ = 300, int ini_status = 0);
        ~sub_class(){};

    private:
        ros::NodeHandle n;
        ros::Publisher status_pub = n.advertise<std_msgs::Int32>("pub_status",1); 
		ros::Publisher agent_pub = n.advertise<main_loop::agent>("agent_msg", 1);
        ros::Subscriber status_sub = n.subscribe<std_msgs::Int32>("update_status", 1, &sub_class::status_sub_callback,this);
        ros::Subscriber strategy_sub = n.subscribe<std_msgs::Int32>("strategy", 1, &sub_class::strategy_sub_callback,this);
		ros::Subscriber ST1_sub = n.subscribe<std_msgs::Int32MultiArray>("rxST1", 1, &sub_class::ST1_sub_callback,this);
        ros::Subscriber ST2_sub = n.subscribe<std_msgs::Int32MultiArray>("rxST2", 1, &sub_class::ST2_sub_callback,this);
        ros::Subscriber camera_sub= n.subscribe<main_loop::position>("enemy_pose", 1, &sub_class::camera_sub_callback,this);
        ros::Subscriber lidarmsg_sub= n.subscribe<lidar_2020::alert_range>("ranging_alert", 1, &sub_class::lidarmsg_sub_callback,this);
        int status_dominate = 0 ;
        int enemy1_pos_x = 0 ;
        int enemy1_pos_y = 0 ;
        int enemy2_pos_x = 0 ;
        int enemy2_pos_y = 0 ; 
        int sub_GUI_status = 0 ;
        int exact_status = 0 ;
        int degree_temp = 0 ;
        std_msgs::Int32 status;
        main_loop::agent pub_to_main;
};
void sub_class::status_sub_callback(const std_msgs::Int32::ConstPtr& msg){

	sub_GUI_status = msg->data;
    if(status_dominate==0){
        if(sub_GUI_status<4){
            change_status(sub_GUI_status);
        }else if(sub_GUI_status==4){
            status_dominate=1;
            change_status(sub_GUI_status);
        }
    }
    status.data = now_status();
    pub_to_main.status = now_status() ;
}
void sub_class::change_status(int z){
    exact_status = z ; 
}
void sub_class::change_ns(int ns_){
    pub_to_main.ns = ns_ ; 
}
int sub_class::now_status(){
    return exact_status; 
}
int sub_class::now_degree(){
    return degree_temp; 
}
void sub_class::status_publish(){
    status.data = now_status() ;
	status_pub.publish(status);
}
void sub_class::change_cup_color(int color[]){
    pub_to_main.cup_color={};
    for(int k=0;k<5;k++){
        pub_to_main.cup_color.push_back(color[k]);
    }
}
sub_class::sub_class(int my_pos_x_,int my_pos_y_, int ini_status){
    pub_to_main.my_pos_x = 700 ; 
    pub_to_main.my_pos_y = 300 ; 
    pub_to_main.my_degree = 10090 ; 
    pub_to_main.enemy1_x = 5000 ;
    pub_to_main.enemy1_y = 5000 ;
    pub_to_main.enemy2_x = 5000 ;
    pub_to_main.enemy2_y = 5000 ;
    pub_to_main.ally_x = 5000 ;
    pub_to_main.ally_y = 5000 ;
    pub_to_main.ns = 0 ; 
    status.data = ini_status;
}
void sub_class::strategy_sub_callback(const std_msgs::Int32::ConstPtr& msg){
    pub_to_main.strategy = msg->data ; 
}
void sub_class::camera_sub_callback(const main_loop::position::ConstPtr& msg){
    enemy1_pos_x =  msg->enemy1_x ;
    enemy1_pos_y =  msg->enemy1_y ;
    enemy2_pos_x =  msg->enemy2_x ;
    enemy2_pos_y =  msg->enemy2_y ;
    if(enemy1_pos_x == -1){
        enemy1_pos_x = pub_to_main.enemy1_x;
    }
    if(enemy1_pos_y == -1){
        enemy1_pos_y = pub_to_main.enemy1_y;
    }
    if(enemy2_pos_x == -1){
        enemy2_pos_x = pub_to_main.enemy2_x;
    }
    if(enemy2_pos_y == -1){
        enemy2_pos_y = pub_to_main.enemy2_y;
    }
    pub_to_main.enemy1_x = enemy1_pos_x;
    pub_to_main.enemy1_y = enemy1_pos_y;
    pub_to_main.enemy2_x = enemy2_pos_x;
    pub_to_main.enemy2_y = enemy2_pos_y;
}

void sub_class::ST1_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg){
    pub_to_main.my_pos_x = msg->data[0] ;
    pub_to_main.my_pos_y = msg->data[1] ;
    degree_temp = msg->data[2] ;
    if(degree_temp>=10000){
        pub_to_main.my_degree = degree_temp-10000 ;
    }else{
        pub_to_main.my_degree = degree_temp ;
    }
}
void sub_class::ST2_sub_callback(const std_msgs::Int32MultiArray::ConstPtr& msg){
    pub_to_main.wrist = msg->data[0] ;
    pub_to_main.hand = msg->data[1] ;
    pub_to_main.finger = msg->data[2] ;    
}
void sub_class::lidarmsg_sub_callback(const lidar_2020::alert_range::ConstPtr& msg){
    pub_to_main.emergency={};
    if(msg->header.seq>2){
        for(int j=0 ;j<8;j++){
            pub_to_main.emergency.push_back(msg->alert[j]);
        }  
    }
}
void sub_class::publish_(float time ){
    pub_to_main.time =time ; 
    status.data = now_status();
    pub_to_main.status = now_status() ;
    if(status.data!=5){
        pub_to_main.emergency={};
        for(int j_=0 ;j_<8;j_++){
            pub_to_main.emergency.push_back(false);
        }  
    }
    agent_pub.publish(pub_to_main);
}

int main(int argc, char **argv){
    ros::init(argc,argv, "agent_new_4");
    ros::NodeHandle nh;
    ros::ServiceClient client_cup = nh.serviceClient<main_loop::cup>("cup");
    ros::ServiceClient client_ns = nh.serviceClient<main_loop::ns>("ns");

    sub_class temp;
    main_loop::cup srv_cup;
    main_loop::ns srv_ns;
    ros::Time begin_time ;
    ros::Time now_time ;

    float last_clustering_time = 0 ;
    float clustering_time = 0 ; 
    float temp_timer ;
    int cup_suck = 0;
    int ns_suck = 0;
    int count = 0 ; 
    srv_cup.request.OUO = 0;
    srv_ns.request.OAO = 0;

    int color_[5]={2,0,0,0,0};
    temp.change_cup_color(color_);
    int timer = 0 ; //--->單台測試時，小雞跳轉的counter(in status 4)

    //availible form 
    //N S : 0 1 
    //color green red : 0 1 
    //cup color : 0 0 0 0 0
    //transfer form 
    //0 : don't eat
    //1 : eat data
    //2 : finish 
    //3 : eat garbage
    //10 : start time 

    while(ros::ok()){
        //when status = a ,timer reset 
        switch(temp.now_status()){
            case 4:
            timer++;
                if(count==0){
                    ROS_INFO("status=4");
                    temp.change_status(4);                        
                    srv_ns.request.OAO=10;
                    count =1;                    
                }else{
                    if(timer>800){
                        srv_cup.request.OUO = 1; //--->call cup service 
                    }
                    if( temp.now_degree() < 10000){
                        if( timer >=1000 || srv_cup.request.OUO == 2){
                            temp.change_status(5);
                            count=0;
                        }
                    }
                }
                break;

            case 5:
                if(count==0){
                    ROS_INFO("status=5");
                    temp.change_status(5);
                    begin_time =ros::Time::now();
                    srv_ns.request.OAO=10; //--->pub to side camera
                    count =1;                    
                }else{
                    if(last_clustering_time>100000){
                        temp.change_status(6);
                        count=0;
                    }
                }

                //calculate time
                now_time = ros::Time::now();
                clustering_time = (now_time - begin_time).toSec();

                if(clustering_time>30){
                    srv_ns.request.OAO=1;//--->30s call ns service 
                }
                


                break;

            case 6:

                break;

            case 7:

                break;

            default :
            
                break;
       }

        //cup service
        if(srv_cup.request.OUO==1){
            if(client_cup.call(srv_cup)){
                if(srv_cup.response.CupResult[0]!=0 && srv_cup.response.CupResult[0]!=1 ){
                    cup_suck = 1 ;     
                    ROS_INFO("cup_suck");
                }else{
                    srv_cup.request.OUO = 2 ;//finish
                    for(int k_=0;k_<5;k_++){
                        color_[k_]=srv_cup.response.CupResult[k_];
                    }
                }  
                if(srv_cup.request.OUO == 2){
                    temp.change_cup_color(color_);
                }
            }else{
                cup_suck = 1 ;     
                ROS_INFO("fail to call");
            }   
        }
        if(cup_suck == 1 ){
            srv_cup.request.OUO = 3;
        }

        //ns service 
        if(srv_ns.request.OAO==1||srv_ns.request.OAO==10){
            if(client_ns.call(srv_ns)){
                if(srv_ns.response.ns !=0 && srv_ns.response.ns!=1 ){
                    ROS_INFO("cup_suck");
                    ns_suck = 1 ;     
                }else{
                    srv_ns.request.OAO = 2 ;//finish 
                    temp.change_ns(srv_ns.response.ns);
                }  
            }else{
                ROS_INFO("fail to call");
                ns_suck = 1 ; 
            }   
        }
        if(cup_suck == 1 ){
            srv_ns.request.OAO = 3;
        }
        if(srv_ns.request.OAO==10 ){
            srv_ns.request.OAO = 0;
        }


        last_clustering_time=clustering_time;
        temp.publish_(clustering_time);
        temp.status_publish();
        ros::spinOnce();
    }
}
