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

// TODO; Add to header file
// TODO Throw error if asset image not found or if asset ID not in asset manager
EntityManager manager;
SDL_Renderer* Game::renderer;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Event Game::event;
SDL_Rect Game::camera {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map* Game::map;
bool Game::showColliders = false;

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

	LoadLevel(0);
	manager.PrintEntities();
	isRunning = true;
}

Entity& player(manager.AddEntity("chopper", PLAYER));
void Game::LoadLevel(int levelNumber) {
	// Add assets to the asset manager
	assetManager->AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
	assetManager->AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
	assetManager->AddTexture("radar-image", std::string("./assets/images/radar.png").c_str());
	assetManager->AddTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());
	assetManager->AddTexture("collider-box", std::string("./assets/images/collision-texture.png").c_str());
	assetManager->AddTexture("heliport-image", std::string("./assets/images/heliport.png").c_str());
	assetManager->AddFont("charriot-font", std::string("./assets/fonts/charriot.ttf").c_str(), 14);
		
	map = new Map("jungle-tiletexture", 2, 32);
	map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);

	// Add entities and their components to entity manager
	Entity& tankEntity(manager.AddEntity("tank", ENEMY));
	tankEntity.AddComponent<TransformComponent>(150, 495, 15, 0, 32, 32, 1);
	tankEntity.AddComponent<SpriteComponent>("tank-image");
	tankEntity.AddComponent<ColliderComponent>("ENEMY", 150, 495, 32, 32);

	player.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
	player.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
	player.AddComponent<KeyboardControlComponent>("up", "down", "left", "right", "space");
	player.AddComponent<ColliderComponent>("PLAYER", 240, 106, 32, 32);

	Entity& radarEntity(manager.AddEntity("radar", UI));
	radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
	radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);

	Entity& heliport(manager.AddEntity("heliport", OBSTACLE));
	heliport.AddComponent<TransformComponent>(470, 420, 0, 0, 32, 32, 1);
	heliport.AddComponent<SpriteComponent>("heliport-image");
	heliport.AddComponent<ColliderComponent>("LEVEL_COMPLETE", 470, 420, 32, 32);
	
	Entity& labelLevelName(manager.AddEntity("LabelLevelName", UI));
	labelLevelName.AddComponent<LabelComponent>(10, 10, "First level...", "charriot-font", WHITE_COLOR);

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
	TransformComponent* mainPlayerTransform = player.GetComponent<TransformComponent>();
	
	camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
	camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

	camera.x = camera.x < 0 ? 0 : camera.x;
	camera.x = camera.x > (Game::map->GetWidth() / 2) ? (Game::map->GetWidth() / 2) : camera.x;
	camera.y = camera.y < 0 ? 0 : camera.y;
	//TODO: Hacky fix for camera bug
	camera.y = camera.y > (Game::map->GetHeight() / 2) + mainPlayerTransform->height ? (Game::map->GetHeight() / 2) + mainPlayerTransform->height : camera.y;
	
}

void Game::CheckCollisions() {
	CollisionType collisionType = manager.CheckCollisions();
	if (collisionType == PLAYER_ENEMY_COLLISION) {
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

