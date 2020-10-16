#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <SDL.h>

#include "glm/glm.hpp"

#include "game.h"

#include "component.h"

class TransformComponent: public Component {
    public:


        TransformComponent(int posX, int posY, int velX, int velY, int w, int h, float s) : _position(glm::vec2(posX,posY)), _velocity(glm::vec2(velX,velY)) {
            _width = w;
            _height = h;
            _scale = s;
        }

        void Init() override {
        
        }

        void Update(float deltaTime) override {
            _position.x += _velocity.x * deltaTime * Game::GetGame()->GetSpeedFactor();
            _position.y += _velocity.y * deltaTime * Game::GetGame()->GetSpeedFactor();
        }

        glm::vec2 & GetPosition()  {return(_position);}
        glm::vec2 & GetVelocity()  {return(_velocity);}
        int GetWidth() { return(_width);}
        int GetHeight() { return(_height);}
        float GetScale() { return(_scale);}

        void SetWidth(int w) { _width = w; }
        void SetHeight(int h) { _height = h; }

    private:
        glm::vec2 _position;
        glm::vec2 _velocity;
        int _width;
        int _height;
        float _scale;
};

#endif