#include "ros/ros.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "std_msgs/String.h"
#include "std_msgs/Header.h"
#include "main_loop/string_with_header.h"
#include "main_loop/path_planning.h"
#include "main_loop/agent.h"
#include <std_msgs/Int32MultiArray.h>
#include "lidar_2020/alert_range.h"


class sub_class{
    public:
        void ST1_sub_Callback(const std_msgs::Int32MultiArray::ConstPtr& msg); 
        void ST1_sub_Callback(const lidar_2020::alert_range::ConstPtr& msg);

    private:
        ros::NodeHandle n;
		ros::Publisher agent_pub;
		ros::Subscriber ST1_sub;
        ros::Subscriber ST2_sub;
        ros::Subscriber lidarmsg_sub;
        main_loop::agent pub_to_main;
        ros::Subscriber ST1_sub = n.subscribe("rxST1", 1, ST1_sub_callback);
//        ros::Subscriber ST2_sub = n.subscribe("agent_pub", 1, callback);
        ros::Subscriber lidarmsg_sub = n.subscribe("range_alert", 1, lidarmsg_sub_callback);
        agent_pub = nh.advertise<lidar_2020::agent>("agent_msg", 1);
}

void sub_class::ST1_sub_Callback(const std_msgs::Int32MultiArray::ConstPtr& msg){
    
    pub_to_main.my_pos_x = msg->data[0] ;
    pub_to_main.my_pos_x = msg->data[1] ;
}
void sub_class::ST1_sub_Callback(const lidar_2020::alert_range::ConstPtr& msg){

    pub_to_main.emergency = msg->alert ; 
}

int main(int argc, char **argv){
    ros::init(argc,argv, "simple_octagonal_ranging");
    ros::Time::init();
    double begin_time =ros::Time::now().toSec();
    ros::Rate loop_rate(50);
    while(ros::ok()){
        double clustering_time = ros::Time::now().toSec () - begin_time ;
        pub_to_main.time = clustering_time ; 
        agent_pub.publish(pub_to_main);

        ros::spinOnce();
        loop_rate.sleep();
    }
    }

}