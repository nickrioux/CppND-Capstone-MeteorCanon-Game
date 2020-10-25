#include <iostream>
#include <memory>

#include "game.h"
#include "entity.h"
#include "collision.h"
#include "collidercomponent.h"
#include "entitymanager.h"

void EntityManager::ClearData() {
    for (auto & entity : _entities) {
            entity->Destroy();
    }
}

void EntityManager::Update(float deltaTime) {
    for (auto & entity : _entities) {
            entity->Update(deltaTime);
    }

    destroyInactiveEntities();
}

void EntityManager::Render() {
    //TODO - Final Solution (Sort Entity by Layer when adding a new Entity)

    for (int layerNumber = 0; layerNumber < GameConstants::NUM_LAYERS; ++layerNumber) {
        for (auto & entity : GetEntitiesByLayer(static_cast<GameConstants::LayerType>(layerNumber))) {
                entity->Render();
        }
    }
}

vector<EntityManager::CollisionData> EntityManager::CheckCollisions() const {
    vector<EntityManager::CollisionData> collisions;

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

                        if (validateCollision(thisCollider,thatCollider,"PLAYER","METEOR")) {
                            collisionData.collisionType = GameConstants::PLAYER_METEOR_COLLISION;
                            collisions.emplace_back(collisionData);
                        }

                        if (validateCollision(thisCollider,thatCollider,"METEOR","BULLET")) {
                            collisionData.collisionType = GameConstants::ENEMY_BULLET_COLLISION;
                            collisions.emplace_back(collisionData);
                        }

                        if (validateCollision(thisCollider,thatCollider,"PLAYER","LEFT_BOUDARY")) {
                            collisionData.collisionType = GameConstants::PLAYER_LEFT_BOUNDARY_COLLISION;
                            collisions.emplace_back(collisionData);
                        }

                        if (validateCollision(thisCollider,thatCollider,"PLAYER","RIGHT_BOUNDARY")) {
                            collisionData.collisionType = GameConstants::PLAYER_RIGHT_BOUNDARY_COLLISION;
                            collisions.emplace_back(collisionData);
                        }

                        if (validateCollision(thisCollider,thatCollider,"SPINNER","BULLET")) {
                            collisionData.collisionType = GameConstants::ENEMY_BULLET_COLLISION;
                            collisions.emplace_back(collisionData);
                        }

                        if (validateCollision(thisCollider,thatCollider,"PLAYER","SPINNER")) {
                            collisionData.collisionType = GameConstants::PLAYER_SPINNER_COLLISION;
                            collisions.emplace_back(collisionData);
                        }

                        if (validateCollision(thisCollider,thatCollider,"SPINNER","GROUND")) {
                            collisionData.collisionType = GameConstants::SPINNER_GROUND_COLLISION;
                            collisions.emplace_back(collisionData);
                        }

                        if  (validateCollision(thisCollider,thatCollider,"METEOR","GROUND")) {

                            collisionData.collisionType = GameConstants::METEOR_GROUND_COLLISION;
                            collisions.emplace_back(collisionData);
                        }

                        if (validateCollision(thisCollider,thatCollider,"METEOR","LEFT_BOUNDARY") ||
                            validateCollision(thisCollider,thatCollider,"METEOR","RIGHT_BOUNDARY") ||
                            validateCollision(thisCollider,thatCollider,"SPINNER","LEFT_BOUNDARY") ||
                            validateCollision(thisCollider,thatCollider,"SPINNER","RIGHT_BOUNDARY")) {
                            collisionData.collisionType = GameConstants::FALLING_OBJECT_BOUNDARY_COLLISION;
                            collisions.emplace_back(collisionData);
                        }
                    }
                }
            }
        }
    }

    return collisions;
}

bool EntityManager::validateCollision(const std::shared_ptr<ColliderComponent> & colliderOne, const std::shared_ptr<ColliderComponent> & colliderTwo, 
                                      const string & collOneName, const string & collTwoName) const {
    if ((colliderOne->GetColliderTag().compare(collOneName) == 0 && colliderTwo->GetColliderTag().compare(collTwoName) == 0) ||
        (colliderOne->GetColliderTag().compare(collTwoName) == 0 && colliderTwo->GetColliderTag().compare(collOneName) == 0)) {
            return true;
    }
    else {
        return false;
    } 
}

bool EntityManager::HasNoEntities() {
    return(_entities.size() == 0);
}

void EntityManager::destroyInactiveEntities() {
    for (int i = 0; i < _entities.size(); i++) {
        if (!_entities[i]->IsActive()) {
            _entities.erase(_entities.begin() + i);
        }
    }
}

std::shared_ptr<Entity> EntityManager::AddEntity(std::string entityName, GameConstants::EntityType entityType, GameConstants::LayerType layerType) {
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

