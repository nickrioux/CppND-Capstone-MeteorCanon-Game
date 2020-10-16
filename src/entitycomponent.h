#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

#include "gameconstants.h"
#include "entity.h"
#include "component.h"

class EntityComponent public: Component {
    public:
        EntityComponent(Entity * entity) : _entity(entity)
        {};

        void Init() override {};

        void Update(float deltaTime) override {
                _entity->Update(deltaTime);
        }

        void Render() override {
                _entity->Render();
        }
};

#endif