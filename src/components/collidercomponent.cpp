#include "entity.h"

#include "collidercomponent.h"

ColliderComponent::ColliderComponent(const GameConstants::ColliderTag & colliderTag, int x, int y, int width, int height) : _colliderTag{colliderTag}, _collider({x,y,width,height}) {
}

void ColliderComponent::Init() {
    if (GetOwner()->HasComponent<TransformComponent>()) {
        _transform = GetOwner()->GetComponent<TransformComponent>();
        _collider.w = _transform->GetWidth();
        _collider.h = _transform->GetHeight();
        _srcRect = {0, 0, _transform->GetWidth(), _transform->GetHeight()};
        _destRect = {_collider.x, _collider.y, _collider.w, _collider.h};
    }
}

void ColliderComponent::Update(float deltaTime) {
    _collider.x = static_cast<int>(_transform->GetPosition().x); 
    _collider.y = static_cast<int>(_transform->GetPosition().y);
    _collider.w = _transform->GetWidth() * _transform->GetScale();
    _collider.h = _transform->GetHeight() * _transform->GetScale();
    _destRect.x = _collider.x - Game::GetGame().GetCamera().x;
    _destRect.y = _collider.y - Game::GetGame().GetCamera().y;
}