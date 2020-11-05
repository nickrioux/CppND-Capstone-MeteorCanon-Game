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

            GameConstants::ColliderTag GetColliderTag() const { return colliderTag_; }
            SDL_Rect GetCollider() const { return collider_; }

        private:
            GameConstants::ColliderTag colliderTag_;
            SDL_Rect collider_;
            SDL_Rect srcRect_;
            SDL_Rect destRect_;
            std::shared_ptr<TransformComponent> transform_;
};

#endif