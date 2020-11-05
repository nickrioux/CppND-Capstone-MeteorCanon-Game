#include "entity.h"

#include "collidercomponent.h"

ColliderComponent::ColliderComponent(const GameConstants::ColliderTag & colliderTag, int x, int y, int width, int height) 
                    : colliderTag_{colliderTag}, collider_({x,y,width,height}) {
}

void ColliderComponent::Init() {
    if (GetOwner()->HasComponent<TransformComponent>()) {
        transform_ = GetOwner()->GetComponent<TransformComponent>();
        collider_.w = transform_->GetWidth();
        collider_.h = transform_->GetHeight();
        srcRect_ = {0, 0, transform_->GetWidth(), transform_->GetHeight()};
        destRect_ = {collider_.x, collider_.y, collider_.w, collider_.h};
    }
}

void ColliderComponent::Update(float deltaTime) {
    collider_.x = static_cast<int>(transform_->GetPosition().x); 
    collider_.y = static_cast<int>(transform_->GetPosition().y);
    collider_.w = transform_->GetWidth() * transform_->GetScale();
    collider_.h = transform_->GetHeight() * transform_->GetScale();
    destRect_.x = collider_.x - Game::GetGame().GetCamera().x;
    destRect_.y = collider_.y - Game::GetGame().GetCamera().y;
}