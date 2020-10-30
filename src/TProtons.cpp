// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#include "TProtons.h"

#include <gsl/gsl_errno.h>
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

namespace STATS {

TProtons::TProtons(const std::string& _initFilename) : initFilename(_initFilename) {
    check_file_0();
    read_energy();
    count_number_of_models();
    load_model();
}

void TProtons::check_file_0() {
    std::string filename_0;

    filename_0 = initFilename + "_0.txt";
    if (file_exists(filename_0)) {
        nLines = count_lines(filename_0) - 1;
        std::cout << "#Read file 0 with number of lines: " << nLines << "\n";
    } else {
        throw std::runtime_error("file 0 must exists.");
    }
}

void TProtons::read_energy() {
    std::string filename_0 = initFilename + "_0.txt";
    std::fstream file_to_read(filename_0.c_str());

    for (int i = 0; i < 1; ++i) file_to_read.ignore(max_num_of_char_in_a_line, '\n');

    double E_i, flux_i, temp_i;
    while (!file_to_read.eof()) {
        file_to_read >> E_i >> flux_i >> temp_i;
        if (!file_to_read.eof()) {
            E.push_back(E_i);
        }
    }

    if (E.size() != nLines) throw std::runtime_error("incorrect size in read_energy.");

    std::cout << "#Min and max energy: " << E.front() << " - " << E.back() << "\n";
}

void TProtons::count_number_of_models() {
    std::string filename = initFilename + "_0.txt";
    while (file_exists(filename)) {
        nModels++;
        if (nLines != count_lines(filename) - 1)
            throw std::runtime_error("file with wrong number of lines.");
        filename = initFilename + "_" + std::to_string(nModels) + ".txt";
    }
    std::cout << "#Read number of models: " << nModels << "\n";
}

void TProtons::load_model() {
    for (size_t i = 0; i < nModels; ++i) {
        std::string filename = initFilename + "_" + std::to_string(i) + ".txt";
        std::fstream file_to_read(filename.c_str());

        for (int i = 0; i < 1; ++i) file_to_read.ignore(max_num_of_char_in_a_line, '\n');

        double E_i, flux_i, temp_i;
        for (size_t j = 0; j < nLines; ++j) {
            file_to_read >> E_i >> flux_i >> temp_i;
            if (E_i != E.at(j))
                throw std::runtime_error("the energy vector must be the same for all files.");
            f_pr.push_back(flux_i);
        }
    }

    assert(f_pr.size() == nLines * nModels);
    std::cout << "#Data loaded on an array of size: " << f_pr.size() << "\n";
}

double TProtons::computeMedian(double efficiency, size_t j) {
    auto data = new double[nModels];
    double medianFlux = 0;
    {
        for (size_t i = 0; i < nModels; ++i) {
            data[i] = efficiency * f_pr.at(i * (nLines) + j);
        }
        medianFlux = gsl_stats_median(data, 1, nModels);
    }
    delete[] data;
    return medianFlux;
}

double TProtons::computeMean(double efficiency, size_t j) {
    auto data = new double[nModels];
    double meanFlux = 0;
    {
        for (size_t i = 0; i < nModels; ++i) {
            data[i] = efficiency * f_pr.at(i * (nLines) + j);
        }
        meanFlux = gsl_stats_mean(data, 1, nModels);
    }
    delete[] data;
    return meanFlux;
}

double TProtons::computeSdev(double efficiency, size_t j) {
    auto data = new double[nModels];
    double sdFlux = 0;
    {
        for (size_t i = 0; i < nModels; ++i) {
            data[i] = efficiency * f_pr.at(i * (nLines) + j);
        }
        sdFlux = gsl_stats_sd(data, 1, nModels);
    }
    delete[] data;
    return sdFlux;
}

double TProtons::computeMad(double efficiency, size_t j) {
    auto data = new double[nModels];
    double madFlux = 0;
    {
        for (size_t i = 0; i < nModels; ++i) {
            data[i] = efficiency * f_pr.at(i * (nLines) + j);
        }

        double* work = new double[nModels];
        madFlux = gsl_stats_mad(data, 1, nModels, work);
        delete[] work;
    }
    delete[] data;
    return madFlux;
}

}  // namespace STATS
