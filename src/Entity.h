#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

class Component;
class EntityManager;

class Entity {
	public:
		std::string name;
		Entity(EntityManager& manager);
		Entity(EntityManager& manager, std::string name);
		void Update(float deltaTime);
		void Render();
		void Destroy();
		bool IsActive() const;
	private:
		EntityManager& manager;
		bool isActive;
		std::vector<Component*> components;
};

#endif

