#include <iostream>
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
}

void EntityManager::Render() {
    //TODO - Final Solution (Sort Entity by Layer when adding a new Entity)

    for (int layerNumber = 0; layerNumber < GameConstants::NUM_LAYERS; ++layerNumber) {
        for (auto & entity : GetEntitiesByLayer(static_cast<GameConstants::LayerType>(layerNumber))) {
                entity->Render();
        }
    }
}

GameConstants::CollisionType EntityManager::CheckCollisions() const {
    for (int i = 0; i < _entities.size() - 1; i++) {
        auto& thisEntity = _entities[i];
        if (thisEntity->HasComponent<ColliderComponent>()) {
            ColliderComponent* thisCollider = thisEntity->GetComponent<ColliderComponent>();
            for (int j = i + 1; j < _entities.size(); j++) {
                auto& thatEntity = _entities[j];
                if (thisEntity->GetName().compare(thatEntity->GetName()) != 0 && thatEntity->HasComponent<ColliderComponent>()) {
                    ColliderComponent* thatCollider = thatEntity->GetComponent<ColliderComponent>();
                    if (Collision::CheckRectCollision(thisCollider->GetCollider(), thatCollider->GetCollider())) {
                        if (thisCollider->GetColliderTag().compare("PLAYER") == 0 && thatCollider->GetColliderTag().compare("METEOR") == 0) {
                            return GameConstants::PLAYER_METEOR_COLLISION;
                        }
                        if (thisCollider->GetColliderTag().compare("PLAYER") == 0 && thatCollider->GetColliderTag().compare("MISSILE") == 0) {
                            return GameConstants::PLAYER_MISSILE_COLLISION;
                        }
                        if (thisCollider->GetColliderTag().compare("METEOR") == 0 && thatCollider->GetColliderTag().compare("FRIENDLY_BULLET") == 0) {
                            return GameConstants::ENEMY_BULLET_COLLISION;
                        }
                        if (thisCollider->GetColliderTag().compare("PLAYER") == 0 && thatCollider->GetColliderTag().compare("LEFT_BOUNDARY") == 0) {
                            return GameConstants::PLAYER_LEFT_BOUNDARY_COLLISION;
                        }
                        if (thisCollider->GetColliderTag().compare("PLAYER") == 0 && thatCollider->GetColliderTag().compare("RIGHT_BOUNDARY") == 0) {
                            return GameConstants::PLAYER_RIGHT_BOUNDARY_COLLISION;
                        }
                        if (thisCollider->GetColliderTag().compare("PLAYER") == 0 && thatCollider->GetColliderTag().compare("SPINNER") == 0) {
                            return GameConstants::PLAYER_SPINNER_COLLISION;
                        }
                        if (thisCollider->GetColliderTag().compare("METEOR") == 0 && thatCollider->GetColliderTag().compare("GROUND") == 0) {
                            return GameConstants::METEOR_GROUND_COLLISION;
                        }
                    }
                }
            }
        }
    }
    return GameConstants::NO_COLLISION;
}

bool EntityManager::HasNoEntities() {
    return(_entities.size() == 0);
}

Entity & EntityManager::AddEntity(std::string entityName, GameConstants::LayerType layerType) {
    Entity *entity = new Entity(*this, entityName, layerType);
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

