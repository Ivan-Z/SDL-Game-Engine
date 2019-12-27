#include <iostream>
#include "./EntityManager.h"
#include "./Collision.h"
#include "./components/ColliderComponent.h"
#include "./components/TransformComponent.h"

void EntityManager::ClearData() {
	for (auto& entity: entities) {
		entity->Destroy();
	}
	// TODO: Clear entity vector 
}	

bool EntityManager::HasNoEntities() {
	return entities.size() == 0;
}

void EntityManager::Update(float deltaTime) {
	for (auto& entity: entities){
		entity->Update(deltaTime);
	}
}

void EntityManager::Render() {
	// TODO optimize entity drawing to fetch them once instead of fetching 6 times
	for (int layer = 0; layer < NUM_LAYERS; layer++) {
		for (auto& entity: GetEntitiesByLayer(static_cast<LayerType>(layer))) {
			entity->Render();
		}
	}
}

Entity& EntityManager::AddEntity(std::string entityName, LayerType layer) {
	Entity* entity = new Entity(*this, entityName, layer);
	entities.emplace_back(entity);
	return *entity;
}

std::vector<Entity*> EntityManager::GetEntities() const {
	return entities;
}

std::vector<Entity*> EntityManager::GetEntitiesByLayer(LayerType layer) const {
	// TODO Fix naive implementation

	std::vector<Entity*> selectedEntities;
	for (auto& entity: entities) {
		if (entity->layer == layer) {
			selectedEntities.emplace_back(entity);
		}
	}
	return selectedEntities;
}

unsigned int EntityManager::GetEntityCount() const {
	return entities.size();
}

void EntityManager::PrintEntities() const {
	for (auto entity: entities) {
		std::cout << "Entity: " << entity->name << std::endl;
		entity->PrintComponents();
	}		
}

CollisionType EntityManager::CheckCollisions() const {
	//TODO: Refactor
	//TODO: O(n^2) every frame
	for (auto& thisEntity: entities) {
		if (!thisEntity->HasComponent<ColliderComponent>()) continue;
		ColliderComponent* thisCollider = thisEntity->GetComponent<ColliderComponent>();
		for (auto& thatEntity: entities) {
			if (thisEntity->name.compare(thatEntity->name) == 0 || !thatEntity->HasComponent<ColliderComponent>()) continue;
			ColliderComponent* thatCollider = thatEntity->GetComponent<ColliderComponent>();
			if (Collision::CheckRectangleCollision(thisCollider->collider, thatCollider->collider)) {
				if (thisCollider->colliderTag.compare("PLAYER") == 0 && thatCollider->colliderTag.compare("ENEMY") == 0) {
					return PLAYER_ENEMY_COLLISION;
				} 
				if (thisCollider->colliderTag.compare("PLAYER") == 0 && thatCollider->colliderTag.compare("PROJECTILE") == 0) {
					return PLAYER_PROJECTILE_COLLISION;
				}
				if (thisCollider->colliderTag.compare("ENEMY") == 0 && thatCollider->colliderTag.compare("FRIENDLY_PROJECTILE") == 0) {
					return ENEMY_PROJECTILE_COLLISION;
				}
				if (thisCollider->colliderTag.compare("PLAYER") == 0 && thatCollider->colliderTag.compare("LEVEL_COMPLETE") == 0) {
					return PLAYER_LEVEL_COMPLETE_COLLISION;
				}
			}

		}
	}
	return NO_COLLISION;
}
