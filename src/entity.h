#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include <vector>
#include <string>
#include <memory>

#include "gameconstants.h"

using std::vector;
using std::string;

class EntityManager;
class Component;

class Entity {
    
    public :
        Entity(EntityManager& manager);
        Entity(EntityManager& manager, std::string name, GameConstants::LayerType layerType);
        ~Entity() {};

        bool IsActive() const;
        void Update(float deltaTime);
        void Render();
        void Destroy();
        void ListAllComponents() const;

        const string & GetName() const { return _name; } 
        const GameConstants::LayerType GetLayer() const { return _layerType; }

        template <typename T, typename... TArgs>
        T& AddComponent(TArgs&&... args) {
            T* newComponent(new T(std::forward<TArgs>(args)...));
            newComponent->SetOwner(this);
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
        T* GetComponent() {
            return static_cast<T*>(_componentTypeMap[&typeid(T)]);
        }

    private:
        string _name;
        EntityManager & _manager;
        vector<Component *> _components;
        GameConstants::LayerType _layerType;
        std::map<const std::type_info*, Component*> _componentTypeMap;
        bool _isActive;

};

#endif