#!/usr/bin/env python

from main_loop.srv import *
import rospy
var = 1
k = 1

def handle_add_two_ints(req):
	print "~~~~~~"
	print "Returning [%s + %s = %s]"%(req.a, req.b, (req.a + req.b))
	return AddTwoIntsResponse(req.a + req.b)

def add_two_ints_server():
	rospy.init_node('add_two_ints_server')
	s = rospy.Service('add_two_ints', AddTwoInts, handle_add_two_ints)
	while var == 1 :
		k = 2 
		
		

if __name__ == "__main__":
    add_two_ints_server()
