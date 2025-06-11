#include <fstream>
#include <iostream>
int main(int argc, char **argv) {
  std::ifstream in(argc > 1 ? argv[1] : "-");
  std::string line;
  int count = 0;
  while (std::getline(in, line)) {
    if (!line.empty() && line[0] != '@')
      ++count;
  }
  std::cout << count << " alignments\n";
  return 0;
}