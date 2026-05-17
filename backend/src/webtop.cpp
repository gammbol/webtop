#include <webtop.hpp>

int main() {
  std::cout << "Hello world!" << std::endl;

  machine_stat stat{};

  stat.cpu.show_usage();

  return 0;
}

