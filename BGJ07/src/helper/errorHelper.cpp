#include <SDL.h>
#include "errorHelper.hpp"

// General Error message
void logsSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}

