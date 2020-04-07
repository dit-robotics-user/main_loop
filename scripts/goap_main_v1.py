#!/usr/bin/env python

from main_loop.srv import *
import rospy

            
def handle_return_to_main(req):

	print "in goap_test.py"
	return [1,0,0]

def add_two_ints_server():
	rospy.init_node('goap_test')
	rospy.Service('goap_test_v1', goap_, handle_return_to_main)
	print "Ready to use goap test."
	rospy.spin()
    

if __name__ == "__main__":
	add_two_ints_server()
