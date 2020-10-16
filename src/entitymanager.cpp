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
            ColliderComponent* thisCollider = thisEntity->GetComponent<ColliderComponent>();
            for (int j = 0; j < _entities.size(); j++) {
                auto& thatEntity = _entities[j];
                if (thisEntity->GetName().compare(thatEntity->GetName()) != 0 && thatEntity->HasComponent<ColliderComponent>()) {
                    ColliderComponent* thatCollider = thatEntity->GetComponent<ColliderComponent>();
                    if (Collision::CheckRectCollision(thisCollider->GetCollider(), thatCollider->GetCollider())) {
                        CollisionData collisionData;

                        collisionData.entityOne = thisEntity;
                        collisionData.entityTwo = thatEntity;

                        if (thisCollider->GetColliderTag().compare("PLAYER") == 0 && thatCollider->GetColliderTag().compare("METEOR") == 0) {
                            collisionData.collisionType = GameConstants::PLAYER_METEOR_COLLISION;
                            collisions.emplace_back(collisionData);
                        }
                        if (thisCollider->GetColliderTag().compare("PLAYER") == 0 && thatCollider->GetColliderTag().compare("MISSILE") == 0) {
                            collisionData.collisionType = GameConstants::PLAYER_MISSILE_COLLISION;
                            collisions.emplace_back(collisionData);
                        }
                        if (thisCollider->GetColliderTag().compare("METEOR") == 0 && thatCollider->GetColliderTag().compare("BULLET") == 0) {
                            collisionData.collisionType = GameConstants::ENEMY_BULLET_COLLISION;
                            collisions.emplace_back(collisionData);
                        }
                        if (thisCollider->GetColliderTag().compare("PLAYER") == 0 && thatCollider->GetColliderTag().compare("LEFT_BOUNDARY") == 0) {
                            collisionData.collisionType = GameConstants::PLAYER_LEFT_BOUNDARY_COLLISION;
                            collisions.emplace_back(collisionData);
                        }
                        if (thisCollider->GetColliderTag().compare("PLAYER") == 0 && thatCollider->GetColliderTag().compare("RIGHT_BOUNDARY") == 0) {
                            collisionData.collisionType = GameConstants::PLAYER_RIGHT_BOUNDARY_COLLISION;
                            collisions.emplace_back(collisionData);
                        }
                        if (thisCollider->GetColliderTag().compare("PLAYER") == 0 && thatCollider->GetColliderTag().compare("SPINNER") == 0) {
                            collisionData.collisionType = GameConstants::PLAYER_SPINNER_COLLISION;
                            collisions.emplace_back(collisionData);
                        }
                        if (thisCollider->GetColliderTag().compare("METEOR") == 0 && thatCollider->GetColliderTag().compare("GROUND") == 0) {
                            collisionData.collisionType = GameConstants::METEOR_GROUND_COLLISION;
                            collisions.emplace_back(collisionData);
                        }
                        if (thisCollider->GetColliderTag().compare("METEOR") == 0 && thatCollider->GetColliderTag().compare("LEFT_BOUNDARY") == 0) {
                            collisionData.collisionType = GameConstants::METEOR_BOUNDARY_COLLISION;
                            collisions.emplace_back(collisionData);
                        }
                        if (thisCollider->GetColliderTag().compare("METEOR") == 0 && thatCollider->GetColliderTag().compare("RIGHT_BOUNDARY") == 0) {
                            collisionData.collisionType = GameConstants::METEOR_BOUNDARY_COLLISION;
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

void EntityManager::destroyInactiveEntities() {
    for (int i = 0; i < _entities.size(); i++) {
        if (!_entities[i]->IsActive()) {
            _entities.erase(_entities.begin() + i);
        }
    }
}

Entity & EntityManager::AddEntity(std::string entityName, GameConstants::EntityType entityType, GameConstants::LayerType layerType) {
    Entity *entity = new Entity(*this, entityName, entityType, layerType);
    _entities.emplace_back(entity);
    return *entity;
}

std::vector<Entity *> EntityManager::GetEntities() const {
    return(_entities);
}

std::vector<Entity *> EntityManager::GetEntitiesByLayer(GameConstants::LayerType layerType) const {
    std::vector<Entity*> entitiesLayer;

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

