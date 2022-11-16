#include "GentooEngine.h"

TileSet::TileSet (
    GameObject& associated, std::string file,
    int tileWidth, int tileHeight
): tileSet(associated, file) {
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    rows = tileSet.GetHeight() / tileHeight;
    columns = tileSet.GetWidth() / tileWidth;
}

void TileSet::RenderTile (unsigned index, float x, float y) {
    int clipStartX, clipStartY;

    if (index < (unsigned)(rows*columns)) {
        clipStartX = (index%columns)*tileWidth;
        clipStartY = (index/columns)*tileHeight;
        tileSet.SetClip(
            clipStartX, clipStartY,
            tileWidth, tileHeight
        );
        tileSet.Render(round(x), round(y));
    }
}

int TileSet::GetTileWidth () {
    return tileWidth;
}

int TileSet::GetTileHeight () {
    return tileHeight;
}
