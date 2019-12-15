#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "./TransformComponent.h"

class SpriteComponent: public Component {

	// TODO: Move to cpp file
	public:
		SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

		SpriteComponent(const char* filePath) {
			SetTexture(filePath);
			
			name = "Sprite";
		}

		void SetTexture(std::string assetTextureId){
			texture = Game::assetManager->GetTexture(assetTextureId);
		}

		void Initialize() override {
			entityTransform = owner->GetComponent<TransformComponent>();
			sourceRectangle.x = 0;
			sourceRectangle.y = 0;
			sourceRectangle.w = entityTransform->width;
			sourceRectangle.h = entityTransform->height;
		}

		void Update(float deltaTime) override {
			destinationRectangle.x = (int) entityTransform->position.x;
			destinationRectangle.y = (int) entityTransform->position.y;
			destinationRectangle.w = entityTransform->width * entityTransform->scale;
			destinationRectangle.h = entityTransform->height * entityTransform->scale;
		}


		void Render() override {
			TextureManager::Draw(texture, sourceRectangle, destinationRectangle, spriteFlip);
		}
	private:
		SDL_Texture* texture;
		SDL_Rect sourceRectangle;
		SDL_Rect destinationRectangle;
		TransformComponent* entityTransform;
};

#endif
