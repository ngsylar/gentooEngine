#ifndef DIALOGUE_BOX_H
#define DIALOGUE_BOX_H

#include "Component.h"
#include "GentooEngine.h"

class DialogueBox : public Component {
    private:
        std::string currentLine;
        std::vector<std::string> Lines;
        Text* dialogue;
        int counter;
        bool looped;
    public:
        DialogueBox(GameObject& associated, bool looped=true);
        ~DialogueBox();
        void Start();
        void Update(float dt);
        void LateUpdate(float dt);
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