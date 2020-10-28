#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <SDL.h>

#include "glm/glm.hpp"

#include "game.h"

#include "component.h"

class TransformComponent: public Component {
    public:
        TransformComponent(int posX, int posY, int velX, int velY, int w, int h, float s);

        void Update(float deltaTime) override;

        const glm::vec2 & GetPosition() const {return(_position);}
        const glm::vec2 & GetVelocity() const {return(_velocity);}
        int GetWidth() const { return(_width);}
        int GetHeight() const { return(_height);}
        float GetScale() const { return(_scale);}

        void SetPosition(int x, int y) { _position = glm::vec2(x,y);}
        void SetVelocity(int x, int y) { _velocity = glm::vec2(x,y);}
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