#ifndef MAP_H
#define MAP_H

#include <string>

using std::string;

class Map {
    public :
        Map(string textureId, int scale, int tileSize);
        ~Map();
        void LoadMap(string filePath, int mapSizeX, int mapSizeY);
        void AddTile(int srcX, int srxY, int x, int y);

    private:
        string _textureId;
        int _scale;
        int _tileSize;
};

#endif