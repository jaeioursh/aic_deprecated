#pragma once

#include "poi.h"
#include <math.h>

class agent {
    public:
        float x;
        float y;
        float type;
        float _x;
        float _y;
        float goalx;
        float goaly;
        int idx;
        int room;
        poi p;
        

        void setter(int index,float startx, float starty);
        void set_goal(float xx, float yy);
        void reset();
        void move();
        void observe(poi &p);


};

void agent::setter(int index,float startx, float starty){
    type=0;
    idx=index;
    _x=startx;
    _y=starty;
    reset();
}



void agent::set_goal(float xx, float yy){
    goalx=xx;
    goaly=yy;
}

void agent::reset(){
    goalx=0;
    goaly=0;
    room=-1;
    x=_x;
    y=_y;
}

void agent::move(){
    float dx,dy,r;
    dx=goalx-x;
    dy=goaly-y;
    r=sqrt(dx*dx+dy*dy);

    x+=dx/r;
    y+=dy/r;

}

void agent::observe(poi &p){
    if (abs(p.x-x)<1 && abs(p.y-y)<1)
        p.observe(idx);

}
