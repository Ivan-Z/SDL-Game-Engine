#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>
#include "./Component.h"
#include "./EntityManager.h"

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

		template <typename T, typename... TArgs>
		T& AddComponent(TArgs&&... args) {
			T* newComponent(new T(std::forward<TArgs>(args)...));
			newComponent->owner = this;
			components.emplace_back(newComponent);
			newComponent->Initialize();
			return *newComponent;
		}

	private:
		EntityManager& manager;
		bool isActive;
		std::vector<Component*> components;
};

#endif

