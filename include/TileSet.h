#ifndef TILE_SET_H
#define TILE_SET_H

#include "Sprite.h"

class TileSet {
    private:
        Sprite tileSet;
        int rows;
        int columns;
        int tileWidth;
        int tileHeight;
    
    public:
        TileSet(
            GameObject& associated, std::string file,
            int tileWidth, int tileHeight
        );
        void RenderTile (unsigned index, float x, float y);
        int GetTileWidth();
        int GetTileHeight();
};

#endif