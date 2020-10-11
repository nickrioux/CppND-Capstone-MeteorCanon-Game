#include <iostream>

#include "entitymanager.h"
#include "entity.h"
#include "component.h"

Entity::Entity(EntityManager& manager): _manager(manager) {
    _isActive = true;
}

Entity::Entity(EntityManager& manager, std::string name, GameConstants::LayerType layerType): _manager(manager), _name(name), _layerType(layerType) {
    _isActive = true;
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

bool Entity::IsActive() const {
    return _isActive;
}

void Entity::ListAllComponents() const {
    for (auto mapElement: _componentTypeMap) {
        std::cout << "    Component<" << mapElement.first->name() << ">" << std::endl;
    }
}

