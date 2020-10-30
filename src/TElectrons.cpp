// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#include "TElectrons.h"

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

TElectrons::TElectrons(const std::string& _initFilename_pwn, const std::string& _initFilename_snr)
    : initFilename_pwn(_initFilename_pwn), initFilename_snr(_initFilename_snr) {
    check_file_0();
    read_energy();
    count_number_of_models();
    load_model();
}

double TElectrons::getMedianSNR(const size_t& j, double _efficiency) const {
    auto data = new double[nModels];
    for (size_t i = 0; i < nModels; ++i) {
        data[i] = _efficiency * f_e_snr.at(i * (nLines) + j);
    }
    auto value = gsl_stats_median(data, 1, nModels);
    delete[] data;
    return value;
}

double TElectrons::getMedianPWN(const size_t& j, double _efficiency) const {
    auto data = new double[nModels];
    for (size_t i = 0; i < nModels; ++i) {
        data[i] = _efficiency * f_e_pwn.at(i * (nLines) + j);
    }
    auto value = gsl_stats_median(data, 1, nModels);
    delete[] data;
    return value;
}

double TElectrons::getMedianFraction(const size_t& j, const std::shared_ptr<TSecondary> secondary,
                                     double _pwn_efficiency, double _efficiency) const {
    auto data = new double[nModels];
    for (size_t i = 0; i < nModels; ++i) {
        double electrons = _efficiency * f_e_snr.at(i * (nLines) + j);
        electrons += _pwn_efficiency * f_e_pwn.at(i * (nLines) + j);
        electrons += secondary->getElectrons(j);

        double positrons = _pwn_efficiency * f_e_pwn.at(i * (nLines) + j);
        positrons += secondary->getPositrons(j);

        data[i] = positrons / (electrons + positrons);
    }
    auto value = gsl_stats_median(data, 1, nModels);
    delete[] data;
    return value;
}

double TElectrons::getMedianFractionNoPulsar(const size_t& j,
                                             const std::shared_ptr<TSecondary> secondary,
                                             double _efficiency) const {
    auto data = new double[nModels];
    for (size_t i = 0; i < nModels; ++i) {
        double electrons = _efficiency * f_e_snr.at(i * (nLines) + j);
        electrons += secondary->getElectrons(j);
        double positrons = secondary->getPositrons(j);
        data[i] = positrons / (electrons + positrons);
    }
    auto value = gsl_stats_median(data, 1, nModels);
    delete[] data;
    return value;
}

double TElectrons::getSdevFraction(const size_t& j, const std::shared_ptr<TSecondary> secondary,
                                   double _pwn_efficiency, double _efficiency) const {
    auto data = new double[nModels];
    for (size_t i = 0; i < nModels; ++i) {
        double electrons = _efficiency * f_e_snr.at(i * (nLines) + j);
        electrons += _pwn_efficiency * f_e_pwn.at(i * (nLines) + j);
        electrons += secondary->getElectrons(j);

        double positrons = _pwn_efficiency * f_e_pwn.at(i * (nLines) + j);
        positrons += secondary->getPositrons(j);

        data[i] = positrons / (electrons + positrons);
    }
#ifdef MAD4VAR
    double* work = new double[nModels];
    auto value = gsl_stats_mad(data, 1, nModels, work);
    delete[] work;
#else
    auto value = gsl_stats_sd(data, 1, nModels);
#endif
    delete[] data;
    return value;
}

double TElectrons::getMedianAll(const size_t& j, const std::shared_ptr<TSecondary> secondary,
                                double _pwn_efficiency, double _efficiency) const {
    auto data = new double[nModels];
    for (size_t i = 0; i < nModels; ++i) {
        double leptons = _efficiency * f_e_snr.at(i * (nLines) + j);
        leptons += 2. * _pwn_efficiency * f_e_pwn.at(i * (nLines) + j);
        leptons += secondary->getElectrons(j);
        leptons += secondary->getPositrons(j);
        data[i] = leptons;
    }
    auto value = gsl_stats_median(data, 1, nModels);
    delete[] data;
    return value;
}

double TElectrons::getSdevAll(const size_t& j, const std::shared_ptr<TSecondary> secondary,
                              double _pwn_efficiency, double _efficiency) const {
    auto data = new double[nModels];
    for (size_t i = 0; i < nModels; ++i) {
        double leptons = _efficiency * f_e_snr.at(i * (nLines) + j);
        leptons += 2. * _pwn_efficiency * f_e_pwn.at(i * (nLines) + j);
        leptons += secondary->getElectrons(j);
        leptons += secondary->getPositrons(j);
        data[i] = leptons;
    }

#ifdef MAD4VAR
    double* work = new double[nModels];
    auto value = gsl_stats_mad(data, 1, nModels, work);
    delete[] work;
#else
    auto value = gsl_stats_sd(data, 1, nModels);
#endif
    delete[] data;
    return value;
}

