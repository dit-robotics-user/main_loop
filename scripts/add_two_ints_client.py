#!/usr/bin/env python

import sys
import rospy
from main_loop.srv import *

def add_two_ints_client(x, y):
    rospy.wait_for_service('add_two_ints')
    try:
        add_two_ints = rospy.ServiceProxy('add_two_ints', AddTwoInts)
        resp1 = add_two_ints(x, y)
        return resp1.sum
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

def usage():
    return "%s [x y]"%sys.argv[0]

if __name__ == "__main__":

	x = 3
	y = 2

	print "Requesting %s+%s"%(x, y)
	print "%s + %s = %s"%(x, y, add_two_ints_client(x, y))
