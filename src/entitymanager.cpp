#include <iostream>
#include <memory>

#include "game.h"
#include "entity.h"
#include "collision.h"
#include "components/collidercomponent.h"
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

    for (int layerNumber = 0; layerNumber < GameConstants::kNumLayers; ++layerNumber) {
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

                        if (validateCollision(thisCollider,thatCollider,GameConstants::PlayerTag,GameConstants::MeteorTag)) {
                            collisionData.collisionType = GameConstants::CollisionType::PlayerMeteor;
                            collisions.emplace_back(collisionData);
                        }

                        if (validateCollision(thisCollider,thatCollider,GameConstants::MeteorTag,GameConstants::BulletTag)) {
                            collisionData.collisionType = GameConstants::CollisionType::EnemyBullet;
                            collisions.emplace_back(collisionData);
                        }

                        if (validateCollision(thisCollider,thatCollider,GameConstants::PlayerTag,GameConstants::LeftBoundaryTag)) {
                            collisionData.collisionType = GameConstants::CollisionType::PlayerLeftBoundary;
                            collisions.emplace_back(collisionData);
                        }

                        if (validateCollision(thisCollider,thatCollider,GameConstants::PlayerTag,GameConstants::RightBoundaryTag)) {
                            collisionData.collisionType = GameConstants::CollisionType::PlayerRightBoundary;
                            collisions.emplace_back(collisionData);
                        }

                        if (validateCollision(thisCollider,thatCollider,GameConstants::SpinnerTag,GameConstants::BulletTag)) {
                            collisionData.collisionType = GameConstants::CollisionType::EnemyBullet;
                            collisions.emplace_back(collisionData);
                        }

                        if (validateCollision(thisCollider,thatCollider,GameConstants::PlayerTag,GameConstants::SpinnerTag)) {
                            collisionData.collisionType = GameConstants::CollisionType::PlayerSpinner;
                            collisions.emplace_back(collisionData);
                        }

                        if (validateCollision(thisCollider,thatCollider,GameConstants::SpinnerTag,GameConstants::GroundTag)) {
                            collisionData.collisionType = GameConstants::CollisionType::SpinnerGround;
                            collisions.emplace_back(collisionData);
                        }

                        if  (validateCollision(thisCollider,thatCollider,GameConstants::MeteorTag,GameConstants::GroundTag)) {

                            collisionData.collisionType = GameConstants::CollisionType::MeteorGround;
                            collisions.emplace_back(collisionData);
                        }

                        if (validateCollision(thisCollider,thatCollider,GameConstants::MeteorTag,GameConstants::LeftBoundaryTag) ||
                            validateCollision(thisCollider,thatCollider,GameConstants::MeteorTag,GameConstants::RightBoundaryTag) ||
                            validateCollision(thisCollider,thatCollider,GameConstants::SpinnerTag,GameConstants::LeftBoundaryTag) ||
                            validateCollision(thisCollider,thatCollider,GameConstants::SpinnerTag,GameConstants::RightBoundaryTag)) {
                            collisionData.collisionType = GameConstants::CollisionType::FallingObjectBoundary;
                            collisions.emplace_back(collisionData);
                        }
                    }
                }
            }
        }
    }

    return collisions;
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

