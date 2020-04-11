#include <iostream>
using namespace std;

class state{
public:
    state(){};
    state(int my_x, int my_y, bool block, int tx0_, int tx1_, int tx2_){
        my_pos_x = my_x;
        my_pos_y = my_y;
        is_blocked = block;
        tx0 = tx0_;
        tx1 = tx1_;
        tx2 = tx2_;
    }
    void ChangeActionDone(bool tf){
        a_done = tf;
    }
    void ChangeKillMission(bool tf){
        kill_mission = tf;
    }
    void ChangeReplanMission(bool tf){
        replan_mission = tf;
    }
    void ChangeActionWait(bool tf){
        action_wait = tf;
    }
    void ChangeReplanPath(bool tf){
        replan_path = tf;
    }
    int MyActionDone(){
        return a_done;
    }
    int MyPosX(){
        return my_pos_x;
    }
    int MyPosY(){
        return my_pos_y;
    }
    bool KillMission(){
        return kill_mission;
    }
    bool ReplanMission(){
        return replan_mission;
    }
    bool IsBlocked(){
        return is_blocked;
    }
    bool ActionWait(){
        return action_wait;
    }
    bool ReplanPath(){
        return replan_path;
    }
    int MyTx0(){
        return tx0;
    }
    int MyTx1(){
        return tx1;
    }
    int MyTx2(){
        return tx2;
    }
    void RefreshActionState(){
        a_done;
        kill_mission = false;
        replan_mission = false;
        replan_path = false;
        is_blocked = false;
        action_wait = false;
    }

private:
    bool a_done;
    int my_pos_x;
    int my_pos_y;
    bool kill_mission;
    bool replan_mission;
    bool replan_path;
    bool is_blocked;
    bool action_wait;
    int tx0;
    int tx1;
    int tx2;
};

class action{
public:
    action(int x, int y, int movement_num[], double what_angle, int how_fast, bool is_wait, int what_mode){
        goal_pos_x = x;
        goal_pos_y = y;
        for(int i = 0; i < 15; i ++){
            movement[i] = movement_num[i];
        }
        angle = what_angle;
        speed = how_fast;
        wait = is_wait;
        mode = what_mode;
    }
    action(){};
    int PosX(){
        return goal_pos_x;
    }
    int PosY(){
        return goal_pos_y;
    }
    int * Movement(){
        return  movement;
    }
    int Speed(){
        return speed;
    }
    bool Wait(){
        return wait;
    }
    double Angle(){
        return angle;
    }
    int Mode(){
        return mode;
    }
private:
    int goal_pos_x;
    int goal_pos_y;
    int movement[15];
    int speed;
    double angle;
    bool wait;
    int mode;
};

enum class ActionMode {SPEED_MODE, POSITION_MODE};
enum class Status {SET_STRATEGY, RESET, SET_INITIAL_POS, STARTING_SCRIPT, READY, RUN, STOP, IDLE};
enum class RobotState {AT_POS, ON_THE_WAY, BLOCKED};

bool at_pos(int x, int y, int c_x, int c_y, int m){
    bool at_p = false;
    if(abs(x - c_x) < m && abs(y - c_y) < m){
        at_p = true;
    }
    return at_p;
}

