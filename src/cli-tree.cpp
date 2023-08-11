#include <sstream>

int columns(const std::string& s)
{
  int i = 0;
  while(s[i] != '\0' && s[i] != '\n') i++;
  return i;
}

std::string concatenateLines(const std::string& data, const std::string& left, const std::string& right)
{
  // Calculate required paddings
  int datasStringLength = data.length();
  int leftStringColumns = columns(left);
  int rightStringColumns = columns(right);
  int childColumn = std::max(leftStringColumns, rightStringColumns);
  int nSlashes = childColumn / 2;
  int rightPadLeft = nSlashes - (leftStringColumns / 2);
  int leftPadLeft = (childColumn + 1) / 2 - (leftStringColumns + 1) / 2;
  int rightPadRight = nSlashes - (rightStringColumns / 2);
  int leftPadRight = (childColumn + 1) / 2 - (rightStringColumns + 1) / 2;
  
  std::stringstream leftstream(left);
  std::stringstream rightstream(right);
  std::string leftLine;
  std::string rightLine;
  std::string s;

  std::getline(leftstream, leftLine);
  std::getline(rightstream, rightLine);
  while(leftLine.length() != 0 || rightLine.length() != 0)
  {
    s += std::string(leftPadLeft, ' ');
    if(leftLine.length() == 0)
      s += std::string(leftStringColumns, ' ');
    else
      s += leftLine;
    s += std::string(rightPadLeft, ' ');
    s += std::string(datasStringLength, ' ');
    s += std::string(leftPadRight, ' ');
    if(rightLine.length() == 0)
      s += std::string(rightStringColumns, ' ');
    else
      s += rightLine;
    s += std::string(rightPadRight, ' ');
    s += '\n';
    std::getline(leftstream, leftLine);
    std::getline(rightstream, rightLine);
  }

  return s;
}
