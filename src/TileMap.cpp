#include "GentooEngine.h"

TileMap::TileMap (
    GameObject& associated, TileSet* tileSet, std::string file, float parallaxFactor, bool loadAsRaw
): Component(associated) {

    this->tileSet = tileSet;
    associated.box.SetSize(tileSet->GetTileWidth(), tileSet->GetTileHeight());
    loadAsRaw ? LoadRaw(file) : Load(file);
    this->parallaxFactor = parallaxFactor;
    associated.label = "TileMap";
    type = ComponentType::_TileMap;
}

TileMap::~TileMap () {
    delete tileSet;
    _tileMatrix.clear();
}

void TileMap::Load (std::string fileName) {
    std::ifstream TextMap(fileName+".txt");
    if(!TextMap.is_open())
    {
        SDL_Log("Unable to open file: %s", fileName.c_str());
        return;
    }
    std::string Entry;//Will collect the inputs for interpretation and storage
    
    //gets initial info about the map file
    std::getline(TextMap, Entry, ',');
    mapWidth = stoi(Entry);
    std::getline(TextMap, Entry, ',');
    mapHeight = stoi(Entry);

    std::vector<std::vector<int>> Map(mapHeight, std::vector<int>(mapWidth));
    for(int i = 0; i<mapHeight; i++)//Reads file to vector of vectors
    {
        for(int j = 0; j<mapWidth; j++)
        {
            std::getline(TextMap, Entry, ',');
            Map[i][j] = (stoi(Entry)+1);
        }
    }
    TextMap.close();
    _tileMatrix = Map;
}

void TileMap::LoadRaw(std::string fileName) {
    std::ifstream TextMap(fileName+".txt");//todo change to .csv
    if(!TextMap.is_open())
    {
        SDL_Log("Unable to open file: %s", fileName.c_str());
        return;
    }
    std::string Entry;//Will collect the inputs for interpretation and storage
    std::vector<std::vector<int>> Map;
    while(!TextMap.eof()) {
        std::getline(TextMap, Entry, '\n');
        std::stringstream Line(Entry);
        std::vector<int> LineVec;
        for (int i; Line >> i;) {
            LineVec.push_back(i+1);    
            if (Line.peek() == ',' or Line.peek() == '\n') {
                Line.ignore();
            }
        }
        if (TextMap.eof()) {
            break;
        }
        Map.emplace_back(LineVec);
    }
    mapWidth = (int) Map[0].size();
    mapHeight = (int) Map.size();
    TextMap.close();
    _tileMatrix = Map;
}

void TileMap::LoadCollision(std::string fileName){
    
    int HashResult = Hash(_tileMatrix);
    std::vector<RectInt> Block = LoadCoord(fileName+"-rect.txt");
    bool RectAbsent = Block.size()==0;

    if(HasMapChanged(HashResult, fileName+"-hash.txt") or RectAbsent) {
        std::ofstream Output(fileName+"-hash.txt");//Register new hash
        Output << HashResult<< ",\n";
        Output.close();

        std::vector<std::vector<int>> Map = _tileMatrix;
        BorderMarking(Map);
        FindX(Map);
        Block = MapToCoord(Map);
        CoordOptimizer(Block);
        DumpCoord(Block, fileName+"-rect.txt");
    }

    for(int i = 0; i < (int)Block.size(); i++) {
        GameObject* ColliderObj = new GameObject(LayerDistance::_ForeGround_VeryClose);
        Collider *TileCollider = new Collider(*ColliderObj);
        ColliderObj->AddComponent(TileCollider);
        ColliderObj->SignalTerrain();
        ColliderObj->box = Rect(Block[i].x*tileSet->GetTileWidth(), Block[i].y*tileSet->GetTileHeight(),
            Block[i].w*tileSet->GetTileWidth(), Block[i].h*tileSet->GetTileHeight());
        Game::GetInstance().GetCurrentState().AddObject(ColliderObj);
    }
}

void TileMap::SetTileSet (TileSet* tileSet) {
    this->tileSet = tileSet;
    associated.box.SetSize(tileSet->GetTileWidth(), tileSet->GetTileHeight());
}

void TileMap::Update (float dt) {}

void TileMap::Render () {
    if (not Game::GetInstance().GetCurrentState().Debugging())
        return;
    Vec2 Lower = Camera::pos;
    Vec2 Upper = Lower + Game::GetInstance().GetResolution();
    int Lx = floor(Lower.x)/tileSet->GetTileWidth()-4;//0;//
    int Hx = floor(Upper.x)/tileSet->GetTileWidth()+4;//mapWidth;//
    int Ly = floor(Lower.y)/tileSet->GetTileHeight()-4;//0;//
    int Hy = floor(Upper.y)/tileSet->GetTileHeight()+4;//mapHeight;//

    for (int y = std::max(Ly, 0) ; y < std::min(Hy, mapHeight); y++)
        for (int x = std::max(Lx, 0); x < std::min(Hx, mapWidth); x++) {
            tileSet->RenderTile(
                (unsigned)_tileMatrix[y][x]-1,
                (float)(associated.box.x+x)*tileSet->GetTileWidth()-(parallaxFactor*(int)Lower.x),
                (float)(associated.box.y+y)*tileSet->GetTileHeight()-(parallaxFactor*(int)Lower.y)
            );
        }
}

