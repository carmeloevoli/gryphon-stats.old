// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#include "TFitElectrons.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include "Utilities.h"

namespace STATS {

double TFitElectrons::run(double E_min, double E_max) {
    double min_chi2 = 1e10;
    for (double eps = 1.; eps <= 40.; eps *= 1.001) {
        electrons->computeMedian(secondary, pwn_efficiency, eps);
        auto chi2 = compute_chi2(E_min, E_max);
        if (chi2 < min_chi2) {
            min_chi2 = chi2;
            efficiency = eps;
        }
    }
    std::cout << "efficiency = " << efficiency << " chi2 = " << min_chi2 << "\n";
    return efficiency;
}

void TFitElectrons::buildSecondary(const std::string& filename) {
    secondary = std::make_shared<TSecondary>(filename);
}

void TFitElectrons::buildChi2(const std::string& filename) {
    AMS02 = std::make_shared<TData>(filename);
}

void TFitElectrons::buildElectrons(const std::string& filename_pwn,
                                   const std::string& filename_snr) {
    electrons = std::make_shared<TElectrons>(filename_pwn, filename_snr);
}

void TFitElectrons::print(const std::string& filename) {
    electrons->computeMedian(secondary, pwn_efficiency, efficiency);
    electrons->computeSdev(secondary, pwn_efficiency, efficiency);
    std::cout << "Saving results on file " << filename << " ...\n";
    std::ofstream outfile(filename.c_str());
    if (outfile.is_open()) {
        outfile << std::scientific << std::setprecision(4);
        outfile << "# E [GeV] - electrons []\n";
        for (size_t i = 0; i < secondary->getSize(); ++i) {
            outfile << secondary->getE(i) << "\t";
            outfile << secondary->getElectrons(i) << "\t";
            outfile << secondary->getPositrons(i) << "\t";
            outfile << electrons->getMedian(i) << "\t";
            outfile << electrons->getSdev(i) << "\t";
            outfile << electrons->getMedianSNR(i, efficiency) << "\t";
            outfile << electrons->getMedianPWN(i, pwn_efficiency) << "\t";
            outfile << electrons->getMedianFraction(i, secondary, pwn_efficiency, efficiency)
                    << "\t";
            outfile << electrons->getMedianFractionNoPulsar(i, secondary, efficiency) << "\t";
            outfile << electrons->getSdevFraction(i, secondary, pwn_efficiency, efficiency) << "\t";
            outfile << electrons->getMedianAll(i, secondary, pwn_efficiency, efficiency) << "\t";
            outfile << electrons->getSdevAll(i, secondary, pwn_efficiency, efficiency) << "\t";
            outfile << "\n";
        }
        outfile.close();
    }
}
double TFitElectrons::compute_chi2(const double& E_min, const double& E_max) const {
    double chi2 = 0.0;
    size_t ndata = 0;
    for (auto it = AMS02->data.begin(); it != AMS02->data.end(); it++) {
        if (it->E > E_min && it->E < E_max) {
            double flux = electrons->getFlux(it->E);
            double delta_chi2 = pow2(flux - it->F);
            delta_chi2 /= (flux < it->F) ? pow2(it->F_err_low) : pow2(it->F_err_high);
            chi2 += delta_chi2;
            ndata++;
        }
    }
    return chi2 / static_cast<double>(ndata);
}

}  // namespace STATS
