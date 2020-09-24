#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <memory>
#include "renderer.h"

//Reference fo Game Programming Patterns book
class Game;
class GameObject;

class InputComponent {
    public:
        virtual ~InputComponent() {};
        virtual void update(GameObject & obj, Game & game) = 0;
};

class PhysicsComponent {
    public :

        virtual ~PhysicsComponent() {};
        virtual void update(GameObject & obj, Game & game) = 0;
};

class GraphicsComponent {
    public :
        virtual ~GraphicsComponent() {}
        virtual void render(GameObject & obj, Renderer & renderer) = 0;
};

class GameObject {
    
    public :
        float _x, _y;
        float _velocity;

        GameObject(InputComponent * input,
                   PhysicsComponent * physics,
                   GraphicsComponent * graphics) 
        : _input(input), 
          _physics(physics), 
          _graphics(graphics)
        {}

        ~GameObject() {};

        float GetVelocity() { return _velocity; }
        float GetX() { return _x; }
        float GetY() { return _y; }

        void update(Game & game) {
                _input->update(*this,game);
                _physics->update(*this, game);
        };

        void render(Renderer & renderer) {
                _graphics->render(*this, renderer);
        }

    protected : 



    
    private:


        InputComponent * _input;
        PhysicsComponent * _physics;
        GraphicsComponent * _graphics;
};

#endif