import cppyy
cppyy.include("aic.h")
from cppyy.gbl import aic,params

import numpy as np
import matplotlib.pyplot as plt


def vis(env):





p=params()
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
env=aic(p) 
print(np.array([i for i in env.state()]))
env.action([1,1,1,1,1])
print(env.agents[0].x)