#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <thread>
#include <mutex>
#include <vector>

#include "gameconstants.h"
#include "component.h"

using std::vector;
using std::string;

class Entity;
class ColliderComponent;
class EntityManager {
       
    public:
        
        struct CollisionData {
            GameConstants::CollisionType collisionType;
            std::shared_ptr<Entity> entityOne;
            std::shared_ptr<Entity> entityTwo;
        };
        ~EntityManager();
        void ClearData();
        void Update(float deltaTime);
        void Render();
        void CheckCollisions();
        bool CollisionsComputed();
        vector<EntityManager::CollisionData> GetCollisions();
        bool HasNoEntities();
        std::shared_ptr<Entity> AddEntity(string entityName, GameConstants::EntityType entityType, GameConstants::LayerType layerType);
        vector<std::shared_ptr<Entity>> GetEntities() const;
        vector<std::shared_ptr<Entity>> GetEntitiesByLayer(GameConstants::LayerType layerType) const;
        unsigned int GetEntityCount();
        void ListAllEntities() const;

        std::unique_lock<std::mutex> GetLock() { return(std::unique_lock<std::mutex>(_mtx)); }
        std::unique_lock<std::mutex> GetLock(std::defer_lock_t t) { return(std::unique_lock<std::mutex>(_mtx,t)); }

    private:
        //Thread
        std::unique_ptr<std::thread> _collisionThread;
        bool _threadLaunched{false};
        bool _endCollisionsThread{false};
        std::mutex _mtx; 

        vector<std::shared_ptr<Entity>> _entities; 
        vector<EntityManager::CollisionData> _collisions{};
        void checkCollisions();
        void checkCollisionsNoThread();
        void destroyInactiveEntities();
        bool validateCollision(const std::shared_ptr<ColliderComponent> &, const std::shared_ptr<ColliderComponent> &, const GameConstants::ColliderTag &, const GameConstants::ColliderTag  &) const;
};

#endif