#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <map>

#include <SDL.h>
#include <SDL_ttf.h>

#include "entitymanager.h"

using std::string;
using std::map;

class AssetManager {
    public :
        AssetManager();
        ~AssetManager();
        void ClearData();
        void AddTexture(string textureId, const string & filePath);
        void AddFont(std::string fontId, const string & filePath, int fontSize);        
        SDL_Texture* GetTexture(std::string textureId);
        TTF_Font* GetFont(std::string fontId);


    private:
        map<string, SDL_Texture*> _textures;
        map<std::string, TTF_Font*> _fonts;

};

#endif