// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#include "TAnisotropyCombo.h"

#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_statistics.h>

#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "Utilities.h"

namespace STATS {

TAnisotropyCombo::TAnisotropyCombo(const std::string& _initFilename_pwn,
                                   const std::string& _initFilename_snr)
    : initFilename_pwn(_initFilename_pwn), initFilename_snr(_initFilename_snr) {
    check_file_0();
    read_energy();
    count_number_of_models();
    load_model();
}

void TAnisotropyCombo::check_file_0() {
    std::string filename_0;

    filename_0 = initFilename_pwn + "_0.anisotropy";
    if (file_exists(filename_0)) {
        nLines = count_lines(filename_0) - 1;
        std::cout << "#Read file 0 with number of lines: " << nLines << "\n";
    } else {
        throw std::runtime_error("file 0 must exists: " + filename_0);
    }
    filename_0 = initFilename_snr + "_0.anisotropy";
    if (file_exists(filename_0)) {
        nLines = count_lines(filename_0) - 1;
        std::cout << "#Read file 0 with number of lines: " << nLines << "\n";
    } else {
        throw std::runtime_error("file 0 must exists: " + filename_0);
    }
}

void TAnisotropyCombo::read_energy() {
    std::string filename_0 = initFilename_snr + "_0.anisotropy";
    std::fstream file_to_read(filename_0.c_str());

    for (int i = 0; i < 1; ++i) file_to_read.ignore(max_num_of_char_in_a_line, '\n');

    double E_i, temp[4];
    while (!file_to_read.eof()) {
        file_to_read >> E_i >> temp[0] >> temp[1] >> temp[2] >> temp[3];
        if (!file_to_read.eof()) {
            E.push_back(E_i);
        }
    }

    if (E.size() != nLines) throw std::runtime_error("incorrect size in read_energy.");
    std::cout << "#Min and max energy: " << E.front() << " - " << E.back() << "\n";
}

void TAnisotropyCombo::count_number_of_models() {
    std::string filename_snr = initFilename_snr + "_0.anisotropy";
    std::string filename_pwn = initFilename_pwn + "_0.anisotropy";
    while (file_exists(filename_snr) && file_exists(filename_pwn)) {
        nModels++;
        if (nLines != count_lines(filename_snr) - 1)
            throw std::runtime_error("file with wrong number of lines.");
        if (nLines != count_lines(filename_pwn) - 1)
            throw std::runtime_error("file with wrong number of lines.");
        filename_snr = initFilename_snr + "_" + std::to_string(nModels) + ".anisotropy";
        filename_pwn = initFilename_pwn + "_" + std::to_string(nModels) + ".anisotropy";
    }
    std::cout << "#Read number of models: " << nModels << "\n";
}

void TAnisotropyCombo::load_model() {
    for (size_t i = 0; i < nModels; ++i) {
        std::string filename_snr = initFilename_snr + "_" + std::to_string(i) + ".anisotropy";
        std::fstream file_to_read(filename_snr.c_str());

        for (int i = 0; i < 1; ++i) file_to_read.ignore(max_num_of_char_in_a_line, '\n');

        double E_i, deltax_i, deltay_i, deltaz_i, flux_i;
        for (size_t j = 0; j < nLines; ++j) {
            file_to_read >> E_i >> deltax_i >> deltay_i >> deltaz_i >> flux_i;
            if (E_i != E.at(j))
                throw std::runtime_error("the energy vector must be the same for all files.");
            f_e_snr.push_back(flux_i);
            Vector3 delta = {deltax_i, deltay_i, deltaz_i};
            delta_snr.push_back(delta);
        }
    }

    for (size_t i = 0; i < nModels; ++i) {
        std::string filename_pwn = initFilename_pwn + "_" + std::to_string(i) + ".anisotropy";
        std::fstream file_to_read(filename_pwn.c_str());

        for (int i = 0; i < 1; ++i) file_to_read.ignore(max_num_of_char_in_a_line, '\n');

        double E_i, deltax_i, deltay_i, deltaz_i, flux_i;
        for (size_t j = 0; j < nLines; ++j) {
            file_to_read >> E_i >> deltax_i >> deltay_i >> deltaz_i >> flux_i;
            if (E_i != E.at(j))
                throw std::runtime_error("the energy vector must be the same for all files.");
            f_e_pwn.push_back(flux_i);
            Vector3 delta = {deltax_i, deltay_i, deltaz_i};
            delta_pwn.push_back(delta);
        }
    }

    assert(f_e_snr.size() == nLines * nModels);
    assert(f_e_pwn.size() == nLines * nModels);
    std::cout << "#Data loaded on an array of size: " << f_e_snr.size() << "\n";
}

void TAnisotropyCombo::set_efficiency(const double& _pwn_efficiency,
                                      const double& _snr_efficiency) {
    pwn_efficiency = _pwn_efficiency;
    snr_efficiency = _snr_efficiency;
}

std::pair<double, double> TAnisotropyCombo::getMedian(const size_t& j) const {
    auto data = new double[nModels];
    for (size_t i = 0; i < nModels; ++i) {
        double n_SNR = snr_efficiency * f_e_snr.at(i * (nLines) + j);
        double n_PWN = 2. * pwn_efficiency * f_e_pwn.at(i * (nLines) + j);
        double n_sum = n_SNR + n_PWN;

        auto delta_SNR = delta_snr.at(i * (nLines) + j);
        auto delta_PWN = delta_pwn.at(i * (nLines) + j);

        auto delta_x = n_SNR / n_sum * delta_SNR.x + n_PWN / n_sum * delta_PWN.x;
        auto delta_y = n_SNR / n_sum * delta_SNR.y + n_PWN / n_sum * delta_PWN.y;
        auto delta_z = n_SNR / n_sum * delta_SNR.z + n_PWN / n_sum * delta_PWN.z;

        data[i] = std::sqrt(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
    }
    auto median = gsl_stats_median(data, 1, nModels);
    auto sd = gsl_stats_sd(data, 1, nModels);

    delete[] data;
    return {median, sd};
}

void TAnisotropyCombo::print(const std::string& filename) const {
    std::cout << "Saving results on file " << filename << " ...\n";
    std::ofstream outfile(filename.c_str());
    if (outfile.is_open()) {
        outfile << std::scientific << std::setprecision(4);
        outfile << "# E [GeV] - electrons []\n";
        for (size_t i = 0; i < E.size(); ++i) {
            outfile << E.at(i) << "\t";
            auto median = getMedian(i);
            outfile << median.first << "\t";
            outfile << median.second << "\t";
            outfile << "\n";
        }
        outfile.close();
    }
}

}  // namespace STATS
