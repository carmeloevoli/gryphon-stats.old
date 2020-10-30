// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#include "Utilities.h"

#include <fstream>
#include <string>

namespace STATS {

int count_lines(const std::string& filename) {
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(filename.c_str());
    while (std::getline(myfile, line)) ++number_of_lines;
    return number_of_lines;
}

bool file_exists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

}  // namespace STATS
