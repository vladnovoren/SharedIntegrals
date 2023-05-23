#include <cstdlib>
#include <string>



int main() {
  for (int i = 0; i < 6; ++i) {
    std::string cmd = "./multi " + std::to_string(i);
    std::system(cmd.c_str());
  }
  return 0;
}
