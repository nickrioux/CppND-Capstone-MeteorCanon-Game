#include "textlabelcomponent.h"

TextLabelComponent::TextLabelComponent(int x, int y, const string & text, const string & fontFamily, const SDL_Color& color) : _position{x,y}, _text(text), _fontFamily(fontFamily), _color(color) {
    SetLabelText(_text, _fontFamily);
}

void TextLabelComponent::SetLabelText(const string & text, const string & fontFamily) {
    SDL_Surface* surface = TTF_RenderText_Blended(Game::GetGame().GetAssetManager().GetFont(fontFamily).get(), text.c_str(), _color);
    _texture = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(Game::GetGame().GetRenderer().get(), surface),SDL_DestroyTexture);
    SDL_FreeSurface(surface);
    SDL_QueryTexture(_texture.get(), NULL, NULL, &_position.w, &_position.h);
}

void TextLabelComponent::Render() {
    FontManager::Draw(_texture, _position);
}