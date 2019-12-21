#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include "../Game.h"
#include "../EntityManager.h"
#include "../Component.h"
#include "./TransformComponent.h"
#include "./SpriteComponent.h"

// TODO: Move to cpp file
class KeyboardControlComponent: public Component {
	public:
		std::string upKey;	
		std::string downKey;
		std::string leftKey;
		std::string rightKey;
		std::string shootKey;
		TransformComponent* entityTransform;
		SpriteComponent* entitySprite;

		KeyboardControlComponent() {
			
		}

		KeyboardControlComponent(std::string upKey, std::string downKey, std::string leftKey, std::string rightKey, std::string shootKey): upKey(GetSDLKeyStringCode(upKey)), downKey(GetSDLKeyStringCode(downKey)),leftKey(GetSDLKeyStringCode(leftKey)), rightKey(GetSDLKeyStringCode(rightKey)), shootKey(GetSDLKeyStringCode(shootKey)) {
			
		}

		std::string GetSDLKeyStringCode(std::string key) {
			if (key.compare("up") == 0) return "1073741906";
			if (key.compare("down") == 0) return "1073741905";
			if (key.compare("left") == 0) return "1073741904";
			if (key.compare("right") == 0) return "1073741903";
			if (key.compare("down") == 0) return "32";
		        return std::to_string(static_cast<int>(key[0]));	
		}

		void Initialize() override {
			entityTransform = owner->GetComponent<TransformComponent>();
			entitySprite = owner->GetComponent<SpriteComponent>();
		}

		void Update(float deltaTime) override {
			// TODO: Refactor
			if (Game::event.type == SDL_KEYDOWN) {
				std::string keyCode = std::to_string(Game::event.key.keysym.sym);

				if (keyCode.compare(upKey) == 0) {
					entityTransform->velocity.y = -160;
					entityTransform->velocity.x = 0;
					entitySprite->Play("UpAnimation");
				}
				if (keyCode.compare(downKey) == 0) {
					entityTransform->velocity.y = 160;
					entityTransform->velocity.x = 0;
					entitySprite->Play("DownAnimation");
				} 
				if (keyCode.compare(rightKey) == 0) {
					entityTransform->velocity.y = 0;
					entityTransform->velocity.x = 160;
					entitySprite->Play("RightAnimation");
				} 
				if (keyCode.compare(leftKey) == 0) {
					entityTransform->velocity.y = 0;
					entityTransform->velocity.x = -160;
					entitySprite->Play("LeftAnimation");
				}
				if (keyCode.compare(shootKey) == 0) {
				}
			}
			if (Game::event.type == SDL_KEYUP) {
				std::string keyCode = std::to_string(Game::event.key.keysym.sym);

				if (keyCode.compare(upKey) == 0) {
					entityTransform->velocity.y = 0;

				}
				if (keyCode.compare(downKey) == 0) {
					entityTransform->velocity.y = 0;

				} 
				if (keyCode.compare(rightKey) == 0) {
					entityTransform->velocity.x = 0;

				} 
				if (keyCode.compare(leftKey) == 0) {
					entityTransform->velocity.x = 0;

				}
				if (keyCode.compare(shootKey) == 0) {

				}

			}
		}

	
};

#endif
