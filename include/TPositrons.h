// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#ifndef LIBS_STATS_INCLUDE_TPOSITRONS_H_
#define LIBS_STATS_INCLUDE_TPOSITRONS_H_

#include <memory>
#include <string>
#include <vector>

#include "TSecondary.h"

namespace STATS {

class TPositrons {
   protected:
    std::string initFilename;
    int nLines = 0;
    int nModels = 0;
    std::vector<double> E;
    std::vector<double> f_p;
    std::vector<double> medianflux;
    std::vector<double> medianflux_nosec;
    std::vector<double> sdflux;
    double efficiency = 0;

   public:
    explicit TPositrons(const std::string& _initFilename);

    virtual ~TPositrons() {}

    double getMedian(const size_t& i) const { return medianflux.at(i); }

    double getMedianNosec(const size_t& i) const { return medianflux_nosec.at(i); }

    double getSdev(const size_t& i) const { return sdflux.at(i); }

    void read_energy();

    void check_file_0();

    void count_number_of_models();

    void load_model();

    void computeMedian(const std::shared_ptr<TSecondary> secondary, double efficiency);

    void computeSdev(const std::shared_ptr<TSecondary> secondary, double efficiency);

    void computeMedianNoSec(double efficiency);

    double getFlux(const double& _E);
};

}  // namespace STATS

#endif  // LIBS_STATS_INCLUDE_TPOSITRONS_H_
