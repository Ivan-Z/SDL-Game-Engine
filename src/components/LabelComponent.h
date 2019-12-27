#ifndef LABELCOMPONENT_H
#define LABELCOMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../FontManager.h"
#include "../EntityManager.h"
#include "../AssetManager.h"
#include "../Game.h"

class LabelComponent: public Component {
	public:
		LabelComponent(int x, int y, std::string text, std::string fontFamily, const SDL_Color& color): position({x, y}), text(text), fontFamily(fontFamily), color(color) {
			SetLabelText(text, fontFamily);
		}

		void SetLabelText(std::string text, std::string fontFamily) {
			SDL_Surface* surface = TTF_RenderText_Blended(Game::assetManager->GetFont(fontFamily), text.c_str(), color);
			texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
			SDL_FreeSurface(surface);
			SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);

		}

		void Render() override {
			FontManager::Draw(texture, position);
		}
	private:
		SDL_Rect position;
		std::string text;
		std::string fontFamily;
		SDL_Color color;
		SDL_Texture* texture;

};

#endif
