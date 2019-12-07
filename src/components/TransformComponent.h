#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <SDL2/SDL.h>
#include "../EntityManager.h"
#include "../../lib/glm/glm.hpp"
#include "../Game.h"

class TransformComponent: public Component {
	public:
		TransformComponent(int posX, int posY, int velX, int velY, int w, int h, int s);
		void Initialize() override {};
		void Update(float deltaTime) override;
		void Render() override;
	private:
		glm::vec2 position;
                glm::vec2 velocity;
                int width;
                int height;
		int scale;
};

#endif
