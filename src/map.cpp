#include <fstream>

#include "game.h"
#include "entitymanager.h"
#include "entity.h"
#include "tilecomponent.h"
#include "map.h"

Map::Map(string textureId, int scale, int tileSize) : _textureId(textureId), _scale(scale), _tileSize(tileSize) {

};

void Map::LoadMap(string filePath, int mapSizeX, int mapSizeY) {
    
    std::fstream mapFile;
    mapFile.open(filePath);
    int srcRectX,srcRectY;
    
    for (int y = 0; y < mapSizeY; ++y) {
        for (int x = 0; x < mapSizeX; ++x) {
                char ch;
                mapFile.get(ch);
                srcRectY = atoi(&ch) * _tileSize;
                mapFile.get(ch);
                srcRectX = atoi(&ch) * _tileSize;
                AddTile(srcRectX,srcRectY,x * (_scale * _tileSize),y * (_scale * _tileSize));
                mapFile.ignore();
        }
    }

    mapFile.close();
}

void Map::AddTile(int srcRectX, int srxRectY, int x, int y) {
    Entity & newTile(Game::GetGame()->GetEntityManager()->AddEntity("Tile",GameConstants::BACKGROUND_LAYER));
    newTile.AddComponent<TileComponent>(srcRectX, srxRectY, x, y, _tileSize,_scale, _textureId);
}