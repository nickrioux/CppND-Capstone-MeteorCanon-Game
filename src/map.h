#ifndef MAP_H
#define MAP_H

#include <string>

class Map {
    public :
        Map(const std::string & textureId, int scale, int tileSize);
        ~Map();
        void LoadMap(const std::string & filePath, int mapSizeX, int mapSizeY);
        void AddTile(int srcX, int srxY, int x, int y);

    private:
        std::string _textureId;
        int _scale;
        int _tileSize;

        void destroy();
};

#endif