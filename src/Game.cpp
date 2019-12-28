#include <iostream>
#include "../lib/glm/glm.hpp"
#include "./Constants.h"
#include "./Game.h"
#include "./Map.h"
#include "./components/TransformComponent.cpp"
#include "./components/SpriteComponent.h"
#include "./components/KeyboardControlComponent.h"
#include "./components/ColliderComponent.h"
#include "./components/LabelComponent.h"
#include "./components/ProjectileEmitterComponent.h"

// TODO; Add to header file
// TODO Throw error if asset image not found or if asset ID not in asset manager
EntityManager manager;
SDL_Renderer* Game::renderer;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Event Game::event;
SDL_Rect Game::camera {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map* Game::map;
bool Game::showColliders = false;
Entity* player = NULL;
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
	
	if (TTF_Init() != 0) {
		std::cerr << "Error initializing SDL TTF!" << std::endl;
		return;
	}
	
	window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS);

	if (!window) std::cerr << "Error creating SDL window." << std::endl;

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) std::cerr << "Error creating SDL renderer." << std::endl;

	LoadLevel(1);
	manager.PrintEntities();
	isRunning = true;
}

void Game::LoadLevel(int levelNumber) {
	sol::state lua;
	lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

	std::string levelName = "Level" + std::to_string(levelNumber);
	lua.script_file("./assets/scripts/" + levelName + ".lua");

	// Loads assets from Lua config file
	sol::table levelData = lua[levelName];
	sol::table levelAssets = levelData["assets"];

	unsigned int assetIndex = 0;
	while(true) {
		sol::optional<sol::table> existsAssetIndexMode = levelAssets[assetIndex];
		if (existsAssetIndexMode == sol::nullopt) {
			break;
		}
		sol::table asset = levelAssets[assetIndex];
		std::string assetType = asset["type"];
		if (assetType.compare("texture") == 0) {
			std::string assetId = asset["id"];
			std::string assetFile = asset["file"];
			assetManager->AddTexture(assetId, assetFile.c_str());
		}
		assetIndex++;
	}
	
	// Loads map from Lua config file
	sol::table levelMap = levelData["map"];
	std::string mapTextureId = levelMap["textureAssetId"];
	std::string mapFile = levelMap["file"];

	map = new Map(mapTextureId, static_cast<int>(levelMap["scale"]), static_cast<int>(levelMap["tileSize"]));

	map->LoadMap(mapFile, static_cast<int>(levelMap["mapSizeX"]), static_cast<int>(levelMap["mapSizeY"]));
}

void Game::ProcessInput() {
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			       	isRunning = false;
			else if (event.key.keysym.sym == SDLK_0)
				showColliders = !showColliders;
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

	HandleCameraMovement();
	CheckCollisions();
}

void Game::HandleCameraMovement() {
	if (player) {
		TransformComponent* mainPlayerTransform = player->GetComponent<TransformComponent>();
		
		camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
		camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

		camera.x = camera.x < 0 ? 0 : camera.x;
		camera.x = camera.x > (Game::map->GetWidth() / 2) ? (Game::map->GetWidth() / 2) : camera.x;
		camera.y = camera.y < 0 ? 0 : camera.y;
		//TODO: Hacky fix for camera bug
		camera.y = camera.y > (Game::map->GetHeight() / 2) + mainPlayerTransform->height ? (Game::map->GetHeight() / 2) + mainPlayerTransform->height : camera.y;
	}
}

void Game::CheckCollisions() {
	CollisionType collisionType = manager.CheckCollisions();
	if (collisionType == PLAYER_ENEMY_COLLISION) {
		ProccessGameOver();
	}
       	if (collisionType == PLAYER_PROJECTILE_COLLISION) {
		ProccessGameOver();

	}	
	if (collisionType == PLAYER_LEVEL_COMPLETE_COLLISION) {
		ProccessNextLevel(1);
	}
}

void Game::ProccessNextLevel(int levelNumber) {
	std::cout << "Next Level" << std::endl;
	isRunning = false;
}

void Game::ProccessGameOver() {
	std::cout << "Game Over" << std::endl;
	isRunning = false;
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

