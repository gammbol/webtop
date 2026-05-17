#ifndef WEBTOP_STRING_HELPER_H
#define WEBTOP_STRING_HELPER_H

#include <algorithm>
#include <cctype>
#include <string>

inline std::string trim(const std::string& str) {
    auto start = std::find_if(
        str.begin(),
        str.end(),
        [](unsigned char ch) {
            return !std::isspace(ch);
        }
    );

    auto end = std::find_if(
        str.rbegin(),
        str.rend(),
        [](unsigned char ch) {
            return !std::isspace(ch);
        }
    ).base();

    if (start >= end) return "";

    return std::string(start, end);
}

#endif