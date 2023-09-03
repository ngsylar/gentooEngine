#include "DialogueBox.h"

bool DialogueBox::dialogueOngoing = false;

DialogueBox::DialogueBox(GameObject& associated, std::string dialogueFile, bool looped, bool followCamera)
: Component(associated) {
    dialogueText = nullptr;
    this->dialogueFile = dialogueFile;
    type = ComponentType::_DialogueBox;
    counter = 0;
    this->followCamera = followCamera;
    this->looped = looped;
    dialogueOngoing = true;
}

DialogueBox::~DialogueBox(){}

#define DIALOGUEFONT "assets/font/Minecraftia-Regular.ttf"
#define DIALOGUEFONTSIZE 8
//Max characters/line = 36, max lines = 3

#define DIALOGUEBALLOON "assets/img/text/Balloon.png"

void DialogueBox::Start(){
    Sprite* balloon = new Sprite(associated,DIALOGUEBALLOON);
    TextParser::Parse(dialogueFile);
    dialogueLines = TextParser::dialogue;
    TextParser::Clear();
    dialogueText = new Text(associated, dialogueLines[counter], DIALOGUEFONT,
        DIALOGUEFONTSIZE, Text::Style::BLENDED, Color("#865C45").ColorSDL());
    
    CameraFollower* balloonfollow = new CameraFollower(associated);
    balloonfollow->offset = Vec2(96, 32);
    associated.AddComponent(balloonfollow);
    
    dialogueText->SetOffset(Vec2(8,8));
    associated.AddComponent(balloon);
    associated.AddComponent(dialogueText);
    associated.box.SetSize(balloon->GetWidth(), balloon->GetHeight());
    dialogueText->SetWrap(247);//size in pixels
    dialogueText->SetAlignment(Text::Align::CENTER); 
    dialogueOngoing = true;
}

void DialogueBox::Update(float dt){
    InputManager& input = InputManager::GetInstance(); 
    if(input.KeyPress(SDLK_a)) {
        counter++;
        if(counter > (int)dialogueLines.size()-1) {
            if(!looped) {
                associated.RemoveComponent(dialogueText);
                return;
            }
            counter = 0;
        }
        dialogueText->SetText(dialogueLines[counter]);
    }
}

void DialogueBox::Render(){
    if(counter > (int)dialogueLines.size()-1 and !looped) {
        associated.RequestDelete();
        dialogueOngoing = false; //Yeah, on render. It is to avoid a glitch.
    }
}

bool DialogueBox::Is(ComponentType type){
    return (this->type & type);
}

//##############################

std::vector<std::string> TextParser::dialogue;

void TextParser::Parse (std::string file) {
    std::ifstream textFile(file);
    if(!textFile.is_open()) {
        SDL_Log("Unable to open file: %s", file.c_str());
        return;
    }
    std::string entry;
    std::getline(textFile, entry, '~');
    int total = stoi(entry);
    dialogue.reserve(total);
    for(int i = 0; i<total; i++) {
        std::getline(textFile, entry, '~');
        dialogue.push_back(entry);
    }
    textFile.close();
}

void TextParser::Clear() {
    dialogue.clear();
}