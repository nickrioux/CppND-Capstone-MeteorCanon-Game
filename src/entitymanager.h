#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <thread>
#include <mutex>
#include <set>
#include <vector>

#include "gameconstants.h"
#include "component.h"

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
        void SimulateCollisions();
        std::vector<EntityManager::CollisionData> GetCollisions();
        bool HasNoEntities();
        std::shared_ptr<Entity> AddEntity(const std::string & entityName, GameConstants::EntityType entityType, GameConstants::LayerType layerType);
        std::vector<std::shared_ptr<Entity>> GetEntitiesByLayer(GameConstants::LayerType layerType) const;
        unsigned int GetEntityCount();
        void ListAllEntities() const;

        std::unique_lock<std::mutex> GetLock() { return(std::unique_lock<std::mutex>(mtx_)); }
        std::unique_lock<std::mutex> GetLock(std::defer_lock_t t) { return(std::unique_lock<std::mutex>(mtx_,t)); }

    private:
        //Thread
        std::unique_ptr<std::thread> collisionThread_{nullptr};
        bool threadLaunched_{false};
        bool endCollisionsThread_{false};
        std::mutex mtx_;

        std::vector<std::shared_ptr<Entity>> entities_{}; 
        std::vector<EntityManager::CollisionData> collisions_{};

        //Private Functions
        void checkCollisions();
        void destroyInactiveEntities();
        bool validateCollision(const std::shared_ptr<ColliderComponent> &, const std::shared_ptr<ColliderComponent> &, const GameConstants::ColliderTag &, const GameConstants::ColliderTag  &) const;
};

#endif