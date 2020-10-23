#ifndef COMPONENT_H
#define COMPONENT_H

#include <map>

class Entity;

using std::map;

class Component {
        public :
            virtual ~Component() {};
            virtual void Update(float) {};
            virtual void Render() {};
            virtual void Init() {};

            Entity * GetOwner() { return(_owner); }
            void SetOwner(Entity * owner) { _owner = owner; }

        private:
            Entity * _owner;
            map<const std::type_info*, Component*> componentTypeMap;
};

#endif