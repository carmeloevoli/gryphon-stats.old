// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#include "TPositrons.h"

#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_statistics.h>

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "Utilities.h"

#define MAD4VAR

namespace STATS {

TPositrons::TPositrons(const std::string& _initFilename) : initFilename(_initFilename) {
    check_file_0();
    read_energy();
    count_number_of_models();
    load_model();
}

void TPositrons::read_energy() {
    std::string filename_0 = initFilename + "_0.txt";
    std::fstream file_to_read(filename_0.c_str());

    for (int i = 0; i < 1; ++i) file_to_read.ignore(max_num_of_char_in_a_line, '\n');

    double E_i, flux_i;
    while (!file_to_read.eof()) {
        file_to_read >> E_i >> flux_i;
        if (!file_to_read.eof()) {
            E.push_back(E_i);
        }
    }

    if (E.size() != nLines) throw std::runtime_error("incorrect size in read_energy.");

    std::cout << "#Min and max energy: " << E.front() << " - " << E.back() << "\n";
}

void TPositrons::check_file_0() {
    std::string filename_0 = initFilename + "_0.txt";
    if (file_exists(filename_0)) {
        nLines = count_lines(filename_0) - 1;
        std::cout << "#Read file 0 with number of lines: " << nLines << "\n";
    } else {
        throw std::runtime_error("file 0 must exists.");
    }
}

void TPositrons::count_number_of_models() {
    std::string filename = initFilename + "_0.txt";
    while (file_exists(filename)) {
        nModels++;
        // std::cout << filename << "\n";
        if (nLines != count_lines(filename) - 1)
            throw std::runtime_error("file " + filename + " with wrong number of lines.");
        filename = initFilename + "_" + std::to_string(nModels) + ".txt";
    }
    std::cout << "#Read number of models: " << nModels << "\n";
}

void TPositrons::load_model() {
    for (size_t i = 0; i < nModels; ++i) {
        std::string filename = initFilename + "_" + std::to_string(i) + ".txt";
        std::fstream file_to_read(filename.c_str());

        for (int i = 0; i < 1; ++i) file_to_read.ignore(max_num_of_char_in_a_line, '\n');

        double E_i, flux_i;
        for (size_t j = 0; j < nLines; ++j) {
            file_to_read >> E_i >> flux_i;
            if (E_i != E.at(j))
                throw std::runtime_error("the energy vector must be the same for all files.");
            f_p.push_back(flux_i);
        }
    }
    assert(f_p.size() == nLines * nModels);
    std::cout << "#Data loaded on an array of size: " << f_p.size() << "\n";
}

void TPositrons::computeMedian(const std::shared_ptr<TSecondary> secondary, double efficiency) {
    medianflux.clear();
    auto data = new double[nModels];
    for (size_t j = 0; j < nLines; ++j) {
        for (size_t i = 0; i < nModels; ++i) {
            data[i] = efficiency * f_p.at(i * (nLines) + j) + secondary->getPositrons(j);
        }

        medianflux.push_back(gsl_stats_median(data, 1, nModels));
    }
    delete[] data;
    assert(medianflux.size() == nLines);
}

void TPositrons::computeSdev(const std::shared_ptr<TSecondary> secondary, double efficiency) {
    sdflux.clear();
    auto data = new double[nModels];
    for (size_t j = 0; j < nLines; ++j) {
        for (size_t i = 0; i < nModels; ++i) {
            data[i] = efficiency * f_p.at(i * (nLines) + j) + secondary->getPositrons(j);
        }
#ifdef MAD4VAR
        double* work = new double[nModels];
        sdflux.push_back(gsl_stats_mad(data, 1, nModels, work));
        delete[] work;
#else
        sdflux.push_back(gsl_stats_sd(data, 1, nModels));
#endif
    }
    delete[] data;
    assert(sdflux.size() == nLines);
}

void TPositrons::computeMedianNoSec(double efficiency) {
    medianflux_nosec.clear();
    auto data = new double[nModels];
    for (size_t j = 0; j < nLines; ++j) {
        for (size_t i = 0; i < nModels; ++i) {
            data[i] = efficiency * f_p.at(i * (nLines) + j);
        }

        medianflux_nosec.push_back(gsl_stats_median(data, 1, nModels));
    }
    delete[] data;
    assert(medianflux_nosec.size() == nLines);
}

double TPositrons::getFlux(const double& _E) {
    double x[nLines], y[nLines];
    for (int i = 0; i < nLines; ++i) {
        x[i] = std::log(E.at(i));
        y[i] = std::log(medianflux.at(i));
    }

    gsl_interp_accel* acc = gsl_interp_accel_alloc();
    gsl_spline* spline = gsl_spline_alloc(gsl_interp_cspline, nLines);
    gsl_spline_init(spline, x, y, nLines);
    auto value = gsl_spline_eval(spline, std::log(_E), acc);
    gsl_spline_free(spline);
    gsl_interp_accel_free(acc);

    return std::exp(value);
}

}  // namespace STATS
