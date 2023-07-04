from numpy import *
import numpy as np
import time
import sys


def get_trajectory(filename): 
    data =[]
    with open(filename) as input_file:
        try:
            for line in input_file:
                line = line.strip()
                for number in line.split():
                    data.append(float(number))
        except IOError:
            print ("Could not read file:", filename)
    data = np.reshape(data,(1000,3))
    return data
num = 0
filename = "trajectory" +str(num) + ".bin"
data  = get_trajectory(filename)


from mayavi import mlab
from mayavi.mlab import *
mlab.figure(1, bgcolor=(1, 1, 1))
timestep = 0
while True:    
        filename = "trajectory" +str(timestep) + ".bin"
        # Increment timestep
        timestep = timestep + 100
        data = get_trajectory(filename)
        items=list(zip(*data))
        points3d(items[0], items[1], items[2], color = (1,0,0), colormap = "bone", scale_factor = 0.9)
        mlab.view(.0, - 5.0, 4)
        mlab.show()

