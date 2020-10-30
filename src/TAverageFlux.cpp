// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#include "TAverageFlux.h"

#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_statistics.h>

#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Utilities.h"

namespace STATS {

TAverageFlux::TAverageFlux(const std::string& _initFilename, int _nModels)
    : initFilename(_initFilename), nModels(_nModels) {
    check_file_0();
    read_energy();
    load_model();
}

void TAverageFlux::read_energy() {
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

void TAverageFlux::check_file_0() {
    std::string filename_0 = initFilename + "_0.txt";
    if (file_exists(filename_0)) {
        nLines = count_lines(filename_0) - 1;
        std::cout << "#Read file 0 with number of lines: " << nLines << "\n";
    } else {
        throw std::runtime_error("file 0 must exists.");
    }
}

void TAverageFlux::load_model() {
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

double TAverageFlux::computeMedian(const size_t& j) {
    auto data = new double[nModels];

    for (size_t i = 0; i < nModels; ++i) {
        data[i] = f_p.at(i * (nLines) + j);
    }

    auto value = gsl_stats_median(data, 1, nModels);

    delete[] data;

    return value;
}

double TAverageFlux::computeMean(const size_t& j) {
    auto data = new double[nModels];

    for (size_t i = 0; i < nModels; ++i) {
        data[i] = f_p.at(i * (nLines) + j);
    }

    auto value = gsl_stats_mean(data, 1, nModels);

    delete[] data;

    return value;
}

#define MAD4VAR
double TAverageFlux::computeSdev(const size_t& j) {
    auto data = new double[nModels];

    for (size_t i = 0; i < nModels; ++i) {
        data[i] = f_p.at(i * (nLines) + j);
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

void TAverageFlux::print(const std::string& filename) {
    std::cout << "Saving results on file " << filename << " ...\n";
    std::ofstream outfile(filename.c_str());
    if (outfile.is_open()) {
        outfile << std::scientific << std::setprecision(4);
        outfile << "# E [GeV] - electrons []\n";
        for (size_t i = 0; i < E.size(); ++i) {
            outfile << E.at(i) << "\t";
            outfile << computeMean(i) << "\t";
            outfile << computeMedian(i) << "\t";
            outfile << computeSdev(i) << "\t";
            outfile << "\n";
        }
        outfile.close();
    }
}
}  // namespace STATS
