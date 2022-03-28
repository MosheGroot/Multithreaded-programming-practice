#pragma once

#include <string>


extern char * const *environ;


namespace PipeApp::Env
{

  std::string   getFromPath(const std::string& command);

} //!namespace PipeApp::Env