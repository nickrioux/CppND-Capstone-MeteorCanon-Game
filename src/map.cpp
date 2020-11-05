#include <fstream>
#include <iostream>

#include "game.h"
#include "entitymanager.h"
#include "entity.h"
#include "components/tilecomponent.h"
#include "map.h"

Map::Map(const std::string & textureId, int scale, int tileSize) : textureId_{textureId}, scale_{scale}, tileSize_{tileSize} {

};

Map::~Map() {
        destroy();
}

void Map::LoadMap(const std::string & filePath, int mapSizeX, int mapSizeY) {
    
    std::fstream mapFile;
    mapFile.open(filePath);
    int srcRectX,srcRectY;
    
    
    for (int y = 0; y < mapSizeY; ++y) {
        for (int x = 0; x < mapSizeX; ++x) {
                char ch;
                mapFile.get(ch);
                srcRectY = atoi(&ch) * tileSize_;
                mapFile.get(ch);
                srcRectX = atoi(&ch) * tileSize_;
                AddTile(srcRectX,srcRectY,x * (scale_ * tileSize_),y * (scale_ * tileSize_));
                mapFile.ignore();
        }
    }

    mapFile.close();
}

void Map::AddTile(int srcRectX, int srxRectY, int x, int y) {
    std::shared_ptr<Entity> newTile(Game::GetGame().GetEntityManager().AddEntity("Tile",GameConstants::TileEntity,GameConstants::BackgroundLayer));
    newTile->AddComponent<TileComponent>(srcRectX, srxRectY, x, y, tileSize_,scale_, textureId_);
}

void Map::destroy() {
        std::vector<std::shared_ptr<Entity>> currentMap = Game::GetGame().GetEntityManager().GetEntitiesByLayer(GameConstants::BackgroundLayer);
        for (auto & entity : currentMap) {
                entity->Destroy();
        }
}