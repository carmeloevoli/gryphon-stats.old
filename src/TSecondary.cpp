// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#include "TSecondary.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Utilities.h"

namespace STATS {

TSecondary::TSecondary(const std::string& filename) {
    if (file_exists(filename)) {
        size = count_lines(filename) - 1;
        load_data(filename);
        std::cout << "#Secondary data loaded on an array of size: " << size;
        std::cout << " with E_min: " << getEmin() << " and E_max: " << getEmax() << "\n";
    } else {
        throw std::runtime_error("secondary file does not exists.");
    }
}

void TSecondary::load_data(const std::string& filename) {
    std::fstream file_to_read(filename.c_str());
    for (int i = 0; i < 1; ++i) file_to_read.ignore(max_num_of_char_in_a_line, '\n');
    double E_i, electrons_i, positrons_i;
    for (size_t j = 0; j < size; ++j) {
        file_to_read >> E_i >> electrons_i >> positrons_i;
        E.push_back(E_i);
        electrons.push_back(electrons_i);
        positrons.push_back(positrons_i);
    }
    assert(electrons.size() == size);
    assert(positrons.size() == size);
}

}  // namespace STATS
