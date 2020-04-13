#!/usr/bin/env python
import rospy
from main_loop.msg import *

import time

txST1_str = ""
txST2_str = ""
rxST1_str = ""
rxST2_str = ""
rate1_str = ""
rate2_str = ""
pi2ST1_rate_str = ""
pi2ST2_rate_str = ""

desire_degree = "" 
desire_speed = ""
desire_mode = ""
desire_pos_x = ""
desire_pos_y = ""
desire_servo_state = ""
desire_stepper = ""
desire_hand = ""
is_wait = ""
mission_name  = ""

status = ""
robot_state = ""
robot_case = ""
pos_x = ""
pos_y = ""
is_blocked = ""
servo_state = ""
stepper_state = ""
hand_state = ""
action_done = ""
kill_mission = ""
goal_covered_counter = ""



def from_goap_callback(data):
	global desire_degree 
	global desire_speed
	global desire_mode 
	global desire_pos_x 
	global desire_pos_y
	global desire_servo_state
	global desire_stepper 
	global desire_hand 
	global is_wait 
	global mission_name
	desire_degree = "desire_degree:" + str(data.desire_degree) 
	desire_speed = "desire_speed:" + str(data.desire_speed)
	desire_mode = "desire_mode:" + str(data.desire_mode)
	desire_pos_x = "desire_pos_x: " + str(data.desire_pos[0]) 
	desire_pos_y = "desire_pos_y: " + str(data.desire_pos[1]) 
	desire_servo_state = "desire_servo_state:" + str(data.desire_servo_state)
	desire_stepper = "desire_stepper:" + str(data.desire_stepper)
	desire_hand = "desire_hand:" + str(data.desire_hand)
	is_wait = "is_wait:" + str(data.is_wait)
	mission_name  = "mission_name:" + str(data.mission_name)

def main_state_callback(data):
	global status
	global robot_state
	global robot_case 
	global pos_x 
	global pos_y
	global is_blocked 
	global servo_state 
	global stepper_state
	global hand_state
	global action_done 
	global kill_mission 
	global goal_covered_counter 
 	status = "status:" + str(data.status) 
	robot_state = "robot_state:" + str(data.robot_state)
	robot_case = "robot_case:" + str(data.robot_case)
	pos_x = "pos_x:" + str(data.pos[0])
	pos_y = "pos_y:" + str(data.pos[1]) 
	is_blocked = "is_blocked:" + str(data.is_blocked)
	servo_state = "servo_state:" + str(data.servo_state)
	stepper_state = "stepper_state:" + str(data.stepper_state)
	hand_state = "hand_state:" + str(data.hand_state)
	action_done  = "action_done:" + str(data.action_done)
	kill_mission = "kill_mission:" + str(data.kill_mission)
	goal_covered_counter = "goal_covered_counter:" + str(data.goal_covered_counter)



txST1_sub = rospy.Subscriber('/Goap_response', from_goap, from_goap_callback)
rxST1_sub = rospy.Subscriber('/Main_state', main_state, main_state_callback)

   
def listener():

	rospy.init_node('debugg_main_and_goap', anonymous=True)
	global status
	global robot_state
	global robot_case 
	global pos 
	global is_blocked 
	global servo_state 
	global stepper_state
	global hand_state
	global action_done 
	global kill_mission 
	global goal_covered_counter 
	
	global desire_degree 
	global desire_speed
	global desire_mode 
	global desire_pos 
	global desire_servo_state
	global desire_stepper 
	global desire_hand 
	global is_wait 
	global mission_name
	rate = rospy.Rate(5)
	while not rospy.is_shutdown():
		starttime = time.time()
		print("-----------")
		print("Goap state:")
		print(desire_degree)
		print(desire_speed)
		print(desire_mode)
		print(desire_pos_x)
		print(desire_pos_y)
		print(desire_servo_state)
		print(desire_stepper)
		print(desire_hand)
		print(is_wait)
		print(mission_name)
		print("-----------")
		print("Main state:")
		print(status)
		print(robot_state)
		print(robot_case)
		print(pos_x)
		print(pos_y)
		print(is_blocked)
		print(servo_state)
		print(stepper_state)
		print(hand_state)
		print(action_done)
		print(kill_mission)
		print(goal_covered_counter)    
		rate.sleep()
		nowtime = time.time()
		print("the loop takes %.2f seconds\n" % (nowtime-starttime))


if __name__ == '__main__':
    try:
        listener()
    except rospy.ROSInterruptException:
        pass
