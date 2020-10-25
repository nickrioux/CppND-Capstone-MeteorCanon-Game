#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>

#include "gameconstants.h"
#include "entity.h"
#include "component.h"

using std::vector;

class ColliderComponent;
class EntityManager {
       
    public:
        
        struct CollisionData {
            GameConstants::CollisionType collisionType;
            std::shared_ptr<Entity> entityOne;
            std::shared_ptr<Entity> entityTwo;
        };

        void ClearData();
        void Update(float deltaTime);
        void Render();
        vector<EntityManager::CollisionData> CheckCollisions() const;
        bool HasNoEntities();
        std::shared_ptr<Entity> AddEntity(std::string entityName, GameConstants::EntityType entityType, GameConstants::LayerType layerType);
        vector<std::shared_ptr<Entity>> GetEntities() const;
        vector<std::shared_ptr<Entity>> GetEntitiesByLayer(GameConstants::LayerType layerType) const;
        unsigned int GetEntityCount();
        void ListAllEntities() const;

    private:
        vector<std::shared_ptr<Entity>> _entities; 

        void destroyInactiveEntities();
        bool validateCollision(const std::shared_ptr<ColliderComponent> &, const std::shared_ptr<ColliderComponent> &, const string &, const string &) const;
};

#endif