#include <iostream>
#include "./Constants.h"
#include "./Game.h"

Game::Game() {
	this->isRunning = false;
}

bool Game::IsRunning() const {
	return this->isRunning;
}

float projectilePosX = 0.0f;
float projectilePosY = 0.0f;

float projectileVelX = 20.0f;
float projectileVelY = 30.0f;

void Game::Initialize(int width, int height) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error initializing SDL!" << std::endl;
		return;
	}

	window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS);

	if (!window) std::cerr << "Error creating SDL window." << std::endl;

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) std::cerr << "Error creating SDL renderer." << std::endl;

	isRunning = true;
}

void Game::ProccessInput() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			       	isRunning = false;
			break;
		default:
			break;
	}
}

void Game::Update() {
	// Wait until frame target time has elapsed since the last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksSinceLastFrame + FRAME_TARGET_TIME));

	// Difference in ticks from last frame in seconds
	float deltaTime = (SDL_GetTicks() - ticksSinceLastFrame) / 1000.0f;

	// Upper bound on delta time of 0.05
	deltaTime = deltaTime > 0.05f ? 0.05f : deltaTime;

	ticksSinceLastFrame = SDL_GetTicks();

	projectilePosX += projectileVelX * deltaTime;
	projectilePosY += projectileVelY * deltaTime;
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	SDL_Rect projectile {
	       	(int) projectilePosX,
		(int) projectilePosY,
		10,
		10};
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &projectile);

	SDL_RenderPresent(renderer);
}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

