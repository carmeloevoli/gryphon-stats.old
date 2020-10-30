// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#ifndef LIBS_STATS_INCLUDE_TSECONDARY_H_
#define LIBS_STATS_INCLUDE_TSECONDARY_H_

#include <string>
#include <vector>

namespace STATS {

class TSecondary {
   protected:
    int size = 0;
    std::vector<double> electrons;
    std::vector<double> positrons;
    std::vector<double> E;

   public:
    explicit TSecondary(const std::string& filename);

    virtual ~TSecondary() {}

    double getEmin() const { return E.front(); }

    double getEmax() const { return E.back(); }

    int getSize() const { return size; }

    double getE(const size_t& i) const { return E.at(i); }

    double getElectrons(const size_t& i) const { return electrons.at(i); }

    double getPositrons(const size_t& i) const { return positrons.at(i); }

    void load_data(const std::string& filename);
};

}  // namespace STATS

#endif  // LIBS_STATS_INCLUDE_TSECONDARY_H_
