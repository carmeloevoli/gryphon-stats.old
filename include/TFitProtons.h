// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#ifndef LIBS_STATS_INCLUDE_TFITPROTONS_H_
#define LIBS_STATS_INCLUDE_TFITPROTONS_H_

#include <memory>
#include <string>

#include "TData.h"
#include "TProtons.h"

namespace STATS {

class TFitProtons {
   public:
    TFitProtons() {}

    virtual ~TFitProtons() {}

    void buildProtons(const std::string& initFilename);

    void buildChi2(const std::string& filename);

    double interpolateFlux(double E, double eps) const;

    double run(double E_min, double E_max) const;

    void print(const std::string& filename, double efficiency) const;

   protected:
    std::shared_ptr<TProtons> protons;
    std::shared_ptr<TData> AMS02;

   protected:
    double compute_chi2(double E_min, double E_max, double eps) const;
};

}  // namespace STATS

#endif  // LIBS_STATS_INCLUDE_TFITPROTONS_H_
