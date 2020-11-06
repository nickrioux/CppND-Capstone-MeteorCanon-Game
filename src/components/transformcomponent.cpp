#include "transformcomponent.h"

TransformComponent::TransformComponent(int posX, int posY, int velX, int velY, int w, int h, float s) : position_{posX,posY}, velocity_{velX,velY},
                                                                                                        width_{w}, height_{h}, scale_{s} {
}

void TransformComponent::Update(float deltaTime) {
    position_.x += velocity_.x * deltaTime;
    position_.y += velocity_.y * deltaTime;
}