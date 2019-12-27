#ifndef PROJECTILEEMITTERCOMPONENT_H
#define PROJECTILEEMITTERCOMPONENT_H

#include "../../lib/glm/glm.hpp"
#include "../EntityManager.h"
#include "./TransformComponent.h"

class ProjectileEmitterComponent : public Component {
	public:
		ProjectileEmitterComponent(int speed, int angleDegrees, int range, bool shouldLoop): speed(speed),range(range), shouldLoop(shouldLoop) {
			angleRadians = glm::radians(static_cast<float>(angleDegrees));	
		}

		void Initialize() override {
			entityTransform = owner->GetComponent<TransformComponent>();
			origin = glm::vec2(entityTransform->position.x, entityTransform->position.y);
			entityTransform->velocity = glm::vec2(glm::cos(angleRadians) * speed, glm::sin(angleRadians) * speed);
		}

		void Update(float deltaTime) override {
			if (glm::distance(entityTransform->position, origin) > range) {
				if (shouldLoop) {
					entityTransform->position.x = origin.x;
					entityTransform->position.y = origin.y;
					// TODO: Fix new spawn code
				} else {
					owner->Destroy();
				}
			}
		}

	private:
		TransformComponent* entityTransform;
		glm::vec2 origin;
		int speed;
		int range;
		float angleRadians;
		bool shouldLoop;
};

#endif
