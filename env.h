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
    float room_size;
    int n_rooms;
    int pois_per_room;
    int poi_types;
    float hallway_width;
    int n_values;
    int coupling;

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
        vector<vector<float>> state();
        void action(vector<int> actions);
        vector<float> G();
        vector<vector<float>> D();
};

env::env(params param) : unif(0,1){
    eng.seed(p.seed);
    p=param;
    create_agents();
    create_rooms();
}

void env::create_rooms(){
    room temp_room;
    poi temp_poi;
    vector<float> vals;
    vals.resize(p.n_values);
    float x,y;
    for(int i=0;i<p.n_rooms/2;i++){
        for(int j=0;j<2;j++){
            temp_room.x1=float(i)*p.room_size;
            temp_room.x2=float(i+1)*p.room_size;
            temp_room.y1=p.hallway_width/2;
            temp_room.y2=p.hallway_width/2+p.room_size;
            if (j==1){
                temp_room.y1*=-1;
                temp_room.y2*=-1;
            }
            temp_room.doorx=(temp_room.x1+temp_room.x2)/2;
            temp_room.doory=temp_room.y1;

            for(int k=0; k<p.pois_per_room;k++){
                x=temp_room.x1+unif(eng)*p.room_size;
                y=temp_room.x1+unif(eng)*p.room_size;
                for(int q=0;q<p.n_values;q++)
                    vals[q]=unif(eng);
                temp_poi.setter(x,y,vals,p.coupling,p.n_agents,k%p.poi_types);
                temp_room.pois.push_back(temp_poi);
            }
            rooms.push_back(temp_room);
            temp_room.pois.clear()
        }
        
    }
}

void env::create_agents(){
    agent temp;
    for(int i=0;i<p.n_agents;i++){
        temp.setter(i,unif(eng)*p.hallway_width,(unif(eng)-0.5)*p.hallway_width);
        agents.push_back(temp);
    }
}

vector<vector<float>> env::state(){
//TODO
}

void env::action(vector<int> actions){
    int room_idx;
    int poi_type;
    int curr_room;
    float x,y;

    for(int i=0; i<p.n_agents;i++){
        x=agents[i].x;
        y=agents[i].y;

        room_idx=actions[i]/p.n_rooms;
        poi_type=actions[i]%p.n_rooms;
        //determin room location
        if (y<p.hallway_width/2 && y>-p.hallway_width/2)
            curr_room=-1;
        else{
            curr_room=int(x/p.room_size);
            if(x<0)
                curr_room+=p.n_rooms/2;
        }
        //movement
        if (curr_room==-1)

        else if (curr_room!=room_idx)

        else



    }
}



vector<float> G(){

}

vector<vector<float>> env::D(){

}