#include "./TransformComponent.h"
#include "../Constants.h"
#include "../Game.h"
#include <iostream>
TransformComponent::TransformComponent(int posX, int posY, int velX, int velY, int w, int h, int s) {
	position = glm::vec2(posX, posY);
	velocity = glm::vec2(velX, velY);
	width = w;
	height = h;
	scale = s;

	name = "Transform";
}
 	
 void TransformComponent::Update(float deltaTime) {

 	position.x += velocity.x * deltaTime;
	position.y += velocity.y * deltaTime;

	// Check Bounds of screen
	// TODO:Refactor

	if (position.x > Game::map->GetWidth() - width) {
		position.x = Game::map->GetWidth() - width;
	} else if (position.x < 0) {
		position.x = 0;
	}

	if (position.y > Game::map->GetHeight() - height) {
		position.y = Game::map->GetHeight() - height;
	} else if (position.y < 0) {
		position.y = 0;
	}
 }      

void TransformComponent::Render() {
}

