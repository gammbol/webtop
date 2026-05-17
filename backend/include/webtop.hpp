#ifndef WEBTOP_H
#define WEBTOP_H

#include <iostream>

#include <cpu.hpp>

class machine_stat {
public:
  cpu::cpu_stat cpu{};
};

#endif