#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include "./Entity.h"
#include "./Component.h"
#include "./Constants.h"

class EntityManager  {
	public:
		void Update(float deltaTime);
		void Render();
		void ClearData();
		bool HasNoEntities();
		Entity& AddEntity(std::string entityName, LayerType layer);
	        std::vector<Entity*> GetEntities() const;
		std::vector<Entity*> GetEntitiesByLayer(LayerType layer) const;
		unsigned int GetEntityCount() const;	
		void PrintEntities() const;
		CollisionType CheckCollisions() const;
		void DestroyInactiveEntities();
		Entity* GetEntityByName(std::string name);
	private:
		std::vector<Entity*> entities;

};

#endif
