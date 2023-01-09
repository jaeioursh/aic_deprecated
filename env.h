#include <vector>
#include <random>

#include "poi.h"
#include "agent.h"

struct room{
    vector<poi> pois;
    int pois;
    float x1,x2,y1,y2;
    float doorx,doory;
};

struct params{
    unsigned long seed;
    int n_agents;
    int n_pois;
    float room_size;
    int n_rooms;
    int pois_per_room;
    int poi_types;
    float hallway_width;

};

class env {
    public:
        vector<room> rooms;
        vector<agent> agents;
        int seed;
        default_random_engine eng;
        uniform_real_distribution<float> unif;
        params p;


        env(params param);
        void create_rooms();
        void create_agents();
};

env::env(params param) : unif(0,1){
    eng.seed(p.seed);
    p=param;
}

void env::create_rooms(){
    room temp;
}

void env::create_agents(){
    agent temp;
    for(int i=0;i<p.n_agents;i++){
        temp.setter(i,unif(eng)*p.hallway_width,unif(eng)*p.hallway_width);
        agents.push_back(temp);
    }
}