void TElectrons::check_file_0() {
    std::string filename_0;

    filename_0 = initFilename_pwn + "_0.txt";
    if (file_exists(filename_0)) {
        nLines = count_lines(filename_0) - 1;
        std::cout << "#Read file 0 with number of lines: " << nLines << "\n";
    } else {
        throw std::runtime_error("file 0 must exists.");
    }
    filename_0 = initFilename_snr + "_0.txt";
    if (file_exists(filename_0)) {
        nLines = count_lines(filename_0) - 1;
        std::cout << "#Read file 0 with number of lines: " << nLines << "\n";
    } else {
        throw std::runtime_error("file 0 must exists.");
    }
}

void TElectrons::read_energy() {
    std::string filename_0 = initFilename_snr + "_0.txt";
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

void TElectrons::count_number_of_models() {
    std::string filename_snr = initFilename_snr + "_0.txt";
    std::string filename_pwn = initFilename_pwn + "_0.txt";
    while (file_exists(filename_snr) && file_exists(filename_pwn)) {
        nModels++;
        if (nLines != count_lines(filename_snr) - 1)
            throw std::runtime_error("file with wrong number of lines.");
        if (nLines != count_lines(filename_pwn) - 1)
            throw std::runtime_error("file with wrong number of lines.");
        filename_snr = initFilename_snr + "_" + std::to_string(nModels) + ".txt";
        filename_pwn = initFilename_pwn + "_" + std::to_string(nModels) + ".txt";
    }
    std::cout << "#Read number of models: " << nModels << "\n";
}

void TElectrons::load_model() {
    for (size_t i = 0; i < nModels; ++i) {
        std::string filename_snr = initFilename_snr + "_" + std::to_string(i) + ".txt";
        std::fstream file_to_read(filename_snr.c_str());

        for (int i = 0; i < 1; ++i) file_to_read.ignore(max_num_of_char_in_a_line, '\n');

        double E_i, flux_i;
        for (size_t j = 0; j < nLines; ++j) {
            file_to_read >> E_i >> flux_i;
            if (E_i != E.at(j))
                throw std::runtime_error("the energy vector must be the same for all files.");
            f_e_snr.push_back(flux_i);
        }
    }

    for (size_t i = 0; i < nModels; ++i) {
        std::string filename_pwn = initFilename_pwn + "_" + std::to_string(i) + ".txt";
        std::fstream file_to_read(filename_pwn.c_str());

        for (int i = 0; i < 1; ++i) file_to_read.ignore(max_num_of_char_in_a_line, '\n');

        double E_i, flux_i;
        for (size_t j = 0; j < nLines; ++j) {
            file_to_read >> E_i >> flux_i;
            if (E_i != E.at(j))
                throw std::runtime_error("the energy vector must be the same for all files.");
            f_e_pwn.push_back(flux_i);
        }
    }

    assert(f_e_snr.size() == nLines * nModels);
    assert(f_e_pwn.size() == nLines * nModels);
    std::cout << "#Data loaded on an array of size: " << f_e_snr.size() << "\n";
}

void TElectrons::computeMedian(const std::shared_ptr<TSecondary> secondary, double pwn_efficiency,
                               double efficiency) {
    medianflux.clear();
    auto data = new double[nModels];
    for (size_t j = 0; j < nLines; ++j) {
        for (size_t i = 0; i < nModels; ++i) {
            data[i] = efficiency * f_e_snr.at(i * (nLines) + j);
            data[i] += pwn_efficiency * f_e_pwn.at(i * (nLines) + j);
            data[i] += secondary->getPositrons(j);
        }

        medianflux.push_back(gsl_stats_median(data, 1, nModels));
        // std::cout << gsl_stats_mad(data, 1, nModels, work) << "   ";
    }
    delete[] data;
    assert(medianflux.size() == nLines);
}

void TElectrons::computeSdev(const std::shared_ptr<TSecondary> secondary, double pwn_efficiency,
                             double efficiency) {
    sdflux.clear();
    auto data = new double[nModels];
    for (size_t j = 0; j < nLines; ++j) {
        // double data_nosec[m_nModels];

        for (size_t i = 0; i < nModels; ++i) {
            data[i] = efficiency * f_e_snr.at(i * (nLines) + j);
            data[i] += pwn_efficiency * f_e_pwn.at(i * (nLines) + j);
            data[i] += secondary->getPositrons(j);
        }

#ifdef MAD4VAR
        double* work = new double[nModels];
        auto value = gsl_stats_mad(data, 1, nModels, work);
        delete[] work;
#else
        auto value = gsl_stats_sd(data, 1, nModels);
#endif
        sdflux.push_back(value);
    }
    delete[] data;
    assert(sdflux.size() == nLines);
}

double TElectrons::getFlux(const double& _E) {
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
