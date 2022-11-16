#ifndef RESOURCES_H
#define RESOURCES_H

#include <memory>
#include <unordered_map>
#include <string>

#define INCLUDE_SDL
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_TTF
#include "SDL_include.h"

class Resources {
    private:
        static std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> imageTable;
        static std::unordered_map<std::string, std::shared_ptr<Mix_Music>> musicTable;
        static std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>> soundTable;
        static std::unordered_map<std::string, std::shared_ptr<TTF_Font>> fontTable;
    
    public:
        static std::shared_ptr<SDL_Texture> GetImage(std::string file);
        static std::shared_ptr<Mix_Music> GetMusic(std::string file);
        static std::shared_ptr<Mix_Chunk> GetSound(std::string file);
        static std::shared_ptr<TTF_Font> GetFont(std::string file, int size);
        static void ClearRemainingImages();
        static void ClearRemainingMusics();
        static void ClearRemainingSounds();
        static void ClearRemainingFonts();
        static void ClearRemaining();
        static void ClearAll();
};

#endif