// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#ifndef LIBS_STATS_INCLUDE_TFITELECTRONS_H_
#define LIBS_STATS_INCLUDE_TFITELECTRONS_H_

#include <memory>
#include <string>

#include "TData.h"
#include "TElectrons.h"
#include "TSecondary.h"

namespace STATS {

class TFitElectrons {
   public:
    explicit TFitElectrons(double _pwn_efficiency) : pwn_efficiency(_pwn_efficiency) {}

    virtual ~TFitElectrons() {}

    double run(double E_min, double E_max);

    void buildSecondary(const std::string& filename);

    void buildChi2(const std::string& filename);

    void buildElectrons(const std::string& filename_pwn, const std::string& filename_snr);

    void print(const std::string& filename);

   protected:
    double pwn_efficiency = 0;
    double efficiency = 0;
    std::shared_ptr<TSecondary> secondary;
    std::shared_ptr<TData> AMS02;
    std::shared_ptr<TElectrons> electrons;

    double compute_chi2(const double& E_min, const double& E_max) const;
};

}  // namespace STATS

#endif  // LIBS_STATS_INCLUDE_TFITELECTRONS_H_
