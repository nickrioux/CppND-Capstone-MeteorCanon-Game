#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include <SDL.h>

#include "glm/glm.hpp" 

#include "game.h"
#include "assetmanager.h"
#include "texturemanager.h"

#include "component.h"

class TileComponent : public Component {
    public :
        TileComponent(int srcRectX, int srcRectY, int x, int y, int tileSize, int tileScale, const std::string & textureId);
                
        void Update(float deltaTime) override;
        void Render() override;

    private:
        std::shared_ptr<SDL_Texture> texture_{nullptr};
        SDL_Rect srcRect_;
        SDL_Rect destRect_;
        glm::vec2 position_;
};

#endif