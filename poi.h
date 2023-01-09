
#include <vector>
#include <algorithm>

using namespace std;

class poi {
    public:
        vector<float> vals;
        vector<float> zero_vec;
        vector<bool> d_vec;
        int n_agents;
        int val_dim;
        int observing;
        bool active;
        float x;
        float y;
        int type;
        int couple;

        void refresh();
        void reset();
        void observe(int idx);
        vector<float> G();
        vector<bool> D();
        void setter(float X, float Y, vector<float> values,int couple_req, int agents,int poi_type)
        poi(){};
};

void poi::reset(){
    active=1;
    observing=0;
    fill(d_vec.begin(), d_vec.end(), 0);
}


void poi::observe(int idx){
    observing+=1;
    d_vec[idx]=1;
}

void poi::refresh(){

    if (observing>=couple)
        active=0;
    fill(d_vec.begin(), d_vec.end(), 0);
    observing=0;
}

vector<float> poi::G(){
    if (active){
        return zero_vec;
    }
    return vals;
}

vector<bool> poi::D(){
    return d_vec;
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

