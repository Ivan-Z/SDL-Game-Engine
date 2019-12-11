#include <iostream>
#include "../lib/glm/glm.hpp"
#include "./Constants.h"
#include "./Game.h"
#include "./components/TransformComponent.cpp"
#include "./components/SpriteComponent.h"


// TODO; Add to header file
EntityManager manager;
SDL_Renderer* Game::renderer;
AssetManager* Game::assetManager = new AssetManager(&manager);

Game::Game() {
	this->isRunning = false;
}

Game::~Game() {}

bool Game::IsRunning() const {
	return this->isRunning;
}

void Game::Initialize(int width, int height) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error initializing SDL!" << std::endl;
		return;
	}

	window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS);

	if (!window) std::cerr << "Error creating SDL window." << std::endl;

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) std::cerr << "Error creating SDL renderer." << std::endl;

	LoadLevel(0);
	manager.PrintEntities();
	isRunning = true;
}

void Game::LoadLevel(int levelNumber) {
	// Add assets to the asset manager
	std::string textureFilePath = "./assets/images/tank-big-right.png";
	assetManager->AddTexture("tank-image", textureFilePath.c_str());

	// Add entities and their components to entity manager
	Entity& newEntity(manager.AddEntity("tank"));
	newEntity.AddComponent<TransformComponent>(0, 0, 20, 20, 32, 32, 1);
	newEntity.AddComponent<SpriteComponent>("tank-image");

	Entity& newEntity1(manager.AddEntity("projectile1"));
	newEntity1.AddComponent<TransformComponent>(400, 400, -20, -20, 64, 56, 1);

	Entity& newEntity2(manager.AddEntity("projectile2"));
	newEntity2.AddComponent<TransformComponent>(0, 0, 25, 50, 32, 32, 1);

}

void Game::ProcessInput() {
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

	manager.Update(deltaTime);
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	if (manager.HasNoEntities()) return;
	manager.Render();

	SDL_RenderPresent(renderer);
}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

