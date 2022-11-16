#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <fstream>

#include "TileSet.h"

class TileMap: public Component {
    private:
        TileSet* tileSet;
        std::vector<int> tileMatrix;
        int mapWidth, mapHeight, mapDepth;
    
    public:
        float parallaxFactor;

        TileMap(
            GameObject& associated,
            TileSet* tileSet,
            std::string file,
            float parallaxFactor=0.0f
        );
        ~TileMap();
        void Load(std::string fileName);
        void SetTileSet(TileSet* tileSet);
        int& At(int x, int y, int z=0);
        void Update(float dt);
        void Render();
        void RenderLayer(int layer, int cameraX=0, int cameraY=0);
        int GetWidth();
        int GetHeight();
        int GetDepth();
        bool Is(std::string type);
};

#endif