#include <iostream>
#include <memory>
#include <future>

#include "game.h"
#include "entity.h"
#include "collision.h"
#include "components/collidercomponent.h"
#include "entitymanager.h"

EntityManager::~EntityManager() { 
    //Scope the Mutex lock
    {
        std::unique_lock<std::mutex> lck = GetLock();
        endCollisionsThread_ = true;
    }
    collisionThread_->join();
}


void EntityManager::ClearData() {
    for (auto & entity : entities_) {
        entity->Destroy();
    }
}

void EntityManager::Update(float deltaTime) {
    std::unique_lock<std::mutex> lck = GetLock();
    
    for (auto & entity : entities_) {
        entity->Update(deltaTime);
    }

    destroyInactiveEntities();
}

void EntityManager::Render() {
    //Sort Entity by Layer and scope the lock on _entities
    {
        std::unique_lock<std::mutex> lck = GetLock();
        std::sort(entities_.begin(),entities_.end(),[](const std::shared_ptr<Entity> lhs, const std::shared_ptr<Entity> & rhs){return(lhs->GetLayer() < rhs->GetLayer());});
    }

    for (auto & entity : entities_) {
            entity->Render();
    }
}

void EntityManager::SimulateCollisions() {
    if (collisionThread_ == nullptr) {       
        collisionThread_ = std::make_unique<std::thread>(&EntityManager::checkCollisions, this);
    }
}

std::vector<EntityManager::CollisionData> EntityManager::GetCollisions() {
    std::unique_lock<std::mutex> lck = GetLock(std::defer_lock);
    
    std::vector<EntityManager::CollisionData> collisions;

    if (lck.try_lock()) {
        collisions = collisions_;
    }

    return(collisions);
}

void EntityManager::checkCollisions() {
    
    bool bEndCollisionThread = false;
    
    while (!bEndCollisionThread) {

        //Wait 1 micro-second
        std::this_thread::sleep_for(std::chrono::microseconds(5));
        
        std::unique_lock<std::mutex> lock = GetLock();

        collisions_.clear();

        for (int i = 0; i < entities_.size(); ++i) {
            auto& thisEntity = entities_[i];
            if (thisEntity->HasComponent<ColliderComponent>()) {
                std::shared_ptr<ColliderComponent> thisCollider = thisEntity->GetComponent<ColliderComponent>();
                for (int j = i-1; j < entities_.size(); ++j) {
                    auto& thatEntity = entities_[j];
                    if (thisEntity->GetName().compare(thatEntity->GetName()) != 0 && thatEntity->HasComponent<ColliderComponent>()) {
                        std::shared_ptr<ColliderComponent> thatCollider = thatEntity->GetComponent<ColliderComponent>();
                        if (Collision::CheckRectCollision(thisCollider->GetCollider(), thatCollider->GetCollider())) {
                            CollisionData collisionData;

                            collisionData.entityOne = thisEntity;
                            collisionData.entityTwo = thatEntity;

                            if (validateCollision(thisCollider,thatCollider,GameConstants::PlayerTag,GameConstants::MeteorTag)) {
                                collisionData.collisionType = GameConstants::CollisionType::PlayerMeteor;
                                collisions_.emplace_back(collisionData);
                            }

                            if (validateCollision(thisCollider,thatCollider,GameConstants::MeteorTag,GameConstants::BulletTag)) {
                                collisionData.collisionType = GameConstants::CollisionType::EnemyBullet;
                                collisions_.emplace_back(collisionData);
                            }

                            if (validateCollision(thisCollider,thatCollider,GameConstants::PlayerTag,GameConstants::LeftBoundaryTag)) {
                                collisionData.collisionType = GameConstants::CollisionType::PlayerLeftBoundary;
                                collisions_.emplace_back(collisionData);
                            }

                            if (validateCollision(thisCollider,thatCollider,GameConstants::PlayerTag,GameConstants::RightBoundaryTag)) {
                                collisionData.collisionType = GameConstants::CollisionType::PlayerRightBoundary;
                                collisions_.emplace_back(collisionData);
                            }

                            if (validateCollision(thisCollider,thatCollider,GameConstants::SpinnerTag,GameConstants::BulletTag)) {
                                collisionData.collisionType = GameConstants::CollisionType::EnemyBullet;
                                collisions_.emplace_back(collisionData);
                            }

                            if (validateCollision(thisCollider,thatCollider,GameConstants::PlayerTag,GameConstants::SpinnerTag)) {
                                collisionData.collisionType = GameConstants::CollisionType::PlayerSpinner;
                                collisions_.emplace_back(collisionData);
                            }

                            if (validateCollision(thisCollider,thatCollider,GameConstants::SpinnerTag,GameConstants::GroundTag)) {
                                collisionData.collisionType = GameConstants::CollisionType::SpinnerGround;
                                collisions_.emplace_back(collisionData);
                            }

                            if  (validateCollision(thisCollider,thatCollider,GameConstants::MeteorTag,GameConstants::GroundTag)) {

                                collisionData.collisionType = GameConstants::CollisionType::MeteorGround;
                                collisions_.emplace_back(collisionData);
                            }

                            if (validateCollision(thisCollider,thatCollider,GameConstants::MeteorTag,GameConstants::LeftBoundaryTag) ||
                                validateCollision(thisCollider,thatCollider,GameConstants::MeteorTag,GameConstants::RightBoundaryTag) ||
                                validateCollision(thisCollider,thatCollider,GameConstants::SpinnerTag,GameConstants::LeftBoundaryTag) ||
                                validateCollision(thisCollider,thatCollider,GameConstants::SpinnerTag,GameConstants::RightBoundaryTag)) {
                                collisionData.collisionType = GameConstants::CollisionType::FallingObjectBoundary;
                                collisions_.emplace_back(collisionData);
                            }
                        }
                    }
                }
            }
        }

        bEndCollisionThread = endCollisionsThread_;
    }
}


