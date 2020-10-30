// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#include "TData.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Utilities.h"

const int max_num_of_char_in_a_line = 512;

namespace STATS {

TData::TData(const std::string& filename) {
    if (file_exists(filename)) {
        read_datafile(filename);
    } else {
        throw std::runtime_error("data file does not exists.");
    }
}

void TData::read_datafile(const std::string& filename) {
    std::cout << "#Reading data from " << filename << "... ";
    std::ifstream file_to_read(filename.c_str());
    if (file_to_read.is_open()) {
        file_to_read.ignore(max_num_of_char_in_a_line, '\n');
        double values[4];
        while (!file_to_read.eof()) {
            file_to_read >> values[0] >> values[1] >> values[2] >> values[3];
            data_point point;
            point.E = values[0];
            point.F = values[1];
            point.F_err_low = values[2];
            point.F_err_high = values[3];
            if (file_to_read.good()) data.push_back(point);
        }
    }
    std::cout << " with size : " << data.size() << "\n";
    file_to_read.close();
}

}  // namespace STATS
