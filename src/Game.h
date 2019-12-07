#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "./Entity.h"
#include "./Component.h"
#include "./EntityManager.h"

class Game {
	public:
		Game();
		~Game();
		void Initialize(int width, int height);
		void ProccessInput();
		void Update();
		void Render();
		void Destroy();
		void LoadLevel(int levelNumber);
		bool IsRunning() const;
			
		static SDL_Renderer* renderer;
	private:
		bool isRunning;
		int ticksSinceLastFrame = 0;
		SDL_Window *window;
};

#endif
