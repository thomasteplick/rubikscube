"""
==============
3D scatterplot
==============

Demonstration of a basic scatterplot in 3D.
"""

import matplotlib.pyplot as plt
import argparse

# size of the container for the geometric object
dim = 48

# get file to plot the from command line
parser = argparse.ArgumentParser(prog='scatter3D', description="3D Scatterplot of Rubik's cube")
parser.add_argument('io', type=int, choices=[0,1])
parser.add_argument('view', type=int, choices=[0,1])
args = parser.parse_args()

# scrambled cube, solved cube
file = ["..\\data\\rubik_scrambled.txt", "..\\data\\rubik_solved.txt"]
state = ["scrambled", "solved"]

fig = plt.figure()
ax = fig.add_subplot(projection='3d')

'''
	std::vector<int> density2FGcolor = {
		FOREGROUND_YELLOW,
		FOREGROUND_GRAY,
		FOREGROUND_ORANGE,
		FOREGROUND_CYAN,
		FOREGROUND_GREEN,
		FOREGROUND_BLUE,
		FOREGROUND_RED,
		FOREGROUND_PURPLE,
		FOREGROUND_BROWN,
		FOREGROUND_WHITE,
	};
'b' as blue
'g' as green
'r' as red
'c' as cyan
'm' as magenta
'y' as yellow
'k' as black
'w' as white
'''

# Define colors
clrs = ['w', 'w', 'y', 'r', 'tab:orange', 'g', 'b', 'k']
mkr = '.'
# Read in cube file into geometric 3D np array
with open(file[args.io], mode='r', encoding="utf-8") as f:
    for i in range(dim):
        for j in range(dim):
            line = f.readline()
            item = line.split()
            for k, val in enumerate(item):
                ival = int(val)
                if (ival > 0):
                    #ax.scatter(np.array(i), np.array(j), np.array(k), marker=mkr, c=clrs[ival])
                    ax.scatter(i, j, k, marker=mkr, c=clrs[ival])

if (args.view == 0):
    ax.set_xlabel('Front face')
    ax.set_ylabel('Right face')
    ax.set_zlabel('Up face')
    plt.title("3D Scatterplot of Rubik's Cube, %s" % state[args.io])
else:
    ax.view_init(elev=-30, azim=135, roll=0)                       
    ax.set_xlabel('Back face')
    ax.set_ylabel('Left face')
    ax.set_zlabel('Down face')
    plt.title("3D Scatterplot of Rubik's Cube, %s" % state[args.io], pad=15)

plt.show()
