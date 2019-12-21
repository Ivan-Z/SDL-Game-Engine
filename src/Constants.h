#ifndef CONSTANTS_H
#define CONSTANTS_H

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

const unsigned int FPS = 60;
// Millisecons per frame
const unsigned int FRAME_TARGET_TIME = 1000 / FPS;

enum LayerType { 
	TILEMAP = 0,
	VEGETATION = 1,
	ENEMY = 2,
	PLAYER = 3,
	PROJECTYYLE = 4,
	UI = 5
};

const unsigned int NUM_LAYERS = 6;

#endif

