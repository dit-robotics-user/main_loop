from setting import *


def output_processor(output_action, left_side, right_side):
    output = [-1]*15
    if output_action.iscup is True:
        for p in output_action.preconditions:
            if p is '1':
                claw_num = 2*(2 - left_side) + 1  # use layer 2 - left_side + '1'
                output[claw_num-1] = 1
            elif p is '2':
                claw_num = 2*(2 - left_side) + 2  # use layer 2 - left_side + '2'
                output[claw_num-1] = 1
            elif p is'3':
                claw_num = 2*(2 - left_side) + 7  # use layer 2 - right_side + '3'
                output[claw_num-1] = 1
            else:
                claw_num = 2 * (2 - left_side) + 8  # use layer 2 - right_side + '4'
                output[claw_num-1] = 1
    if output_action.name is 'lift_left':
        if left_side is 1:
            output[12] = 3  # lift layer 3
        else:
            output[12] = 2  # lift layer 2
    elif output_action.name is 'lift_right':
        if right_side is 1:
            output[13] = 3  # lift layer 3
        else:
            output[13] = 2  # lift layer 2
    return output


action_done = True  # <----
my_pos = (3, 3)  # <----
output = [-1]*15  # --->
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

    while path_done is False:
        if action_done is False:
            output = output_processor(path, left_side, right_side)
        else:
            print(path.name)
            print(path.position)
            print(path.degree)
            print()
            if path.name is 'lift_left':
                left_side += 1
            elif path.name is 'lift_right':
                right_side += 1
            path_done = True
            demo_path.remove(demo_path[0])




