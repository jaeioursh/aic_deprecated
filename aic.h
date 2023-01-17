#pragma once

#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

#include "poi.h"
#include "agent.h"

struct room{
    vector<poi> pois;
    float x1,x2,y1,y2;
    float doorx,doory;
};

struct params{
    unsigned long seed;
    int room_decay;
    int n_agents;
    float room_width;
    float room_height;
    int n_rooms;
    int pois_per_room;
    int poi_types;
    int agent_types;
    float hallway_width;
    int n_values;
    int coupling;

};

class aic {
    public:
        vector<room> rooms;
        vector<agent> agents;
        vector<vector<float>> s;

        vector<vector<int>> room_timers;
        vector<float> global_state;

        int seed;
        default_random_engine eng;
        uniform_real_distribution<float> unif;
        params p;


        aic(params param);
        void gen_global_state();
        void create_rooms();
        void reset();
        void create_agents();
        vector<vector<float>> state();
        void action(vector<int> actions);
        vector<float> G();
        vector<vector<float>> D();
};

aic::aic(params param) : unif(0,1){
    vector<int> temp;
    
    p=param;
    eng.seed(p.seed);
    create_agents();
    create_rooms();
    global_state.resize(p.n_rooms*(p.poi_types+p.agent_types));
    room_timers.resize(p.n_agents);
    temp.resize(p.n_rooms);
    for(int i=0;i<p.n_agents;i++)
        room_timers[i]=temp;
    s.resize(p.n_agents);
    for(int i=0; i<p.n_agents;i++)
        s[i].resize(p.n_rooms*(p.poi_types+p.agent_types));
        
    reset();
}

void aic::reset(){
    for(int i=0; i<p.n_agents;i++){
        fill(room_timers[i].begin(), room_timers[i].end(), 0);
        fill(s[i].begin(),s[i].end(),0);
    }
    
    fill(global_state.begin(), global_state.end(), 0);
    for(int i=0; i<p.n_rooms;i++){
        for(int j=0;j<rooms[i].pois.size();j++){
            rooms[i].pois[j].reset();
        }
    }
    for(int i=0;i<p.n_agents;i++)
        agents[i].reset();

}


void aic::create_rooms(){
    room temp_room;
    poi temp_poi;
    vector<float> vals;
    vals.resize(p.n_values);
    float x,y;
    for(int i=0;i<p.n_rooms/2;i++){
        for(int j=0;j<2;j++){
            //set up room dimensions
            temp_room.x1=float(i)*p.room_width;
            temp_room.x2=float(i+1)*p.room_width;
            temp_room.y1=p.hallway_width/2;
            temp_room.y2=p.hallway_width/2+p.room_height;
            if (j==1){
                temp_room.y1*=-1;
                temp_room.y2*=-1;
            }
            temp_room.doorx=(temp_room.x1+temp_room.x2)/2;
            temp_room.doory=temp_room.y1;

            //add pois
            for(int k=0; k<p.pois_per_room;k++){
                x=temp_room.x1+unif(eng)*(temp_room.x2-temp_room.x1);
                y=temp_room.y1+unif(eng)*(temp_room.y2-temp_room.y1);
                for(int q=0;q<p.n_values;q++)
                    vals[q]=unif(eng);
                temp_poi.setter(x,y,vals,p.coupling,p.n_agents,k%p.poi_types);
                temp_room.pois.push_back(temp_poi);
            }
            rooms.push_back(temp_room);
            temp_room.pois.clear();
        }
        
    }
}

void aic::create_agents(){
    agent temp;
    for(int i=0;i<p.n_agents;i++){
        temp.setter(i,unif(eng)*p.hallway_width,(unif(eng)-0.5)*p.hallway_width);
        agents.push_back(temp);
    }
}

