#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <string>

#include <SDL.h>

#include "game.h"
#include "transformcomponent.h"

class ColliderComponent : public Component {

        public:

            ColliderComponent(std::string colliderTag, int x, int y, int width, int height) : _colliderTag(colliderTag) {
                _collider = {x, y, width, height};
            }

            void Init() override {
                if (GetOwner()->HasComponent<TransformComponent>()) {
                    _transform = GetOwner()->GetComponent<TransformComponent>();
                    _collider.w = _transform->GetWidth();
                    _collider.h = _transform->GetHeight();
                    _srcRect = {0, 0, _transform->GetWidth(), _transform->GetHeight()};
                    _destRect = {_collider.x, _collider.y, _collider.w, _collider.h};
                }
            }

            void Update(float deltaTime) override {
                _collider.x = static_cast<int>(_transform->GetPosition().x); 
                _collider.y = static_cast<int>(_transform->GetPosition().y);
                _collider.w = _transform->GetWidth() * _transform->GetScale();
                _collider.h = _transform->GetHeight() * _transform->GetScale();
                _destRect.x = _collider.x - Game::GetGame().GetCamera().x;
                _destRect.y = _collider.y - Game::GetGame().GetCamera().y;
            }

            std::string GetColliderTag() const { return _colliderTag; }
            SDL_Rect GetCollider() const { return _collider; }

        private:
            std::string _colliderTag;
            SDL_Rect _collider;
            SDL_Rect _srcRect;
            SDL_Rect _destRect;
            TransformComponent* _transform;
};

#endif