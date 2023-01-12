import cppyy
cppyy.include("aic.h")
from cppyy.gbl import aic,params

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle

def view(env):
    plt.ion()
    plt.clf()


    ax = plt.gca()
    for room in env.rooms:
        
        ax.add_patch(Rectangle([room.x1,room.y1], room.x2-room.x1 ,room.y2-room.y1,
                                edgecolor='black',
                                facecolor='none',
                                zorder=-1,
                                lw=1))
   
        door = room.doorx,room.doory
        if door is not None:
            plt.scatter(door[0], door[1], s=200, c="white", marker="s")

        poi = np.array([[poi.x, poi.y, poi.type,poi.active] for poi in room.pois]).T
        color = ["r", "b", "y", "c"]
        colors = [color[int(p)] for p in poi[2]]
        alphas = [float(p)*0.5+0.5 for p in poi[3]]
        plt.scatter(poi[0], poi[1], c=colors, marker="s",alpha=alphas)

    agent = np.array([[agent.x, agent.y, agent.type] for agent in env.agents]).T
    color = ["b", "y", "c"]
    colors = [color[int(p)] for p in agent[2]]
    plt.scatter(agent[0], agent[1], c=colors, marker="o")
    ax.axis('equal')
    plt.pause(.2)




p=params()
p.seed=0
p.room_decay=10
p.n_agents=5
p.room_height=10
p.room_width=4
p.n_rooms=6
p.pois_per_room=2
p.poi_types=2
p.hallway_width=2
p.n_values=3
p.coupling=1
env=aic(p) 
for j in range(p.n_rooms):
    for i in range(20):
        view(env)
        #print(np.array([i for i in env.state()]))
        env.action([j*2+1]*p.n_agents)
