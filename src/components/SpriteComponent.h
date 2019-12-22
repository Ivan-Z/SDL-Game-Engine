#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "../Animation.h"
#include "./TransformComponent.h"

class SpriteComponent: public Component {

	// TODO: Move to cpp file
	// TODO: Possible bug with animationIndex not being updated
	public:
		SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

		SpriteComponent(std::string assetTextureId) {
			SetTexture(assetTextureId);
			isAnimated = false;
			isFixed = false;	
			name = "Sprite";
		}

		SpriteComponent(std::string assetTextureId, int numFrames, int animationSpeed, bool hasDirection, bool isFixed): isAnimated(true), numFrames(numFrames), animationSpeed(animationSpeed), isFixed(isFixed) {
			isAnimated = true;

			if (hasDirection) {
				Animation downAnimation = Animation(0, numFrames, animationSpeed);
				Animation rightAnimation = Animation(1, numFrames, animationSpeed);
				Animation leftAnimation = Animation(2, numFrames, animationSpeed);
				Animation upAnimation = Animation(3, numFrames, animationSpeed);

				animations.emplace("DownAnimation", downAnimation);
				animations.emplace("RightAnimation", rightAnimation);
				animations.emplace("LeftAnimation", leftAnimation);
				animations.emplace("UpAnimation", upAnimation);
				
				animationIndex = 0;
				currentAnimationName = "DownAnimation";
			} else {
				Animation singleAnimation = Animation(0, numFrames, animationSpeed);
				animations.emplace("SingleAnimation", singleAnimation);
				animationIndex = 0;
				currentAnimationName = "SingleAnimation";
			}
			Play(currentAnimationName);
			SetTexture(assetTextureId);

			name = "Sprite";	
		}

		void Play(std::string animationName) {
			numFrames = animations[animationName].numFrames;
			animationIndex = animations[animationName].index;
			animationSpeed = animations[animationName].animationSpeed;
			currentAnimationName = animationName;	
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
			if (isAnimated) {
				sourceRectangle.x = sourceRectangle.w * static_cast<int>((SDL_GetTicks() / animationSpeed) % numFrames);
			}
			sourceRectangle.y = animationIndex * entityTransform->height;

			destinationRectangle.x = static_cast<int>(entityTransform->position.x) - Game::camera.x ;
			destinationRectangle.y = static_cast<int>(entityTransform->position.y) - Game::camera.y;
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
		bool isAnimated;
		int numFrames;
		int animationSpeed;
		bool isFixed;
		std::map<std::string, Animation> animations;
		std::string currentAnimationName;
		unsigned int animationIndex = 0;
};

#endif
