#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <SDL2/SDL.h>
#include "../Game.h"
#include "../EntityManager.h"
#include "./TransformComponent.h"

class ColliderComponent: public Component {
	public:
		std::string colliderTag;
		SDL_Rect collider;
		SDL_Rect sourceRectangle;
		SDL_Rect destinationRectangle;
		TransformComponent* entityTransform;

		ColliderComponent(std::string colliderTag, int x, int y, int width, int height): colliderTag(colliderTag), collider({x, y, width, height}) {
			name = "Collider";
		}

		void Initialize() override {
			if (owner->HasComponent<TransformComponent>()) {
				entityTransform = owner->GetComponent<TransformComponent>();
				sourceRectangle = {0, 0, entityTransform->width, entityTransform->height};
				destinationRectangle = {collider.x, collider.y, collider.w, collider.h};
			}
		}	

		void Update(float deltaTime) override {
			collider.x = static_cast<int>(entityTransform->position.x);
			collider.y = static_cast<int>(entityTransform->position.y);
			collider.w = entityTransform->width * entityTransform->scale;
			collider.h = entityTransform->height * entityTransform->scale;
			destinationRectangle.x = collider.x - Game::camera.x;
			destinationRectangle.y = collider.y - Game::camera.y;
		}
};


#endif