bool EntityManager::HasNoEntities() {
    return(entities_.size() == 0);
}

//Validate Collions between two Entities. 
bool EntityManager::validateCollision(const std::shared_ptr<ColliderComponent> & colliderOne, const std::shared_ptr<ColliderComponent> & colliderTwo, 
                                      const GameConstants::ColliderTag & collOneTag, const GameConstants::ColliderTag & collTwoTag) const {
    if ((colliderOne->GetColliderTag() == collOneTag) && (colliderTwo->GetColliderTag() == collTwoTag) ||
        (colliderOne->GetColliderTag() == collTwoTag) && (colliderTwo->GetColliderTag() == collOneTag)) {
            return true;
    }
    else {
        return false;
    } 
}

void EntityManager::destroyInactiveEntities() {
    for (auto it = entities_.begin(); it != entities_.end(); ++it) {
        if (!(*it)->IsActive()) {
            entities_.erase(it);
        }
    }
}

std::shared_ptr<Entity> EntityManager::AddEntity(std::string entityName, GameConstants::EntityType entityType, GameConstants::LayerType layerType) {
    std::unique_lock<std::mutex> lock = GetLock();
    std::shared_ptr<Entity> entity = std::make_shared<Entity>(*this, entityName, entityType, layerType);
    entities_.emplace_back(entity);
    return entity;
}

std::vector<std::shared_ptr<Entity>> EntityManager::GetEntitiesByLayer(GameConstants::LayerType layerType) const {
    std::vector<std::shared_ptr<Entity>> entitiesLayer;

    for(auto & entity : entities_) {
            if (entity->GetLayer() == layerType) {
                    entitiesLayer.emplace_back(entity);
            }
    }

    return(entitiesLayer);
}

unsigned int EntityManager::GetEntityCount() {
    return(entities_.size());
}

void EntityManager::ListAllEntities() const {
    unsigned int i = 0;

    for (auto & entity : entities_) {
        std::cout << "Entity[" << i << "]: " << entity->GetName() << std::endl;
        entity->ListAllComponents();
        i++;
    }
}

