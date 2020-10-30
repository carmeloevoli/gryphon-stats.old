// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#ifndef LIBS_STATS_INCLUDE_TELECTRONS_H_
#define LIBS_STATS_INCLUDE_TELECTRONS_H_

#include <memory>
#include <string>
#include <vector>

#include "TSecondary.h"

namespace STATS {

class TElectrons {
   protected:
    std::string initFilename_pwn;
    std::string initFilename_snr;
    int nLines = 0;
    int nModels = 0;
    std::vector<double> E;
    std::vector<double> f_e_snr;
    std::vector<double> f_e_pwn;
    std::vector<double> medianflux;
    std::vector<double> sdflux;
    double efficiency = 0;
    double efficiency_pwn = 0;

   public:
    TElectrons(const std::string& _initFilename_pwn, const std::string& _initFilename_snr);

    virtual ~TElectrons() = default;

    double getMedian(const size_t& i) const { return medianflux.at(i); }

    double getSdev(const size_t& i) const { return sdflux.at(i); }

    double getMedianSNR(const size_t& j, double _efficiency) const;

    double getMedianPWN(const size_t& j, double _efficiency) const;

    double getMedianFraction(const size_t& j, const std::shared_ptr<TSecondary> secondary,
                             double _pwn_efficiency, double _efficiency) const;

    double getMedianFractionNoPulsar(const size_t& j, const std::shared_ptr<TSecondary> secondary,
                                     double _efficiency) const;

    double getSdevFraction(const size_t& j, const std::shared_ptr<TSecondary> secondary,
                           double _pwn_efficiency, double _efficiency) const;

    double getMedianAll(const size_t& j, const std::shared_ptr<TSecondary> secondary,
                        double _pwn_efficiency, double _efficiency) const;

    double getSdevAll(const size_t& j, const std::shared_ptr<TSecondary> secondary,
                      double _pwn_efficiency, double _efficiency) const;

    void check_file_0();

    void read_energy();

    void count_number_of_models();

    void load_model();

    void computeMedian(const std::shared_ptr<TSecondary> secondary, double pwn_efficiency,
                       double efficiency);

    void computeSdev(const std::shared_ptr<TSecondary> secondary, double pwn_efficiency,
                     double efficiency);

    double getFlux(const double& _E);
};

}  // namespace STATS
#endif  // LIBS_STATS_INCLUDE_TELECTRONS_H_
