#include "errorHelper.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// General Error message
void logsSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}

void logsIMGError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << IMG_GetError() << std::endl;
}

void logsTTFError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << TTF_GetError() << std::endl;
}