void TileMap::RenderLayer (int layer, int cameraX, int cameraY) {
    
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

bool TileMap::Is (ComponentType type) {
    return (type & this->type);
}

//__________________________________
// TileMap auto-collision generation/optimization code below.
// Abandon all hope before attempting to read it. Good luck.
//__________________________________

int TileMap::Hash (std::vector<std::vector<int>>& Map) {
    int Hash, x;
    for(int i = 0; i < (int)Map.size(); i++) {
        for(int j = 0; j < (int)Map[i].size(); j++) {
            x = Map[i][j];
            x = ((x >> 16) ^ x) * 0x45d9f3b;
            x = ((x >> 16) ^ x) * 0x45d9f3b;
            x = (x >> 16) ^ x;
            Hash ^= x + 0x9e3779b9 + (Hash << 6) + (Hash >> 2);
        }
    }
    return Hash;
}

bool TileMap::HasMapChanged (int HashResult, std::string HashPath) {
    std::ifstream HashFile(HashPath);
    if(HashFile) {
        std::string Entry;
        std::getline(HashFile, Entry,',');
        int x = stoi(Entry);
        if(x == HashResult) {
            return false;
        }
        return true;    
    }
    return true;
}

std::vector<RectInt> TileMap::LoadCoord (std::string File) {
    std::ifstream BoxCoords(File);
    if(!BoxCoords.is_open()){
        std::vector<RectInt> fail;
        return fail;
    }
    std::string Entry;
    int x;
    std::getline(BoxCoords, Entry, ',');
    x = stoi(Entry);
    std::vector<RectInt> Box(x);

    for(int i = 0; i<x; i++){
        std::getline(BoxCoords, Entry, ',');
        Box[i].x = (stoi(Entry));
        std::getline(BoxCoords, Entry, ',');
        Box[i].y = (stoi(Entry));
        std::getline(BoxCoords, Entry, ',');
        Box[i].w = (stoi(Entry));
        std::getline(BoxCoords, Entry, ',');
        Box[i].h = (stoi(Entry));
    }
    BoxCoords.close();
    return Box;
}

void TileMap::DumpCoord (const std::vector<RectInt>& Data, std::string File) {
    std::ofstream Output(File);
    Output << Data.size()<< ",\n";
    for(int i = 0; i<(int)Data.size(); i++) {
        Output << Data[i].x <<','<< Data[i].y <<','<< Data[i].w <<','<< Data[i].h <<",\n";
    }
    Output.close();
}

//Todo ignore blocks meant for half colliders
void TileMap::BorderMarking (std::vector<std::vector<int>> &Map) {
    int Index = 1;
    for(int i = 0; i < (int)Map.size(); i++) {
        for(int j = 0; j < (int)Map[i].size()-1; j++) {
            if(Map[i][j] > 0) {
                if((j > 0 and Map[i][j-1] == 0)
                or (j < (int)Map[i].size()-1 and Map[i][j+1] == 0)
                or (i > 0 and Map[i-1][j] == 0)
                or (i < (int)Map.size()-1 and Map[i+1][j] == 0)){
                    Map[i][j] = Index;
                    Index++;
                } else {
                    Map[i][j] = -1;
                }
            }
        }
        Index++;
    }
}

void TileMap::FindX (std::vector<std::vector<int>>& Map) {
    int Index = 1;
    for(int i = 0; i < (int)Map.size(); i++) {
        for(int j = 0; j < (int)Map[i].size()-1; j++) {
            if(Map[i][j] > 0){
                Map[i][j] = Index;
                if(Map[i][j+1] > 0){
                   Map[i][j+1] = Map[i][j];
                } else {
                    Index++;
                }
            }
        }
        Index++;
    }
}

std::vector<RectInt> TileMap::MapToCoord (std::vector<std::vector<int>>& Map) {
    std::vector<RectInt> Coords;
    for(int i = 0; i < (int)Map.size(); i++)
    {
        for(int j = 0; j < (int)Map[i].size()-1; j++){
            if(Map[i][j] > 0){
                RectInt Res{j,i,1,1};
                if(Map[i][j] == Map[i][j+1]){
                    while(j < (int)Map[i].size()-2 and Map[i][j] == Map[i][j+1]){
                        Res.w++;
                        j++;
                    }
                }
                Coords.emplace_back(Res);
            }
        }
    }
    return Coords;
}

void TileMap::CoordOptimizer (std::vector<RectInt>& Coords) {
    std::sort(Coords.begin(), Coords.end(), 
        [](RectInt& a, RectInt& b) {
            if(a.x == b.x) {
                return a.y < b.y;
            }
        return a.x < b.x;
        });

    for(int i = 0; i < (int)Coords.size(); i++) {
        if(Coords[i].x == Coords[i+1].x
        and Coords[i].w == Coords[i+1].w
        and Coords[i].y == Coords[i+1].y - Coords[i].h) {
            Coords[i].h++;
            Coords.erase(Coords.begin()+(i+1));
            i--;
        }
    }
}
//__________________________________