#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <string>

#include <SDL.h>

#include "game.h"
#include "transformcomponent.h"

class ColliderComponent : public Component {

        public:
            ColliderComponent(const GameConstants::ColliderTag & colliderTag, int x, int y, int width, int height);

            void Init() override;
            void Update(float deltaTime) override;

            GameConstants::ColliderTag GetColliderTag() const { return _colliderTag; }
            SDL_Rect GetCollider() const { return _collider; }

        private:
            GameConstants::ColliderTag _colliderTag;
            SDL_Rect _collider;
            SDL_Rect _srcRect;
            SDL_Rect _destRect;
            std::shared_ptr<TransformComponent> _transform;
};

#endif