#include <iostream>

#include "textlabelcomponent.h"

TextLabelComponent::TextLabelComponent(int x, int y, const std::string & text, const std::string & fontFamily, const SDL_Color& color) : position_{x,y}, text_{text}, fontFamily_{fontFamily}, color_{color} {
    SetLabelText(text_, fontFamily_);
}

void TextLabelComponent::SetLabelText(const std::string & text, const std::string & fontFamily) {
    text_ = text;
    SDL_Surface* surface = TTF_RenderText_Blended(Game::GetGame().GetAssetManager().GetFont(fontFamily).get(), text_.c_str(), color_);
    texture_ = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(Game::GetGame().GetRenderer().get(), surface),SDL_DestroyTexture);
    SDL_FreeSurface(surface);
    SDL_QueryTexture(texture_.get(), NULL, NULL, &position_.w, &position_.h);
}

void TextLabelComponent::Render() {
    FontManager::Draw(texture_, position_);
}