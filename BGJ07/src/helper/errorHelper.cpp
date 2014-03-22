#ifndef ERRORHELPER_CPP
#define ERRORHELPER_CPP

#include <iostream>
#include <SDL.h>

// General Error message
void logsSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}

#endif

