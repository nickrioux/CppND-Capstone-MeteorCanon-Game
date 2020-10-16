#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "gameconstants.h"
#include "entity.h"
#include "component.h"
#include <vector>

using std::vector;

class EntityManager {
       
    public:
        
        struct CollisionData {
            GameConstants::CollisionType collisionType;
            Entity * entityOne;
            Entity * entityTwo;
        };

        void ClearData();
        void Update(float deltaTime);
        void Render();
        vector<EntityManager::CollisionData> CheckCollisions() const;
        bool HasNoEntities();
        Entity& AddEntity(std::string entityName, GameConstants::EntityType entityType, GameConstants::LayerType layerType);
        std::vector<Entity*> GetEntities() const;
        std::vector<Entity*> GetEntitiesByLayer(GameConstants::LayerType layerType) const;
        unsigned int GetEntityCount();
        void ListAllEntities() const;

    private:
        vector<Entity*> _entities; 

        void destroyInactiveEntities();

};

#endif