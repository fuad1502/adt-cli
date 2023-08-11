#include <sstream>

int columns(const std::string& s)
{
  int i = 0;
  while(s[i] != '\0' && s[i] != '\n') i++;
  return i;
}
