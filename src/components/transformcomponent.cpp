#include "transformcomponent.h"

TransformComponent::TransformComponent(int posX, int posY, int velX, int velY, int w, int h, float s) : _position(glm::vec2(posX,posY)), _velocity(glm::vec2(velX,velY)),
                                                                                                        _width(w), _height(h), _scale(s) {
}

void TransformComponent::Update(float deltaTime) {
    _position.x += _velocity.x * deltaTime;
    _position.y += _velocity.y * deltaTime;
}