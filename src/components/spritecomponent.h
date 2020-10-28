#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <iostream>
#include <string>

#include <SDL.h>

#include "texturemanager.h"
#include "transformcomponent.h"

using std::string;

class SpriteComponent: public Component {
    public:
        SpriteComponent(const string & filePath, bool isFixed = false);
        SpriteComponent(const std::string & id, int numFrames, int animationSpeed, bool isFixed = false);

        void SetTexture(const std::string & assetTextureId);

        void Init() override;

        void Update(float deltaTime) override;

        void Render() override;

        private:
            bool _isFixed{false};
            SDL_RendererFlip _spriteFlip = SDL_FLIP_NONE;
            std::shared_ptr<TransformComponent> _transform;
            std::shared_ptr<SDL_Texture> _texture;
            SDL_Rect _sourceRectangle;
            SDL_Rect _destinationRectangle;
            bool _isAnimated{false};
            int _numFrames;
            int _animationSpeed;
};






#endif
