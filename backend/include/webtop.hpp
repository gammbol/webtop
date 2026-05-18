#ifndef WEBTOP_H
#define WEBTOP_H

#include <cstdint>
#include <format>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <unistd.h>

#include <cpu.hpp>
#include <memory.hpp>

namespace webtop {

inline constexpr const char *UPTIME_FILE = "/proc/uptime";

class machine_stat {
  uint32_t uptime{};

  cpu::cpu_stat cpu{};
  memory::memory mem{};

public:
  inline void show_usage() {
    cpu.update();
    sleep(3);
    cpu.update();
    mem.update();

    std::cout << "========= STAT =========" << std::endl;
    std::cout << "name: " <<  cpu.name << std::endl;
    std::cout << "hz: " << cpu.hz / 1000 << "GHz" << std::endl;
    std::cout << "cores: " << (int)cpu.cores_num << std::endl;
    std::cout << "swap used: " 
              << (
                mem.swap_total ? 
                std::format(
                  "{}gb total ({}gb avaliable)", 
                  (double)mem.swap_total / memory::KBTOGB, 
                  (double)mem.swap_available / memory::KBTOGB
                ) : 
                "no"
              ) 
              << std::endl;
    std::cout << "uptime: " << uptime_to_string() << std::endl;
    std::cout << "========================" << std::endl;
    
    std::cout << "\n";

    std::cout << "========= USAGE =========" << std::endl;
    std::cout << "RAM: " 
              << (double)mem.ram_total / memory::KBTOGB
              << "gb total ("
              << (double)mem.ram_available / memory::KBTOGB
              << "gb available)"
              << std::endl;
    for (uint8_t core = 0; core < cpu.cores_num; ++core) {
      std::cout << "cpu" << (int)core << ": " << cpu.cores[core].calculate_usage() << "%";
      std::cout << std::endl;
    }
    std::cout << "=========================" << std::endl;
  }

  void update_uptime() {
    std::ifstream uptime_file(UPTIME_FILE);
    if (!uptime_file.is_open()) 
      throw std::runtime_error("DEVICE INITIALIZATION: opening file");

    std::string line;
    if (!std::getline(uptime_file, line)) 
      throw std::runtime_error("DEVICE INITIALIZATION: reading file");

    std::istringstream ss(line);

    if (!(ss >> uptime))
      throw std::runtime_error("DEVICE INITIALIZATION: processing data");
  }

  inline std::string uptime_to_string() {
    uint8_t hours{}, minutes{}, seconds{};
    
    seconds = uptime % 60;
    minutes = (uptime / 60) % 60;
    hours = uptime / 3600;

    return std::string{
      (hours ? (std::to_string(hours) + "h ") : "") +
      ((hours || minutes) ? (std::to_string(minutes) + "min ") : "") +
      ((hours || minutes || seconds) ? (std::to_string(minutes) + "sec") : "0sec")
    };

  }

  inline machine_stat() {
    update_uptime();
  }
};

}

#endif