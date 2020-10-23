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
        TextLabelComponent(int x, int y, string text, string fontFamily, const SDL_Color& color) : _position{x,y}, _text(text), _fontFamily(fontFamily), _color(color) {
            SetLabelText(_text, _fontFamily);
        }

        void SetLabelText(string text, std::string fontFamily) {
            SDL_Surface* surface = TTF_RenderText_Blended(Game::GetGame().GetAssetManager().GetFont(fontFamily), text.c_str(), _color);
            _texture = SDL_CreateTextureFromSurface(Game::GetGame().GetRenderer(), surface);
            SDL_FreeSurface(surface);
            SDL_QueryTexture(_texture, NULL, NULL, &_position.w, &_position.h);
        }

        void Render() override {
            FontManager::Draw(_texture, _position);
        }

    private:
        SDL_Rect _position;
        string _text;
        string _fontFamily;
        SDL_Color _color;
        SDL_Texture* _texture;
};

#endif