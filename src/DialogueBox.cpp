#include "DialogueBox.h"


DialogueBox::DialogueBox(GameObject& associated, bool looped)
: Component(associated) {
    dialogue = nullptr;
    type = ComponentType::_DialogueBox;
    counter = 0;
    this->looped = looped;
}

DialogueBox::~DialogueBox(){}

#define DIALOGUEFONT "assets/font/jupiter-crash-brk-regular.ttf"
//Max characters/line = 36, max lines = 3

#define DIALOGUEBALLOON "assets/img/BaseBalloon.png"
#define DIALOGUEFILE "assets/txt/test.txt"

void DialogueBox::Start(){
    Sprite* balloon = new Sprite(associated,DIALOGUEBALLOON);
    CameraFollower* balloonfollow = new CameraFollower(associated);
    TextParser::Parse(DIALOGUEFILE);
    dialogue = new Text(associated, TextParser::dialogue[counter], DIALOGUEFONT, 20, Text::Style::BLENDED, Color("#ffffff").ColorSDL());
    balloonfollow->offset = Vec2(96, 32);
    dialogue->SetOffset(Vec2(8,8));
    associated.AddComponent(balloon);
    associated.AddComponent(balloonfollow);
    associated.AddComponent(dialogue);
    associated.box.SetSize(balloon->GetWidth(), balloon->GetHeight());
    dialogue->SetWrap(247);//size in pixels
    dialogue->SetAlignment(Text::Align::CENTER); 
}

void DialogueBox::Update(float dt){
    InputManager& input = InputManager::GetInstance(); 
    if(input.KeyPress(SDLK_z)) {
        counter++;
        if(counter > (int)TextParser::dialogue.size()-1) {
            if(!looped) {
                associated.RequestDelete();
                return;
            }
            counter = 0;
        }
        dialogue->SetText(TextParser::dialogue[counter]);
    }
}

void DialogueBox::LateUpdate(float dt){
    
}

bool DialogueBox::Is(ComponentType type){
    return (this->type & type);
}

//##############################

std::vector<std::string> TextParser::dialogue;

void TextParser::Parse (std::string file) {
    std::ifstream textFile(file);
    if(!textFile.is_open())
    {
        SDL_Log("Unable to open file: %s", file.c_str());
        return;
    }
    std::string entry;
    std::getline(textFile, entry, '~');
    int total = stoi(entry);
    dialogue.reserve(total);

    for(int i = 0; i<total; i++)//Reads file to vector of vectors
    {
        std::getline(textFile, entry, '~');
        dialogue.push_back(entry);
    }
    
    textFile.close();
}

void TextParser::Clear() {
    dialogue.clear();
}