#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include <vector>
#include <string>
#include <memory>

#include "entitymanager.h"
#include "gameconstants.h"

using std::vector;
using std::string;

class Component;

class Entity : public std::enable_shared_from_this<Entity> {
    
    public :
        Entity(EntityManager& manager);
        Entity(EntityManager& manager, const string & name, GameConstants::EntityType entityType, GameConstants::LayerType layerType);
        ~Entity() {};

        bool IsActive();
        void Update(float deltaTime);
        void Render();
        void Destroy();
        void ListAllComponents() const;

        const string & GetName() const { return _name; } 
        const GameConstants::LayerType GetLayer() const { return _layerType; }
        const GameConstants::EntityType GetEntityType() const { return _entityType; }

        const float GetLifeTime() const { return _lifeTime; }
        void SetLifeTime(float lifeTime) { _lifeTime = lifeTime; }

        template <typename T, typename... TArgs>
        T& AddComponent(TArgs&&... args) {
            std::unique_lock<std::mutex> lock = _manager.GetLock();
            std::shared_ptr<T> newComponent(std::make_shared<T>(T(std::forward<TArgs>(args)...)));
            newComponent->SetOwner(shared_from_this());
            _components.emplace_back(newComponent);
            _componentTypeMap[&typeid(*newComponent)] = newComponent;
            newComponent->Init();
            return *newComponent;
        }

        template <typename T>
        bool HasComponent() const {
            return _componentTypeMap.count(&typeid(T));
        }

        template <typename T>
        std::shared_ptr<T> GetComponent() {
            return (std::static_pointer_cast<T>(_componentTypeMap[&typeid(T)]));
        }

    private:
        string _name;
        GameConstants::EntityType _entityType;
        EntityManager & _manager;
        vector<std::shared_ptr<Component>> _components;
        GameConstants::LayerType _layerType;
        std::map<const std::type_info*, std::shared_ptr<Component> > _componentTypeMap;
        bool _isActive;
        float _lifeTime{GameConstants::kEntityLifeInfinite};
        int _timeCreation{0};
};

#endif