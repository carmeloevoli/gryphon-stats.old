// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#ifndef LIBS_STATS_INCLUDE_UTILITIES_H_
#define LIBS_STATS_INCLUDE_UTILITIES_H_

#include <string>

namespace STATS {

const int max_num_of_char_in_a_line = 512;

bool file_exists(const std::string& name);

int count_lines(const std::string& filename);

inline double pow2(const double& x) { return x * x; }

}  // namespace STATS

#endif  // LIBS_STATS_INCLUDE_UTILITIES_H_
