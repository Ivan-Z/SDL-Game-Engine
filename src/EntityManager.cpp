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

std::string EntityManager::CheckEntityCollisions(Entity& myEntity) const {
	ColliderComponent* myCollider = myEntity.GetComponent<ColliderComponent>();
	for (auto& entity: entities) {
		if (entity->name.compare(myEntity.name) == 0 || entity->name.compare("Tile") == 0) continue;
		if (entity->HasComponent<ColliderComponent>()) {
			ColliderComponent* otherCollider = entity->GetComponent<ColliderComponent>();
			if (Collision::CheckRectangleCollision(myCollider->collider, otherCollider->collider)) {
				return otherCollider->colliderTag;	
			}
		}
	}
	return std::string();
}
