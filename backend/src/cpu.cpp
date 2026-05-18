#include <cpu.hpp>
#include <iostream>

cpu::cpu_stat::cpu_stat() {
  if (!get_cpu_info()) throw std::runtime_error("ERROR");

  cores.resize(cores_num);
}

bool cpu::cpu_stat::isFilled() {
  if (cores_num == 0) return false;
  if (hz == 0.0) return false;
  if (name.empty()) return false;
  return true;
}

bool cpu::cpu_stat::get_cpu_info() {
  std::ifstream cpuinfo(cpu::CPUINFO);
  if (!cpuinfo.is_open()) return false;

  std::string line;
  while (std::getline(cpuinfo, line)) {
    if (isFilled()) break;

    std::istringstream ss(line);
    std::string param, val;

    if (!std::getline(ss, param, ':')) { std::cerr << line << std::endl; return false; }
    
    param = trim(param);

    if (param == "model name") {
      std::getline(ss, val, ':');
      name = val;
    }
    else if (param == "siblings") {
      std::getline(ss, val, ':');
      try {
        cores_num = std::stoi(trim(val));
      } catch (...) {
        return false;
      }
    }
    else if (param == "cpu MHz") {
      std::getline(ss, val, ':');
      try {
        hz = std::stod(val);
      } catch (...) {
        return false;
      }
    }
  }

  if (!isFilled()) return false;

  return true;
}

bool cpu::cpu_stat::update() {
  std::ifstream coresinfo(cpu::STAT);
  if (!coresinfo.is_open()) return false;

  std::string line;
  for (uint8_t i = 0; i < cores_num; ++i) {
    if (!std::getline(coresinfo, line)) return false;
    std::istringstream ss(line);
    
    cores[i].last = cores[i].current;

    cpu::core_states_time cst{};

    if (!(
      ss >> 
      cores[i].name >> 
      cst.user >> 
      cst.nice >> 
      cst.system >>
      cst.idle >>
      cst.iowait >>
      cst.irq >>
      cst.softirq >>
      cst.steal >>
      cst.guest >>
      cst.guest_nice 
    )) return false;

    cores[i].current = std::move(cst);
  }

  return true;
}

double cpu::core_stat::calculate_usage() {
  auto total_time_last = last.total_time();
  auto work_time_last = last.work_time();

  auto total_time_current = current.total_time();
  auto work_time_current = current.work_time();

  auto work_over_period = work_time_current - work_time_last;
  auto total_over_period = total_time_current - total_time_last;

  return (double)work_over_period / total_over_period * 100;
}

uint32_t cpu::core_states_time::total_time() {
  return user + nice + system + idle + iowait + irq + softirq + steal + guest + 
  guest_nice;
}

uint32_t cpu::core_states_time::work_time() {
  return user + nice + system;
}