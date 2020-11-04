#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <map>

#include <SDL.h>
#include <SDL_ttf.h>

#include "entitymanager.h"

class AssetManager {
    public :
        void ClearData();
        void AddTexture(std::string textureId, const std::string & filePath);
        void AddFont(std::string fontId, const std::string & filePath, int fontSize);        
        std::shared_ptr<SDL_Texture> GetTexture(std::string textureId);
        std::shared_ptr<TTF_Font> GetFont(std::string fontId);


    private:
        std::map<std::string, std::shared_ptr<SDL_Texture>> _textures;
        std::map<std::string, std::shared_ptr<TTF_Font>> _fonts;

};

#endif