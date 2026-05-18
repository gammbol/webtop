#ifndef WEBTOP_PROCS_H
#define WEBTOP_PROCS_H

#include <cstdint>
#include <filesystem>
#include <list>
#include <string>

namespace fs = std::filesystem;

namespace process {

static bool is_pid(const std::string& str) {
    return !str.empty() &&
           std::all_of(str.begin(), str.end(), ::isdigit);
}


struct proc_node {
    uint32_t pid{};
    uint32_t ppid{};

    std::string name{};

    uint64_t memory_bytes{};

    uint64_t utime{};
    uint64_t stime{};

    double cpu_usage{};
};

struct procs {
  std::list<proc_node> proc_list{};

  inline void update() {
    proc_list.clear();

    for (const auto& entry : fs::directory_iterator("/proc")) {
        if (!entry.is_directory()) continue;

        std::string dirname = entry.path().filename().string();

        if (!is_pid(dirname)) continue;

        process::proc_node node{};

        try {
            node.pid = std::stoul(dirname);
        } catch (...) {
            continue;
        }

        //
        // =========================
        // /proc/<pid>/stat
        // =========================
        //
        {
            std::ifstream stat_file(
                entry.path() / "stat"
            );

            if (!stat_file.is_open()) continue;

            std::string line;
            if (!std::getline(stat_file, line)) continue;

            //
            // format:
            // pid (comm) state ppid ...
            //

            auto lparen = line.find('(');
            auto rparen = line.rfind(')');

            if (lparen == std::string::npos ||
                rparen == std::string::npos)
                continue;

            node.name = line.substr(
                lparen + 1,
                rparen - lparen - 1
            );

            std::string after =
                line.substr(rparen + 2);

            std::istringstream ss(after);

            char state;

            uint64_t dummy;

            ss >> state;
            ss >> node.ppid;

            //
            // skip fields 5-13
            //
            for (int i = 0; i < 9; ++i)
                ss >> dummy;

            //
            // fields 14-15
            //
            ss >> node.utime;
            ss >> node.stime;
        }

        //
        // =========================
        // /proc/<pid>/status
        // =========================
        //
        {
            std::ifstream status_file(
                entry.path() / "status"
            );

            if (status_file.is_open()) {
                std::string line;

                while (std::getline(status_file, line)) {
                    if (line.starts_with("VmRSS:")) {
                        std::istringstream ss(line);

                        std::string key;
                        uint64_t kb;

                        ss >> key >> kb;

                        node.memory_bytes = kb * 1024;

                        break;
                    }
                }
            }
        }

        proc_list.push_back(std::move(node));
    }
  }
};

}

#endif