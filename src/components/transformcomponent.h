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

        const glm::vec2 & GetPosition() const {return(position_);}
        const glm::vec2 & GetVelocity() const {return(velocity_);}
        int GetWidth() const { return(width_);}
        int GetHeight() const { return(height_);}
        float GetScale() const { return(scale_);}

        void SetPosition(int x, int y) { position_ = glm::vec2(x,y);}
        void SetVelocity(int x, int y) { velocity_ = glm::vec2(x,y);}
        void SetWidth(int w) { width_ = w; }
        void SetHeight(int h) { height_ = h; }

    private:
        glm::vec2 position_;
        glm::vec2 velocity_;
        int width_;
        int height_;
        float scale_;
};

#endif