#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "fontmanager.h"
#include "entitymanager.h"
#include "assetmanager.h"
#include "game.h"

using std::string;

class TextLabelComponent: public Component {

    public:
        TextLabelComponent(int x, int y, const string & text, const string & fontFamily, const SDL_Color& color);

        void SetLabelText(const string & text, const string & fontFamily);

        void Render() override;
        
    private:
        SDL_Rect _position;
        string _text;
        string _fontFamily;
        SDL_Color _color;
        std::shared_ptr<SDL_Texture> _texture;
};

#endif