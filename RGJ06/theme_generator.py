#!/usr/bin/python2
#
# Awesome random theme chooser for Reddit Game Jam 06
#

import random 

themes = ["food/eating", "insects", "giant robots", "symmetry",
		"growth", "hats", "colors", "masks", 
		"afterlife", "energy", "hunger", "flight", "seasons",
		"storm", "beauty/ugliness", "evolution", "time", "apocalypse",
        "particles", "rotation", "up/down", "cowardice", "limitation",
        "steampunk", "nemesis", "connectivity", "office", "dawn of man",
        "elements", "twisted", "anger", "blind"]

print random.choice(themes)
