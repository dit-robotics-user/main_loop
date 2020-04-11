from setting import *
'''
1 2  3  4
5 6  7  8
9 10 11 12
'''
def output_processor(output_action, left_side, right_side):
    output = [0]*15
    if output_action.iscup is True:
        for p in output_action.preconditions:
            if p is '1':
                claw_num = 2*left_side + 7  # use layer 2 - left_side + '1'
                output[claw_num] = 1
            elif p is '2':
                claw_num = 2*left_side + 6  # use layer 2 - left_side + '2'
                output[claw_num] = 1
            elif p is'3':
                claw_num = 2*right_side + 1  # use layer 2 - right_side + '3'
                output[claw_num] = 1
            else:
                claw_num = 2*right_side      # use layer 2 - right_side + '4'
                output[claw_num] = 1
    if output_action.name is 'lift_left':
        if left_side is 0:
            output[12] = 0
        elif left_side is 1:
            output[12] = 1  # lift layer 3
        elif left_side is 2:
            output[12] = 2  # lift layer 2
    elif output_action.name is 'lift_right':
        if right_side is 0:
            output[13] = 0
        if right_side is 1:
            output[13] = 1  # lift layer 3
        elif right_side is 2:
            output[13] = 2  # lift layer 2
    elif output_action.name is 'lower_right':
        if right_side is 0:
            output[12] = 0
        if right_side is 1:
            output[12] = 1  # lift layer 3
        elif right_side is 2:
            output[12] = 2  # lift layer 2
    elif output_action.name is 'lower_right':
        if right_side is 0:
            output[13] = 0
        if right_side is 1:
            output[13] = 1  # lift layer 3
        elif right_side is 2:
            output[13] = 2  # lift layer 2
    elif output_action.name is 'lower':
        if right_side is 0:
            output[12] = 0
            output[13] = 0
        if right_side is 1:
            output[12] = 1
            output[13] = 1  # lift layer 3
        elif right_side is 2:
            output[12] = 2
            output[13] = 2  # lift layer 2
    elif output_action.name is 'lift':
        if right_side is 0:
            output[12] = 0
            output[13] = 0
        if right_side is 1:
            output[12] = 1
            output[13] = 1  # lift layer 3
        elif right_side is 2:
            output[12] = 2
            output[13] = 2  # lift layer 2
    elif output_action.name is 'hand_down':
        output[14] = 2
    elif output_action.name is 'hand_up':
        output[14] = 0
    if output_action.name is 'open':
        for p in output_action.preconditions:
            if p is '1':
                claw_num = 4*left_side   # use layer 2 - left_side + '1'
                output[claw_num] = 0
            elif p is '2':
                claw_num = 4*left_side + 1  # use layer 2 - left_side + '2'
                output[claw_num] = 0
            elif p is'3':
                claw_num = 4*right_side + 2  # use layer 2 - right_side + '3'
                output[claw_num] = 0
            else:
                claw_num = 4*right_side + 3  # use layer 2 - right_side + '4'
                output[claw_num] = 0
    return output


action_done = True  # <----
my_pos = (3, 3)  # <----
output = [0]*15  # --->
output_speed = 0  # --->
output_mode = -1  # --->
output_degree = -1  # --->
output_position = (0, 0)  # --->
output_wait = True  # --->
demo_path = setting(1)
goal = []
path_done = False
left_side = 0
right_side = 0

while len(demo_path) > 0:

    path_done = False
    path = demo_path[0]
    if path.iscup is True:
        if path.grab_mode is 2:  # speed mode
            path.tangent_point_calculation(my_pos, stretch_factor=5)
    output = output_processor(path, left_side, right_side)
    output_degree = path.degree
    output_speed = path.speed
    output_mode = path.grab_mode
    output_position = path.position
    output_wait = path.iswait

    while path_done is False:
        if action_done is True:
            print(path.name)
            print(path.position)
            print(path.degree)
            print()
            if path.name is 'lift_left':
                left_side += 1
            elif path.name is 'lift_right':
                right_side += 1
            elif path.name is 'lower_left':
                left_side -= 1
            elif path.name is 'lower_right':
                right_side -= 1
            elif path.name is 'lower':
                left_side -= 1
                right_side -= 1
            elif path.name is 'lift':
                left_side += 1
                right_side += 1
            path_done = True
            if len(demo_path) > 1:
                demo_path.remove(demo_path[0])




