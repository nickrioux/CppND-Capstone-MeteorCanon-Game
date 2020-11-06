#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <iostream>
#include <string>

#include <SDL.h>

#include "texturemanager.h"
#include "transformcomponent.h"

class SpriteComponent: public Component {
    public:
        SpriteComponent(const std::string & texutreId, bool isFixed = false);
        SpriteComponent(const std::string & texutreId, int numFrames, int animationSpeed, bool isFixed = false);

        void SetTexture(const std::string & assetTextureId);

        void Init() override;

        void Update(float deltaTime) override;

        void Render() override;

        private:
            bool isFixed_{false};
            SDL_RendererFlip spriteFlip_{SDL_FLIP_NONE};
            std::shared_ptr<TransformComponent> transform_{nullptr};
            std::shared_ptr<SDL_Texture> texture_{nullptr};
            SDL_Rect sourceRectangle_;
            SDL_Rect destinationRectangle_;
            bool isAnimated_{false};
            int numFrames_{0};
            int animationSpeed_{0};
};






#endif
