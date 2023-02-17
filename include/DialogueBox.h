#ifndef DIALOGUE_BOX_H
#define DIALOGUE_BOX_H

#include "Component.h"
#include "GentooEngine.h"

class DialogueBox : public Component {
    private:
        std::string dialogueFile;
        std::vector<std::string> dialogueLines;
        Text* dialogueText;
        bool followCamera;
        int counter;
        bool looped;
    public:
        static bool dialogueOngoing;
        DialogueBox(GameObject& associated, std::string dialogueFile, bool looped = false, bool followCamera=true);
        ~DialogueBox();
        void Start();
        void Update(float dt);
        void Render();
        bool Is(ComponentType type);

};


class TextParser {
    private:

    public:
        static std::vector<std::string> dialogue;
        static void Parse(std::string file);
        static void Clear();

};


#endif// DIALOGUE_BOX_H