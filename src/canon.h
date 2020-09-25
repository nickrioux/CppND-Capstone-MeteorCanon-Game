#ifndef _CANON_H
#define _CANON_H

#include "sprite.h"
#include "controller.h"
#include "gameobject.h"


class CanonGraphicsComponent : public GraphicsComponent {
        public :
            CanonGraphicsComponent(std::shared_ptr<Sprite> sprite) : _sprite(sprite) {};

            void render(GameObject & obj, Renderer & renderer);

        private:
            std::shared_ptr<Sprite> _sprite;
};

class ControllerInputComponent : public InputComponent {
        public:
           enum class Direction { kLeft, kRight, kNone };
            ControllerInputComponent(Controller const & controller) : _controller(controller) {};
            void update(GameObject & obj, Game & game);
        private:
            Controller const & _controller;
            Direction _currentDirection{Direction::kNone};
};

class CanonPhysicsComponent : public PhysicsComponent {
        public:
            CanonPhysicsComponent() {};
            void update(GameObject & obj, Game & game) {};
};

class Canon : public GameObject {
        public :
            Canon(ControllerInputComponent * controllerComponent,
                  CanonPhysicsComponent * physicsComponent, 
                  CanonGraphicsComponent * graphicsComponent,float w, float h) 
            : GameObject(controllerComponent, physicsComponent, graphicsComponent,w,h) {};
            ~Canon() {};

        private :
};

#endif