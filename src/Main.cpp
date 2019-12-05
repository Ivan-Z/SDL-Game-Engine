#include "./Constants.h"
#include "./Game.h"

int main(int argc, char* argv[]) {
	Game *game = new Game();

	game->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

	while(game->IsRunning()) {
		game->ProccessInput();
		game->Update();
		game->Render();
	}	

	game->Destroy();
}

