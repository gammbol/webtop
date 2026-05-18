#ifndef WEBTOP_H
#define WEBTOP_H

#include <iostream>
#include <unistd.h>

#include <cpu.hpp>
#include <swap.hpp>

class machine_stat {
public:
  cpu::cpu_stat cpu{};
  swap::swap_stat swap{};

  void show_usage();
};

#endif