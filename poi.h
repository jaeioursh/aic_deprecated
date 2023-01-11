#pragma once

#include <vector>
#include <algorithm>

using namespace std;

class poi {
    public:
        vector<float> vals;
        vector<float> zero_vec;
        vector<float> d_vec;
        int n_agents;
        int val_dim;
        int observing;
        bool observed;
        bool active;
        float x;
        float y;
        int type;
        int couple;

        void refresh();
        void reset();
        void observe(int idx);
        void setter(float X, float Y, vector<float> values,int couple_req, int agents,int poi_type);
        poi(){};
};

void poi::reset(){
    active=1;
    observing=0;
    observed=0;
    fill(d_vec.begin(), d_vec.end(), 0);
}


void poi::observe(int idx){
    observing+=1;
    d_vec[idx]=1;
}

void poi::refresh(){
    if (active){
        if (observing>=couple)
            active=0;
            observed=1;
        if (observing != couple)
            fill(d_vec.begin(), d_vec.end(), 0);
        observing=0;
    }
}




void poi::setter(float X, float Y, vector<float> values,int couple_req,int agents,int poi_type){
    n_agents=agents;
    d_vec.resize(n_agents);
    type=poi_type;
    x=X;
    y=Y;
    val_dim=values.size();
    zero_vec.resize(val_dim);
    fill(zero_vec.begin(), zero_vec.end(), 0);
    couple=couple_req;
    reset();
}

