import math


def calculate_unit_vector(start, goal):  # give two points and calculates the unit vector
    x = goal[0] - start[0]
    y = goal[1] - start[1]
    length = math.sqrt(x*x + y*y)
    unit_vector = (x/length, y/length)
    return unit_vector


def calculate_degree(start, goal):  # from vector then calculates the degree from 0-360
    num = goal[1] - start[1]
    den = goal[0] - start[0]
    pi = 3.1415926
    radius = 0
    if num != 0 and den != 0:
        if num > 0 and den > 0:  # 1 quadrant
            radius = math.atan(num / den)
        elif num > 0:  # 2, 3 quadrant
            radius = pi + math.atan(num / den)
        else:  # 4 quadrant
            radius = 2*pi + math.atan(num / den)
    elif num == 0 and den > 0:
        radius = 0  # slope is zero
    elif num == 0 and den < 0:
        radius = pi  # slope is zero
    elif num > 0 and den == 0:
        radius = pi/2  # slope is infinity
    elif num < 0 and den == 0:
        radius = 3*pi/2  # slope is infinity
    else:
        print('calculate degree error(num, den = 0)')
    degree = int(radius * 180 / pi)  # switches radius to degree
    print('degree:', degree)
    return degree

# def tangent_point_calculation(action, my_pos, radius):////////////////////////////////////////////////////
# action.is_right//////////////////////////////////////////////////////////////////////////////////////////





