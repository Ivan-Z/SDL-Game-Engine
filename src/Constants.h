#ifndef CONSTANTS_H
#define CONSTANTS_H

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

const unsigned int FPS = 60;
// Millisecons per frame
const unsigned int FRAME_TARGET_TIME = 1000 / FPS;

enum CollisionType {
	NO_COLLISION,
	PLAYER_ENEMY_COLLISION,
	PLAYER_PROJECTILE_COLLISION,
	ENEMY_PROJECTILE_COLLISION,
	PLAYER_VEGITATION_COLLISION,
	PLAYER_LEVEL_COMPLETE_COLLISION
};

enum LayerType { 
	TILEMAP = 0,
	VEGETATION = 1,
	ENEMY = 2,
	PLAYER = 3,
	OBSTACLE = 4,
	PROJECTILE = 5,
	UI = 6
};

const unsigned int NUM_LAYERS = 7;

#endif

