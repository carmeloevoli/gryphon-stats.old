// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#ifndef LIBS_STATS_INCLUDE_TFITPOSITRONS_H_
#define LIBS_STATS_INCLUDE_TFITPOSITRONS_H_

#include <memory>
#include <string>

#include "TData.h"
#include "TPositrons.h"
#include "TSecondary.h"

namespace STATS {

class TFitPositrons {
   public:
    TFitPositrons() {}

    virtual ~TFitPositrons() {}

    void buildSecondary(const std::string& filename);

    void buildPositrons(const std::string& initFilename);

    void buildChi2(const std::string& filename);

    double run(double E_min, double E_max);

    void print(const std::string& filename) const;

   protected:
    double efficiency = 0;
    std::shared_ptr<TSecondary> secondary;
    std::shared_ptr<TPositrons> positrons;
    std::shared_ptr<TData> AMS02;

   protected:
    double compute_chi2(const double& E_min, const double& E_max) const;
};

}  // namespace STATS

#endif  // LIBS_STATS_INCLUDE_TFITPOSITRONS_H_
