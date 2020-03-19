#include <iostream>

using namespace std;

class state{
public:
    state();
    state(int action_status, int my_x, int my_y, bool block){
        status = action_status;
        my_pos_x = my_x;
        my_pos_y = my_y;
        is_blocked = block;
    }
    void ChangeActionStatus(int state){
        if(state != 0 || state != 1 || state != 2){
            status = 0;
        }
        else{
            status = state;
        }
        //0 = nothing going on
        //1 = undergoing
        //2 = finished
    }
    void KillMissionTrue(){
        kill_mission = true;
    }
    void ReplanMissionTrue(){
        replan_mission = true;
    }
    void ActionWaitTrue(){
        action_wait = true;
    }
    void ReplanPathTrue(){
        replan_path = true;
    }
    void KillMissionFalse(){
        kill_mission = false;
    }
    void ReplanMissionFalse(){
        replan_mission = false;
    }
    void ActionWaitFalse(){
        action_wait = false;
    }
    void ReplanPathFalse(){
        replan_path = false;
    }
    void StateChageFalse(){
        state_change = false;
    }
    int MyActionStatus(){
        return status;
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
    void RefreshActionState(){
        status = 0;
        kill_mission = false;
        replan_mission = false;
        replan_path = false;
        is_blocked = false;
        action_wait = false;
    }

private:
    int status;
    bool kill_mission;
    bool replan_mission;
    bool replan_path;
    int my_pos_x;
    int my_pos_y;
    bool is_blocked;
    bool action_wait;
    bool state_change;
};

int main(){
    state current_state;
    return 0 ;


}