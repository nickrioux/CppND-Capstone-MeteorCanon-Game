#include <iostream>

#include <SDL.h>
#include "entitymanager.h"
#include "entity.h"
#include "component.h"

Entity::Entity(EntityManager& manager): _manager(manager) {
    _isActive = true;
    _timeCreation = SDL_GetTicks();
}

Entity::Entity(EntityManager& manager, std::string name, GameConstants::EntityType entityType, GameConstants::LayerType layerType): _manager(manager), _name(name), _entityType(entityType), _layerType(layerType) {
    _isActive = true;
    _timeCreation = SDL_GetTicks();
}

void Entity::Update(float deltaTime) {
    for (auto& component: _components) {
        component->Update(deltaTime);
    }
}

void Entity::Render() {
    for (auto& component: _components) {
        component->Render();
    }
}

void Entity::Destroy() {
    _isActive = false;
}

bool Entity::IsActive() {
    if (GetLifeTime() == GameConstants::ENTITY_LIFE_INFINITE) {
        return _isActive;
    }
    else
    {
        //Delta Time
        int currTicks = SDL_GetTicks();
        float deltaTime = (currTicks - _timeCreation) / 1000.0f;
        if (deltaTime > static_cast<float>(GetLifeTime()))
        {
            _isActive = false;
        }

        return _isActive;
    }
    
}

void Entity::ListAllComponents() const {
    for (auto mapElement: _componentTypeMap) {
        std::cout << "    Component<" << mapElement.first->name() << ">" << std::endl;
    }
}

