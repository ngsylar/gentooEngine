#include "GentooEngine.h"

TileMap::TileMap (
    GameObject& associated, TileSet* tileSet, std::string file, float parallaxFactor
): Component(associated) {

    this->tileSet = tileSet;
    Load(file);
    this->parallaxFactor = parallaxFactor;
    associated.label = "TileMap";
}

TileMap::~TileMap () {
    delete tileSet;
    tileMatrix.clear();
}

void TileMap::Load (std::string fileName) {
    std::ifstream file;
    std::string tile;
    char delimiter;

    file.open(fileName);
    if (not file) {
        SDL_Log("Unable to open file: %s", fileName.c_str());
        return;
    }

    file >> mapWidth >> delimiter >> mapHeight >> delimiter >> mapDepth >> delimiter;
    while (std::getline(file, tile, delimiter)) {
        tileMatrix.push_back(atoi(tile.c_str())-1);
    }

    file.close();
}

void TileMap::SetTileSet (TileSet* tileSet) {
    this->tileSet = tileSet;
}

int& TileMap::At (int x, int y, int z) {
    int tile;
    tile = (z*mapWidth*mapHeight)+(y*mapWidth)+x;
    return tileMatrix[tile];
}

void TileMap::Update (float dt) {}

void TileMap::Render () {
    for (int z=0; z<mapDepth; z++) {
        RenderLayer(z, (int)Camera::pos.x, (int)Camera::pos.y);
    }
}

void TileMap::RenderLayer (int layer, int cameraX, int cameraY) {
    int tile;

    for (int y=0; y < mapHeight; y++)
        for (int x=0; x < mapWidth; x++) {
            tile = At(x, y, layer);
            tileSet->RenderTile(
                (unsigned)tile,
                (float)(associated.box.x+x)*tileSet->GetTileWidth()-(1.0f+parallaxFactor)*cameraX,
                (float)(associated.box.y+y)*tileSet->GetTileHeight()-(1.0f+parallaxFactor)*cameraY
            );
        }
}

int TileMap::GetWidth () {
    return mapWidth;
}

int TileMap::GetHeight () {
    return mapHeight;
}

int TileMap::GetDepth () {
    return mapDepth;
}

bool TileMap::Is (std::string type) {
    return (type == "TileMap");
}