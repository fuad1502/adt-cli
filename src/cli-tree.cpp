#include <sstream>

int columns(const std::string& s)
{
  int i = 0;
  while(s[i] != '\0' && s[i] != '\n') i++;
  return i;
}

std::string addColumn(const std::string& leftColumn, const std::string& s, const std::string& rightColumn)
{
  std::stringstream ss(s);
  std::string line, res;
  std::getline(ss, line);
  while(line.length() != 0) {
    res += leftColumn;
    res += line;
    res += rightColumn;
    res += "\n";
    std::getline(ss, line);
  }
  return res;
}
