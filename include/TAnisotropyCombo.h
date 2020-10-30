// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#ifndef LIBS_STATS_INCLUDE_TANISOTROPYCOMBO_H_
#define LIBS_STATS_INCLUDE_TANISOTROPYCOMBO_H_

#include <string>
#include <utility>
#include <vector>

namespace STATS {

struct Vector3 {
    double x;
    double y;
    double z;
};

class TAnisotropyCombo {
   protected:
    std::string initFilename_pwn;
    std::string initFilename_snr;
    int nLines = 0;
    int nModels = 0;
    std::vector<double> E;
    std::vector<double> f_e_snr;
    std::vector<double> f_e_pwn;
    std::vector<Vector3> delta_snr;
    std::vector<Vector3> delta_pwn;
    double pwn_efficiency = 0;
    double snr_efficiency = 0;

   public:
    TAnisotropyCombo(const std::string& initFilename_pwn, const std::string& initFilename_snr);

    ~TAnisotropyCombo() = default;

    void check_file_0();

    void read_energy();

    void count_number_of_models();

    void load_model();

    void set_efficiency(const double& pwn_efficiency, const double& snr_efficiency);

    std::pair<double, double> getMedian(const size_t& j) const;

    void print(const std::string& filename) const;
};

}  // namespace STATS

#endif  // LIBS_STATS_INCLUDE_TANISOTROPYCOMBO_H_
