#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <fstream>

#include "TileSet.h"

struct RectInt{
    int x, y, w, h;
};

class TileMap: public Component {
    private:
        TileSet* tileSet;
        std::vector<std::vector<int>> _tileMatrix;
        int mapWidth, mapHeight, mapDepth;

        //Collision automation functions
        int Hash(std::vector<std::vector<int>>& Map);
        bool HasMapChanged(int HashResult, std::string HashPath);
        std::vector<RectInt> LoadCoord(std::string File);
        void DumpCoord(const std::vector<RectInt>& Data, std::string File);
        void BorderMarking(std::vector<std::vector<int>> &Map);
        void FindX(std::vector<std::vector<int>>& Map);
        std::vector<RectInt> MapToCoord(std::vector<std::vector<int>>& Map);
        void CoordOptimizer(std::vector<RectInt>& Coords);
    
    public:
        float parallaxFactor;

        TileMap(
            GameObject& associated,
            TileSet* tileSet,
            std::string file,
            float parallaxFactor=1.0f
        );
        ~TileMap();
        void Load(std::string fileName);
        void LoadCollision(std::string fileName);
        void SetTileSet(TileSet* tileSet);
        void Update(float dt);
        void Render();
        void RenderLayer(int layer, int cameraX=0, int cameraY=0);
        int GetWidth();
        int GetHeight();
        int GetDepth();
        bool Is(std::string type);
        bool Is(ComponentType type);
};

#endif