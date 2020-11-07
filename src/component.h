#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <map>

class Entity;

class Component {
        public :
            virtual ~Component() {};
            virtual void Update(float) {};
            virtual void Render() {};
            virtual void Init() {};

            std::shared_ptr<Entity> GetOwner() { return(_owner.lock()); }
            void SetOwner(std::shared_ptr<Entity>  owner) { _owner = owner; }

        private:
            std::weak_ptr<Entity> _owner;
};

#endif
