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
        SDL_Rect _position;
        std::string _text;
        std::string _fontFamily;
        SDL_Color _color;
        std::shared_ptr<SDL_Texture> _texture;
};

#endif