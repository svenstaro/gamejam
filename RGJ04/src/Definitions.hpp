#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#define WIDTH	800
#define HEIGHT	600

#define PIXELS_PER_METER	64.0f
#define METERS_PER_PIXEL	(1.0f/PIXELS_PER_METER)

#define COL_NOTHING 0
#define COL_PLAYER 1<<1
#define COL_ENEMY 1<<2
#define COL_WALL 1<<3

#define PlayerCollidesWith COL_WALL
#define WallCollidesWith COL_PLAYER
#define EnemyCollidesWith COL_PLAYER

#endif
