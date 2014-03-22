#ifndef ERRORHELPER_HPP
#define ERRORHELPER_HPP

#include <iostream>

void logsSDLError(std::ostream &os, const std::string &msg);
void logsIMGError(std::ostream &os, const std::string &msg);
void logsTTFError(std::ostream &os, const std::string &msg);

#endif
