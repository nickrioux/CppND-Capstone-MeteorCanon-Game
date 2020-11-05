#include <iostream>

#include <SDL.h>
#include "entitymanager.h"
#include "entity.h"
#include "component.h"

Entity::Entity(EntityManager& manager, const std::string & name, GameConstants::EntityType entityType, GameConstants::LayerType layerType)
        :   manager_{manager}, 
            name_{name}, entityType_{entityType}, 
            isActive_{true}, layerType_{layerType},
            timeCreation_(SDL_GetTicks()) {
}

void Entity::Update(float deltaTime) {
    for (auto& component: components_) {
        component->Update(deltaTime);
    }
}

void Entity::Render() {
    for (auto& component: components_) {
        component->Render();
    }
}

void Entity::Destroy() {
    isActive_ = false;
}

bool Entity::IsActive() {
    if (GetLifeTime() == GameConstants::kEntityLifeInfinite) {
        return isActive_;
    }
    else
    {
        //Delta Time
        int currTicks = SDL_GetTicks();
        float deltaTime = (currTicks - timeCreation_) / 1000.0f;
        if (deltaTime > static_cast<float>(GetLifeTime()))
        {
            isActive_ = false;
        }

        return isActive_;
    }
    
}

void Entity::ListAllComponents() const {
    for (auto mapElement: componentTypeMap_) {
        std::cout << "    Component<" << mapElement.first->name() << ">" << std::endl;
    }
}

