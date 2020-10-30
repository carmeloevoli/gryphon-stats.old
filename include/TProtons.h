// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#ifndef LIBS_STATS_INCLUDE_TPROTONS_H_
#define LIBS_STATS_INCLUDE_TPROTONS_H_

#include <memory>
#include <string>
#include <vector>

namespace STATS {

class TProtons {
   protected:
    std::string initFilename;
    int nLines = 0;
    int nModels = 0;
    std::vector<double> E;
    std::vector<double> f_pr;
    double efficiency = 0;

   public:
    explicit TProtons(const std::string& _initFilename);

    virtual ~TProtons() = default;

    double getE(size_t i) const { return E.at(i); }

    int getSize() const { return E.size(); }

    void check_file_0();

    void read_energy();

    void count_number_of_models();

    void load_model();

    double computeMedian(double _efficiency, size_t i);

    double computeMean(double _efficiency, size_t i);

    double computeSdev(double _efficiency, size_t i);

    double computeMad(double _efficiency, size_t i);
};

}  // namespace STATS

#endif  // LIBS_STATS_INCLUDE_TPROTONS_H_