void aic::gen_global_state(){
    int idx;
    fill(global_state.begin(), global_state.end(), 0);
    int i;
    for(i=0;i<p.n_rooms;i++){
        for(int j=0;j<rooms[i].pois.size();j++){
            idx=i*(p.agent_types+p.poi_types)+rooms[i].pois[j].type;
            global_state[idx]+=1;
        }
    }
    for(int j=0;j<p.n_agents;j++){
        i=agents[j].room;
        idx=i*(p.agent_types+p.poi_types)+p.poi_types+agents[j].type;
        global_state[idx]+=1;
    }
}

vector<vector<float>> aic::state(){
    
    int rm_idx;
    gen_global_state();
    s.resize(p.n_agents);
    for(int i=0; i<p.n_agents;i++){
        if (agents[i].room>=0)
            room_timers[i][agents[i].room]=p.room_decay;

        s[i].resize(p.n_rooms*(p.poi_types+p.agent_types));
        for(int j=0;j<(p.n_rooms*(p.poi_types+p.agent_types));j++){
            rm_idx=j/(p.poi_types+p.agent_types);
            if (rm_idx==agents[i].room)
                s[i][j]=global_state[j];
            if(room_timers[i][rm_idx]==0)
                s[i][j]=0;

        }

        for(int j=0;j<p.n_rooms;j++)
            if (room_timers[i][j]>0)
               room_timers[i][j]--; 
    }
    
    return s;
}

void aic::action(vector<int> actions){
    int room_idx;
    int poi_type;
    int curr_room;
    float x,y;

    for(int i=0; i<p.n_agents;i++){
        x=agents[i].x;
        y=agents[i].y;

        room_idx=actions[i]/p.poi_types;
        poi_type=actions[i]%p.poi_types;
        //determine room location
        if (y<p.hallway_width/2 && y>-p.hallway_width/2)
            curr_room=-1;
        else{
            curr_room=int(x/p.room_width)*2;
            if(y<0)
                curr_room++;
        }
        //movement
        if (curr_room==-1){ //hallway -> room
            agents[i].set_goal(rooms[room_idx].doorx,rooms[room_idx].doory);
        }
                   
        else if (curr_room!=room_idx){ //room -> hallway
            agents[i].set_goal(rooms[curr_room].doorx,rooms[curr_room].doory);
        }

        else{ //in room
            for(int j=0;j<rooms[room_idx].pois.size();j++){
                if (rooms[room_idx].pois[j].type==poi_type && rooms[room_idx].pois[j].active){
                    agents[i].set_goal(rooms[room_idx].pois[j].x,rooms[room_idx].pois[j].y);
                    agents[i].observe(rooms[room_idx].pois[j]);
                    break;
                }
            }
        }
        
        agents[i].move();
        agents[i].room=curr_room;
        

    }
    for(int i=0; i<p.n_rooms;i++)
        for(int j=0;j<rooms[i].pois.size();j++)
            rooms[i].pois[j].refresh();
    
}



vector<float> aic::G(){
    vector<float> g;
    g.resize(p.n_values);
    fill(g.begin(), g.end(), 0);
    for(int i=0; i<p.n_rooms;i++){
        for(int j=0;j<rooms[i].pois.size();j++){
            if (rooms[i].pois[j].observed){
                for(int k=0;k<p.n_values;k++)
                    g[k]+=rooms[i].pois[j].vals[k];
            }
        }
    }
    return g;
}

vector<vector<float>> aic::D(){

    vector<vector<float>> d;

    
    d.resize(p.n_agents);
    for(int i=0; i<p.n_agents;i++){
        d[i].resize(p.n_values);
        fill(d[i].begin(), d[i].end(), 0);
    }


    for(int i=0; i<p.n_rooms;i++){
        for(int j=0;j<rooms[i].pois.size();j++){
            if (rooms[i].pois[j].observed){
                for(int k=0;k<p.n_agents;k++)
                    for(int m=0;m<p.n_values;m++)
                        d[k][m]+=rooms[i].pois[j].vals[m]*rooms[i].pois[j].d_vec[k];
            }
        }
    }
    return d;

}