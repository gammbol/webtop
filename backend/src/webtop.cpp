#include <webtop.hpp>

int main() {
  std::cout << "Hello world!" << std::endl;

  webtop::machine_stat stat{};

  stat.show_usage();

  return 0;
}

