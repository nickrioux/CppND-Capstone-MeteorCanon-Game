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
        std::shared_ptr<SDL_Texture> _texture;
        SDL_Rect _srcRect;
        SDL_Rect _destRect;
        glm::vec2 _position;
};

#endif