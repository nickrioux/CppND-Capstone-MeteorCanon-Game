#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <map>

class Entity;

using std::map;

class Component {
        public :
            virtual ~Component() {};
            virtual void Update(float) {};
            virtual void Render() {};
            virtual void Init() {};

            std::shared_ptr<Entity> GetOwner() { return(_owner); }
            void SetOwner(std::shared_ptr<Entity>  owner) { _owner = owner; }

        private:
            std::shared_ptr<Entity> _owner;
};

#endif
