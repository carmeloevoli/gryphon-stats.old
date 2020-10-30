// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#ifndef LIBS_STATS_INCLUDE_TDATA_H_
#define LIBS_STATS_INCLUDE_TDATA_H_

#include <string>
#include <vector>

namespace STATS {

struct data_point {
    double E;
    double F;
    double F_err_low;
    double F_err_high;
};

class TData {
   public:
    explicit TData(const std::string& filename);
    virtual ~TData() = default;

    std::vector<data_point> data;

   protected:
    void read_datafile(const std::string& filename);
};

}  // namespace STATS

#endif  // LIBS_STATS_INCLUDE_TDATA_H_
