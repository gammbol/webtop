#ifndef WEBTOP_H
#define WEBTOP_H

#include <cstdint>
#include <format>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <unistd.h>

#include <cpu.hpp>
#include <memory.hpp>
#include <procs.hpp>

namespace webtop {

inline constexpr const char *UPTIME_FILE = "/proc/uptime";

class machine_stat {
  uint32_t uptime{};

  cpu::cpu_stat cpu{};
  memory::memory mem{};
  process::procs procs{};

public:
  inline void show_usage() {
    cpu.update();
    procs.update();
    sleep(3);
    cpu.update();
    mem.update();
    procs.update();

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

    std::cout << "\n";

    std::cout << "============================== PROCESSES ==============================\n";

std::cout
    << std::left
    << std::setw(8)  << "PID"
    << std::setw(8)  << "PPID"
    << std::setw(30) << "NAME"
    << std::setw(10) << "CPU%"
    << std::setw(12) << "MEM"
    << std::setw(12) << "UTIME"
    << std::setw(12) << "STIME"
    << '\n';

std::cout << std::string(92, '-') << '\n';

for (const auto& proc : procs.proc_list) {
    std::cout
        << std::left
        << std::setw(8)  << proc.pid
        << std::setw(8)  << proc.ppid
        << std::setw(30) << proc.name
        << std::setw(10) << std::fixed << std::setprecision(2)
                         << proc.cpu_usage
        << std::setw(12) << proc.memory_bytes
        << std::setw(12) << proc.utime
        << std::setw(12) << proc.stime
        << '\n';
}

std::cout << "========================================================================\n";
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