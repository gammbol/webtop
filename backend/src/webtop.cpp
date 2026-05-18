#include <webtop.hpp>

void machine_stat::show_usage() {
  cpu.get_cores_info();
  sleep(3);
  cpu.get_cores_info();

  std::cout << "========= STAT =========" << std::endl;
  std::cout << "name: " <<  cpu.name << std::endl;
  std::cout << "hz: " << cpu.hz / 1000 << "GHz" << std::endl;
  std::cout << "cores: " << (int)cpu.cores_num << std::endl;
  std::cout << "swap used: " << (swap.isUsed ? "yes" : "no") << std::endl;
  if (swap.isUsed) std::cout << "swap size: " << (double)swap.size / 1048576 << "gb" << std::endl;
  std::cout << "========================" << std::endl;
  
  std::cout << "\n";

  std::cout << "========= USAGE =========" << std::endl;
  for (uint8_t core = 0; core < cpu.cores_num; ++core) {
    std::cout << "cpu" << (int)core << ": " << cpu.cores[core].calculate_usage() << "%";
    std::cout << std::endl;
  }
  std::cout << "=========================" << std::endl;
}

int main() {
  std::cout << "Hello world!" << std::endl;

  machine_stat stat{};

  stat.show_usage();

  return 0;
}

