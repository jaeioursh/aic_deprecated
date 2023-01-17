#include "aic.h"

#include <iostream>
#include <algorithm>
using namespace std;

int main(){
    params p;
    p.seed=0;
    p.room_decay=10;
    p.n_agents=2;
    p.room_height=10;
    p.room_width=4;
    p.n_rooms=6;
    p.pois_per_room=2;
    p.poi_types=2;
    p.hallway_width=2;
    p.agent_types=1;
    p.n_values=1;
    p.coupling=2;
    aic test(p);
    vector<int> act;
    act.resize(p.n_agents);
    vector<vector<float>> s;
    for (int j=0;j<6;j++){
        fill(act.begin(), act.end(), j);
        for(int i=0;i<15;i++){
            test.action(act);
            s = test.state();
        }
    }

}