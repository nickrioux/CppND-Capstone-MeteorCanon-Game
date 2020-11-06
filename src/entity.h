#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include <vector>
#include <string>
#include <memory>

#include "entitymanager.h"
#include "gameconstants.h"

class Component;

class Entity : public std::enable_shared_from_this<Entity> {
    
    public :
        Entity(EntityManager& manager, const std::string & name, GameConstants::EntityType entityType, GameConstants::LayerType layerType);
        ~Entity() {};

        bool IsActive();
        void Update(float deltaTime);
        void Render();
        void Destroy();
        void ListAllComponents() const;

        const std::string & GetName() const { return name_; } 
        const GameConstants::LayerType GetLayer() const { return layerType_; }
        const GameConstants::EntityType GetEntityType() const { return entityType_; }

        const float GetLifeTime() const { return lifeTime_; }
        void SetLifeTime(float lifeTime) { lifeTime_ = lifeTime; }

        template <typename T, typename... TArgs>
        T& AddComponent(TArgs&&... args) {
            std::unique_lock<std::mutex> lock = manager_.GetLock();
            std::shared_ptr<T> newComponent(std::make_shared<T>(T(std::forward<TArgs>(args)...)));
            newComponent->SetOwner(shared_from_this());
            components_.emplace_back(newComponent);
            componentTypeMap_[&typeid(*newComponent)] = newComponent;
            newComponent->Init();
            return *newComponent;
        }

        template <typename T>
        bool HasComponent() const {
            return componentTypeMap_.count(&typeid(T));
        }

        template <typename T>
        std::shared_ptr<T> GetComponent() {
            return (std::static_pointer_cast<T>(componentTypeMap_[&typeid(T)]));
        }

    private:
        std::string name_;
        GameConstants::EntityType entityType_;
        EntityManager & manager_;
        std::vector<std::shared_ptr<Component>> components_;
        GameConstants::LayerType layerType_;
        std::map<const std::type_info*, std::shared_ptr<Component> > componentTypeMap_;
        bool isActive_;
        float lifeTime_{GameConstants::kEntityLifeInfinite};
        int timeCreation_{0};
};

#endif