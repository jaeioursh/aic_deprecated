#include "env.h"

#include <iostream>
using namespace std;

int main(){
    params p;
    p.seed=0;
    p.room_decay=10;
    p.n_agents=5;
    p.room_size=10;
    p.n_rooms=6;
    p.pois_per_room=2;
    p.poi_types=2;
    p.hallway_width=2;
    p.n_values=3;
    p.coupling=1;

    env test(p);
    for(int i;i<p.n_agents;i++)
        cout<< test.agents[i].x << ", "<< test.agents[i].y << endl;

}