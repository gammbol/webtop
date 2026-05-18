#ifndef WEBTOP_SWAP_H
#define WEBTOP_SWAP_H

#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

namespace swap {

inline constexpr const char *SWAPFILE = "/proc/swaps";

struct swap_stat {
  bool isUsed{false};

  std::string partition{};
  uint32_t size{};
  uint32_t used{};

  inline bool updateStats() {
    std::ifstream swapfile(SWAPFILE);
    if (!swapfile.is_open()) return false;

    std::string line;

    if (!std::getline(swapfile, line)) return false;
    if (!std::getline(swapfile, line)) return false;

    std::istringstream ss(line);
    std::string type;

    if (!(ss >> partition >> type >> size >> used)) return false;

    return true;
  }

  inline swap_stat() {
    if (updateStats()) isUsed = true;
  }
};

}

#endif