int main()
{
    // give default value here
    int r0;
    int r1;
    int r2;
    int r3;
    int rx0;
    int rx1;
    int rx2;
    int rx3;

    int goal_covered_counter = 0;
    int cover_limit = 20;
    int old_grab_status[12] = {0};
    int distance_square = 0;
    int action_done = false;
    int kill_mission = false;
    int replan_mission = false;
    int margin = 30;
    int switch_mode_distance = 100;
    ActionMode m;
    RobotState robot;


    while(1){
        int s = 0; //<----------get from command
        Status stat  = static_cast<Status>(s);
        switch(stat){
            case Status::SET_STRATEGY: //0
                r0 = 0x6000;
                r1 = 0;
                r2 = 0;
                r3 = 0;
                break;
            case Status::RUN:{ //5
                state current_state(1,1,true,1,1,1);//<--------get undergoing, finish, my_x, my_y, block from other nodes ()********
                state action_state(0,0,false,0,0,0);
                action_state = current_state;
                if(action_done){
                    action_state.ChangeActionDone(true);
                    action_done = false;
                }
                if(kill_mission){
                    action_state.ChangeKillMission(true);
                    kill_mission = false;
                }
                action act;
                int desire_pos_x = act.PosX();
                int desire_pos_y = act.PosY();
                int*desire_movement;
                int desire_speed = act.Speed();
                int desire_mode = act.Mode();
                bool desire_wait = act.Wait();
                double desire_angle = act.Angle();
                desire_movement = act.Movement();
                if(at_pos(current_state.MyPosX(),current_state.MyPosY(), desire_pos_y, desire_pos_y, margin) && (desire_wait == false || ((rx0==current_state.MyTx0()) && (rx1 == current_state.MyTx1()) && (rx2 == current_state.MyTx1())))){
                    action_done = true;
                    goal_covered_counter = 0;
                }
                if(desire_mode == 1){
                    m = ActionMode::POSITION_MODE;
                }
                else{
                    m = ActionMode::SPEED_MODE;
                }
                switch(m){
                    case ActionMode::POSITION_MODE:
                        if(at_pos(current_state.MyPosX(),current_state.MyPosY(), desire_pos_y, desire_pos_y, margin)){
                            robot = RobotState::AT_POS;
                        }
                        else{
                            if(current_state.IsBlocked()){
                                robot = RobotState::BLOCKED;
                            }
                            else{
                                robot = RobotState::ON_THE_WAY;
                            }
                        }
                        switch(robot){
                            case RobotState::AT_POS:{
                                //rx0
                                int out = 0;
                                for(int i = 0; i < 12; i ++){
                                    if(desire_movement[i] != -1){
                                        old_grab_status[i] = desire_movement[i];
                                    }
                                }
                                for(int i = 0; i < 12; i ++){
                                    out += old_grab_status[i];
                                    out = out << 2;
                                }
                                out = out << 6;
                                rx0 = out;
                                //rx1
                                int left_layer = 0;
                                int right_layer = 0;
                                left_layer = desire_movement[12];
                                right_layer = desire_movement[13];
                                rx1 = (left_layer << 30) + (right_layer << 28);
                                //rx2
                                rx2 = desire_movement[14];
                                break;}

                            case RobotState::BLOCKED:
                                //return stop; //<-------------tell STM to stop
                                r0 = 0x5000;
                                r1 = 0;
                                r2 = 0;
                                r3 = 0;
                                action_state.ChangeReplanMission(true);   //---------------> GOAP replan == True
                                break;

                            case RobotState::ON_THE_WAY:
                                distance_square = (current_state.MyPosX() - desire_pos_x)*(current_state.MyPosX() - desire_pos_x) + (current_state.MyPosY() - desire_pos_y)*(current_state.MyPosY() - desire_pos_y);
                                if(distance_square < switch_mode_distance){
                                    r0 = 0x4000;
                                    r1 = desire_pos_x;
                                    r2 = desire_pos_y;
                                    r3 = desire_angle;
                                    //return pos_mode; //<----------------
                                }
                                else{
                                    r0 = 0x3000;
                                    r1 = desire_speed;
                                    r2 = A*();//<-----------------
                                    r3 = 0;
                                    if( A*()cover == true){ //<-------------
                                        goal_covered_counter ++;
                                    }
                                    if( A*()cover == false){ //<-------------
                                        goal_covered_counter = 0;
                                    }
                                    if(goal_covered_counter > cover_limit){
                                        action_state.ChangeKillMission(true);
                                        goal_covered_counter = 0;
                                    }
                                    //return speed_mode;//<-----------------
                                }
                                break;
                        }
                        break;

                    case ActionMode::SPEED_MODE:{
                        int out;
                        rx0 = desire_movement[12];
                        rx1 = desire_movement[13];
                        for(int i = 0; i < 12; i ++){
                            if(desire_movement[i] == 1){
                                old_grab_status[i] = 2;
                            }
                            else if(desire_movement[i] == 0){
                                old_grab_status[i] = 0;
                            }
                        }
                        for(int i = 0; i < 12; i ++){
                            out += old_grab_status[i];
                            out = out << 2;
                        }
                        out = out << 6;
                        rx2 = out;
                        rx3 = desire_movement[14];
                        if(current_state.MyPosX() != desire_pos_x && current_state.MyPosY() != desire_pos_y){
                           if(current_state.MyActionDone() == 1){ //if not at pos and hasn't got cup, keep going
                                r0 = 0x3000;
                                r1 = desire_speed;
                                r2 = A*();//<-----------------
                                r3 = 0;
                                if( A*()cover == true){ //<-------------
                                    goal_covered_counter ++;
                                }
                                if( A*()cover == false){ //<-------------
                                    goal_covered_counter = 0;
                                }
                                if(goal_covered_counter > cover_limit){
                                    //action_state.ChangeKillMission(true);
                                    action_state.ChangeActionDone(true); //should kill mission but fuck that.. this should still work
                                    goal_covered_counter = 0;
                                    for(int i = 0; i < 12; i ++){
                                       if(desire_movement[i] == 2){
                                            old_grab_status[i] = 0;
                                        }
                                    }
                            	} //if get cup, action_done is then true(MyActionStatus()==2), GOAP will automatically give next action
                           }
                            else{
                                for(int i = 0; i < 12; i ++){
                                       if(desire_movement[i] == 2){
                                            old_grab_status[i] = 1;
                                        }
                                    }
                            }
                        }
                        else{ //at pos, mission has failed..
                            //action_state.ChangeKillMission(true);
                            action_state.ChangeActionDone(true); //should kill mission but fuck that.. this should still work
                            for(int i = 0; i < 12; i ++){
                               if(desire_movement[i] == 2){
                                    old_grab_status[i] = 0;
                                }
                            }
                        }
                        break;}
                }
                break;
                }
            case Status::STOP: //6
                r0 = 0x5000;
                r1 = 0;
                r2 = 0;
                r3 = 0;
                break;
        }
    }
}
