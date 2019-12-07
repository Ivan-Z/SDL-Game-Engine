#include "./TransformComponent.h"

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
 }      

void TransformComponent::Render() {
}

