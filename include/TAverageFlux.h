// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#ifndef LIBS_STATS_INCLUDE_TAVERAGEFLUX_H_
#define LIBS_STATS_INCLUDE_TAVERAGEFLUX_H_

#include <string>
#include <vector>

namespace STATS {

class TAverageFlux {
   protected:
    std::string initFilename;
    int nLines = 0;
    int nModels = 0;
    std::vector<double> E;
    std::vector<double> f_p;

   public:
    TAverageFlux(const std::string& _initFilename, int _nModels);

    virtual ~TAverageFlux() {}

    void read_energy();

    void check_file_0();

    void load_model();

    double computeMedian(const size_t& i);

    double computeMean(const size_t& i);

    double computeSdev(const size_t& i);

    void print(const std::string& filename);
};

}  // namespace STATS

#endif  // LIBS_STATS_INCLUDE_TAVERAGEFLUX_H_
