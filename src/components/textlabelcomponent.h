#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "fontmanager.h"
#include "entitymanager.h"
#include "assetmanager.h"
#include "game.h"

class TextLabelComponent: public Component {

    public:
        TextLabelComponent(int x, int y, const std::string & text, const std::string & fontFamily, const SDL_Color& color);

        void SetLabelText(const std::string & text, const std::string & fontFamily);

        void Render() override;
        
    private:
        SDL_Rect position_;
        std::string text_;
        std::string fontFamily_;
        SDL_Color color_;
        std::shared_ptr<SDL_Texture> texture_{nullptr};
};

#endif