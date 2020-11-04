#include "transformcomponent.h"

TransformComponent::TransformComponent(int posX, int posY, int velX, int velY, int w, int h, float s) : _position{posX,posY}, _velocity{velX,velY},
                                                                                                        _width{w}, _height{h}, _scale{s} {
}

void TransformComponent::Update(float deltaTime) {
    _position.x += _velocity.x * deltaTime;
    _position.y += _velocity.y * deltaTime;
}