#ifndef WEBTOP_CPU_H
#define WEBTOP_CPU_H

#include <cstdint>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <string_helper.hpp>

namespace cpu {

inline constexpr const char *CPUINFO = "/proc/cpuinfo";
inline constexpr const char *STAT = "/proc/stat";

struct core_states_time {
  uint32_t user{};
  uint32_t nice{};
  uint32_t system{};
  uint32_t idle{};
  uint32_t iowait{};
  uint32_t irq{};
  uint32_t softirq{};
  uint32_t steal{};
  uint32_t guest{};
  uint32_t guest_nice{};

  uint32_t total_time();
  uint32_t work_time();
};

struct core_stat {
  std::string name{};

  core_states_time current{};
  core_states_time last{};

  double calculate_usage();
};

struct cpu_stat {
  uint8_t cores_num{};

  double hz{};

  std::string name;

  std::vector<core_stat> cores;

  double calculate_total_usage();

  bool isFilled();

  bool get_cpu_info();
  bool get_cores_info();

  void show_usage();

  cpu_stat();
};

} // namespace cpu

#endif