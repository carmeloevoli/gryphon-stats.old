// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#include "TFitProtons.h"

#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Utilities.h"

namespace STATS {

void TFitProtons::buildProtons(const std::string& initFilename) {
    protons = std::make_shared<TProtons>(initFilename);
}

void TFitProtons::buildChi2(const std::string& filename) {
    AMS02 = std::make_shared<TData>(filename);
}

double TFitProtons::run(double E_min, double E_max) const {
    double min_chi2 = 1e10;
    double best_efficiency = -1;
    for (double eps = 0.01; eps <= 2.0; eps *= 1.001) {
        auto chi2 = compute_chi2(E_min, E_max, eps);
        if (chi2 < min_chi2) {
            min_chi2 = chi2;
            best_efficiency = eps;
        }
    }
    std::cout << "efficiency = " << best_efficiency << " chi2 = " << min_chi2 << "\n";
    return best_efficiency;
}

void TFitProtons::print(const std::string& filename, double efficiency) const {
    std::cout << "Saving results on file " << filename << " ...\n";
    std::ofstream outfile(filename.c_str());
    if (outfile.is_open()) {
        outfile << std::scientific << std::setprecision(4);
        outfile << "# E [GeV] - protons []\n";
        for (size_t i = 0; i < protons->getSize(); ++i) {
            outfile << protons->getE(i) << "\t";
            outfile << protons->computeMedian(efficiency, i) << "\t";
            outfile << protons->computeMad(efficiency, i) << "\t";
            outfile << protons->computeMean(efficiency, i) << "\t";
            outfile << protons->computeSdev(efficiency, i) << "\t";
            outfile << "\n";
        }
        outfile.close();
    }
}

double TFitProtons::interpolateFlux(double E, double eps) const {
    const auto size = protons->getSize();
    double logx[size], logy[size];
    for (int i = 0; i < size; ++i) {
        logx[i] = std::log(protons->getE(i));
        logy[i] = std::log(protons->computeMedian(eps, i));
    }

    gsl_interp_accel* acc = gsl_interp_accel_alloc();
    gsl_spline* spline = gsl_spline_alloc(gsl_interp_cspline, size);
    gsl_spline_init(spline, logx, logy, size);
    auto value = gsl_spline_eval(spline, std::log(E), acc);
    gsl_spline_free(spline);
    gsl_interp_accel_free(acc);

    return std::exp(value);
}

double TFitProtons::compute_chi2(double E_min, double E_max, double eps) const {
    double chi2 = 0.0;
    size_t ndata = 0;
    for (auto it = AMS02->data.begin(); it != AMS02->data.end(); it++) {
        if (it->E > E_min && it->E < E_max) {
            double flux = interpolateFlux(it->E, eps);
            double delta_chi2 = pow2(flux - it->F);
            delta_chi2 /= (flux < it->F) ? pow2(it->F_err_low) : pow2(it->F_err_high);
            chi2 += delta_chi2;
            ndata++;
        }
    }
    return chi2 / static_cast<double>(ndata);
}

}  // namespace STATS
