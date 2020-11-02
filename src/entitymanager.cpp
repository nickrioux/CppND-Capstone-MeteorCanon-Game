#include <iostream>
#include <memory>
#include <future>

#include "game.h"
#include "entity.h"
#include "collision.h"
#include "components/collidercomponent.h"
#include "entitymanager.h"

EntityManager::~EntityManager() { 
    {
        std::unique_lock<std::mutex> lck = GetLock();
        _endCollisionsThread = true;

    }
    _collisionThread->join();
}


void EntityManager::ClearData() {
    for (auto & entity : _entities) {
            entity->Destroy();
    }
}

void EntityManager::Update(float deltaTime) {
    std::unique_lock<std::mutex> lck = GetLock();
    
    for (auto & entity : _entities) {
            entity->Update(deltaTime);
    }

    destroyInactiveEntities();
}

void EntityManager::Render() {
    //TODO - Final Solution (Sort Entity by Layer when adding a new Entity)
    for (int layerNumber = 0; layerNumber < GameConstants::kNumLayers; ++layerNumber) {
        for (auto & entity : GetEntitiesByLayer(static_cast<GameConstants::LayerType>(layerNumber))) {
                entity->Render();
        }
    }
}

void EntityManager::CheckCollisions() {
    if (!_threadLaunched) {       
        _collisionThread = std::make_unique<std::thread>(&EntityManager::checkCollisions, this);
        _threadLaunched = true;
    }
}

vector<EntityManager::CollisionData> EntityManager::GetCollisions() {
    std::unique_lock<std::mutex> lck = GetLock(std::defer_lock);
    
    vector<EntityManager::CollisionData> collisions;

    if (lck.try_lock()) {
        collisions = _collisions;
    }

    return(collisions);
}

void EntityManager::checkCollisions() {
    
    bool bEndCollisionThread = false;
    
    while (!bEndCollisionThread) {

        //Wait 1 micro-second
        std::this_thread::sleep_for(std::chrono::microseconds(5));
        
        std::unique_lock<std::mutex> lock = GetLock();

        _collisions.clear();

        for (int i = 0; i < _entities.size() - 1; i++) {
            auto& thisEntity = _entities[i];
            if (thisEntity->HasComponent<ColliderComponent>()) {
                std::shared_ptr<ColliderComponent> thisCollider = thisEntity->GetComponent<ColliderComponent>();
                for (int j = i+1; j < _entities.size(); j++) {
                    auto& thatEntity = _entities[j];
                    if (thisEntity->GetName().compare(thatEntity->GetName()) != 0 && thatEntity->HasComponent<ColliderComponent>()) {
                        std::shared_ptr<ColliderComponent> thatCollider = thatEntity->GetComponent<ColliderComponent>();
                        if (Collision::CheckRectCollision(thisCollider->GetCollider(), thatCollider->GetCollider())) {
                            CollisionData collisionData;

                            collisionData.entityOne = thisEntity;
                            collisionData.entityTwo = thatEntity;

                            if (validateCollision(thisCollider,thatCollider,GameConstants::PlayerTag,GameConstants::MeteorTag)) {
                                collisionData.collisionType = GameConstants::CollisionType::PlayerMeteor;
                                _collisions.emplace_back(collisionData);
                            }

                            if (validateCollision(thisCollider,thatCollider,GameConstants::MeteorTag,GameConstants::BulletTag)) {
                                collisionData.collisionType = GameConstants::CollisionType::EnemyBullet;
                                _collisions.emplace_back(collisionData);
                            }

                            if (validateCollision(thisCollider,thatCollider,GameConstants::PlayerTag,GameConstants::LeftBoundaryTag)) {
                                collisionData.collisionType = GameConstants::CollisionType::PlayerLeftBoundary;
                                _collisions.emplace_back(collisionData);
                            }

                            if (validateCollision(thisCollider,thatCollider,GameConstants::PlayerTag,GameConstants::RightBoundaryTag)) {
                                collisionData.collisionType = GameConstants::CollisionType::PlayerRightBoundary;
                                _collisions.emplace_back(collisionData);
                            }

                            if (validateCollision(thisCollider,thatCollider,GameConstants::SpinnerTag,GameConstants::BulletTag)) {
                                collisionData.collisionType = GameConstants::CollisionType::EnemyBullet;
                                _collisions.emplace_back(collisionData);
                            }

                            if (validateCollision(thisCollider,thatCollider,GameConstants::PlayerTag,GameConstants::SpinnerTag)) {
                                collisionData.collisionType = GameConstants::CollisionType::PlayerSpinner;
                                _collisions.emplace_back(collisionData);
                            }

                            if (validateCollision(thisCollider,thatCollider,GameConstants::SpinnerTag,GameConstants::GroundTag)) {
                                collisionData.collisionType = GameConstants::CollisionType::SpinnerGround;
                                _collisions.emplace_back(collisionData);
                            }

                            if  (validateCollision(thisCollider,thatCollider,GameConstants::MeteorTag,GameConstants::GroundTag)) {

                                collisionData.collisionType = GameConstants::CollisionType::MeteorGround;
                                _collisions.emplace_back(collisionData);
                            }

                            if (validateCollision(thisCollider,thatCollider,GameConstants::MeteorTag,GameConstants::LeftBoundaryTag) ||
                                validateCollision(thisCollider,thatCollider,GameConstants::MeteorTag,GameConstants::RightBoundaryTag) ||
                                validateCollision(thisCollider,thatCollider,GameConstants::SpinnerTag,GameConstants::LeftBoundaryTag) ||
                                validateCollision(thisCollider,thatCollider,GameConstants::SpinnerTag,GameConstants::RightBoundaryTag)) {
                                collisionData.collisionType = GameConstants::CollisionType::FallingObjectBoundary;
                                _collisions.emplace_back(collisionData);
                            }
                        }
                    }
                }
            }
        }

        bEndCollisionThread = _endCollisionsThread;
    }
}


bool EntityManager::HasNoEntities() {
    return(_entities.size() == 0);
}

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
    for (int i = 0; i < _entities.size(); i++) {
        if (!_entities[i]->IsActive()) {
            _entities.erase(_entities.begin() + i);
        }
    }
}

std::shared_ptr<Entity> EntityManager::AddEntity(std::string entityName, GameConstants::EntityType entityType, GameConstants::LayerType layerType) {
    std::unique_lock<std::mutex> lock = GetLock();
    std::shared_ptr<Entity> entity = std::make_shared<Entity>(*this, entityName, entityType, layerType);
    _entities.emplace_back(entity);
    return entity;
}

std::vector<std::shared_ptr<Entity>> EntityManager::GetEntities() const {
    return(_entities);
}

std::vector<std::shared_ptr<Entity>> EntityManager::GetEntitiesByLayer(GameConstants::LayerType layerType) const {
    std::vector<std::shared_ptr<Entity>> entitiesLayer;

    for(auto & entity : _entities) {
            if (entity->GetLayer() == layerType) {
                    entitiesLayer.emplace_back(entity);
            }
    }

    return(entitiesLayer);
}

unsigned int EntityManager::GetEntityCount() {
    return(_entities.size());
}

void EntityManager::ListAllEntities() const {
    unsigned int i = 0;

    for (auto & entity : _entities) {
        std::cout << "Entity[" << i << "]: " << entity->GetName() << std::endl;
        entity->ListAllComponents();
        i++;
    }
}

