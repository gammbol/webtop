#ifndef WEBTOP_MEMORY_H
#define WEBTOP_MEMORY_H

#include <cstdint>
#include <fstream>
#include <sstream>

#include <string_helper.hpp>

namespace memory {

inline constexpr const uint32_t KBTOGB = 1048576;

inline constexpr const char *MEMINFO = "/proc/meminfo";

struct memory {
  uint32_t ram_total{};
  uint32_t ram_available{};

  uint32_t swap_total{};
  uint32_t swap_available{};

  inline void update() {
    std::ifstream meminfo(MEMINFO);
    if (!meminfo.is_open()) 
      throw std::runtime_error("MEMORY UPDATE: read error");

    std::string line;
    while (std::getline(meminfo, line)) {
      std::string token;
      std::istringstream ss(line);
      if (!std::getline(ss, token, ':'))
        throw std::runtime_error("MEMORY UPDATE: parsing error");
      
      if (string_helper::trim(token) == "MemTotal") {
        if (!std::getline(ss, token, ':'))
          throw std::runtime_error("MEMORY UPDATE: total ram tokenizing error");
        
        std::istringstream sstoken(token);
        if (!(sstoken >> ram_total))
          throw std::runtime_error("MEMORY UPDATE: total ram processing error");
      }
      else if (string_helper::trim(token) == "MemFree") {
        if (!std::getline(ss, token, ':'))
          throw std::runtime_error("MEMORY UPDATE: free ram tokenizing error");
        
        std::istringstream sstoken(token);
        if (!(sstoken >> ram_available))
          throw std::runtime_error("MEMORY UPDATE: free ram processing error");
      }
      else if (string_helper::trim(token) == "SwapTotal") {
        if (!std::getline(ss, token, ':'))
          throw std::runtime_error("MEMORY UPDATE: total swap tokenizing error");
        
        std::istringstream sstoken(token);
        if (!(sstoken >> swap_total))
          throw std::runtime_error("MEMORY UPDATE: total swap processing error");
      }
      else if (string_helper::trim(token) == "SwapFree") {
        if (!std::getline(ss, token, ':'))
          throw std::runtime_error("MEMORY UPDATE: available swap tokenizing error");
        
        std::istringstream sstoken(token);
        if (!(sstoken >> swap_available))
          throw std::runtime_error("MEMORY UPDATE: available swap processing error");
      }
    }
  }

  inline memory() { update(); }
};

}

#endif