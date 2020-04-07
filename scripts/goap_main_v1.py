#!/usr/bin/env python

from main_loop.srv import *
import rospy
from setting import *

demo_path = []
demo_path = setting(1, demo_path)
output = 0
goal = []
path_done = False
            
def handle_return_to_main(req):
	action_done = True  # <----
	my_pos = (req.pos[0],req.pos[1])  # <----
	
	while len(demo_path) > 0:
		path_done = False
		path = demo_path[0]
		if path.iscup is True:
			if path.grab_mode is 2:  # speed mode
				path.tangent_point_calculation(my_pos, stretch_factor=5)

		while path_done is False:
			if action_done is False:
				output = path
			else:
				print(path.name)
				print(path.position)
				print(path.degree)
				print()
				path_done = True
				demo_path.remove(demo_path[0])
	print "in goap_test.py"
	return [1,0,0]

def add_two_ints_server():
	rospy.init_node('goap_test')
	rospy.Service('goap_test_v1', goap_, handle_return_to_main)
	print "Ready to use goap test."
	rospy.spin()
    

if __name__ == "__main__":
	add_two_ints_server()
