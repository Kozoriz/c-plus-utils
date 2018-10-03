#include "utils/containers/converters.h"

utils::Vector<utils::String> utils::SplitCSV(const utils::String &str, const utils::String &delim)
{
  utils::Vector<String> splited;
  Int pos = 0;
  Int pos2 = 0;
  while((pos2 = str.find(delim, pos)) != String::npos)
  {
    splited.push_back(str.substr(pos, pos2 - pos));
    pos = pos2 + 1;
  }
  splited.push_back(str.substr(pos));
  return splited;
}
