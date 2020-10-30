// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#include <iostream>

#include "include/TAnisotropyCombo.h"
#include "include/TFitElectrons.h"
#include "include/TFitPositrons.h"

void plot_onlyelectrons() {
    {
        double epsilon_PWN = 0;
        auto fitElectrons = STATS::TFitElectrons(epsilon_PWN);
        fitElectrons.buildSecondary("output/secondary_1e1_1e5_32_secondary.txt");
        fitElectrons.buildChi2("data/electrons_AMS02.txt");
        fitElectrons.buildElectrons("output/snr_2.55_9.0_nopwn", "output/snr_2.55_9.0_nopwn");
        auto epsilon_SNR = fitElectrons.run(20, 40);
        fitElectrons.print("output/snr_2.55_9.0_nopwn.stats");
    }

    {
        double epsilon_PWN = 0;
        auto fitElectrons = STATS::TFitElectrons(epsilon_PWN);
        fitElectrons.buildSecondary("output/secondary_1e1_1e5_32_secondary.txt");
        fitElectrons.buildChi2("data/electrons_AMS02.txt");
        fitElectrons.buildElectrons("output/snr_2.56_8.5_nopwn", "output/snr_2.56_8.5_nopwn");
        auto epsilon_SNR = fitElectrons.run(20, 40);
        fitElectrons.print("output/snr_2.56_8.5_nopwn.stats");
    }

    {
        double epsilon_PWN = 0;
        auto fitElectrons = STATS::TFitElectrons(epsilon_PWN);
        fitElectrons.buildSecondary("output/secondary_1e1_1e5_32_secondary.txt");
        fitElectrons.buildChi2("data/electrons_AMS02.txt");
        fitElectrons.buildElectrons("output/snr_2.56_8.5_lowrate_nopwn",
                                    "output/snr_2.56_8.5_lowrate_nopwn");
        auto epsilon_SNR = fitElectrons.run(20, 40);
        fitElectrons.print("output/snr_2.56_8.5_lowrate_nopwn.stats");
    }

    {
        double epsilon_PWN = 0;
        auto fitElectrons = STATS::TFitElectrons(epsilon_PWN);
        fitElectrons.buildSecondary("output/secondary_1e1_1e5_32_secondary.txt");
        fitElectrons.buildChi2("data/electrons_AMS02.txt");
        fitElectrons.buildElectrons("output/snr_2.59_8.0_nopwn", "output/snr_2.59_8.0_nopwn");
        auto epsilon_SNR = fitElectrons.run(20, 40);
        fitElectrons.print("output/snr_2.59_8.0_nopwn.stats");
    }
}

void plot_fullobservables() {
    //    {
    //        auto fitPositrons = STATS::TFitPositrons();
    //        fitPositrons.buildSecondary("output/secondary_1e1_1e5_32_secondary.txt");
    //        fitPositrons.buildChi2("data/positrons_AMS02.txt");
    //        fitPositrons.buildPositrons("ulite/pwn-1.70-2.60-fast-burst");
    //        auto epsilon_PWN = fitPositrons.run(30, 1e4);
    //        fitPositrons.print("output/pwn-1.70-fast-burst.stats");
    //    }

    double epsilon_PWN = 0;
    {
        auto fitPositrons = STATS::TFitPositrons();
        fitPositrons.buildSecondary("repository_prd2020/secondary_1e1_1e5_32_secondary.txt");
        fitPositrons.buildChi2("data/positrons_AMS02.txt");
        fitPositrons.buildPositrons("repository_prd2020/pwn_burst_1.80");
        epsilon_PWN = fitPositrons.run(20, 1e3);
        fitPositrons.print("output/pwn_burst_1.80.stats");
    }

    std::cout << epsilon_PWN << "\n";
    {
        auto fitElectrons = STATS::TFitElectrons(epsilon_PWN);
        fitElectrons.buildSecondary("repository_prd2020/secondary_1e1_1e5_32_secondary.txt");
        fitElectrons.buildChi2("data/electrons_AMS02.txt");
        fitElectrons.buildElectrons("repository_prd2020/pwn_burst_1.80",
                                    "repository_prd2020/snr_2.58");
        auto epsilon_SNR = fitElectrons.run(20, 1e3);
        fitElectrons.print("output/snr_2.58.stats");
    }
}

void plot_nokn() {
    double epsilon_PWN = 0;
    {
        auto fitPositrons = STATS::TFitPositrons();
        fitPositrons.buildSecondary("ulite/secondary_1e1_1e5_32_secondary.txt");
        fitPositrons.buildChi2("data/positrons_AMS02.txt");
        fitPositrons.buildPositrons("ulite/pwn_burst_1.80");
        epsilon_PWN = fitPositrons.run(20, 1e3);
        fitPositrons.print("output/pwn_burst_1.80.stats");
    }

    std::cout << epsilon_PWN << "\n";
    {
        auto fitElectrons = STATS::TFitElectrons(epsilon_PWN);
        fitElectrons.buildSecondary("ulite/secondary_1e1_1e5_32_secondary.txt");
        fitElectrons.buildChi2("data/electrons_AMS02.txt");
        fitElectrons.buildElectrons("ulite/pwn_burst_1.80", "output/snr_2.38_nokn");
        auto epsilon_SNR = fitElectrons.run(20, 1e2);
        fitElectrons.print("output/snr_2.38_nokn.stats");
    }

    std::cout << epsilon_PWN << "\n";
    {
        auto fitElectrons = STATS::TFitElectrons(epsilon_PWN);
        fitElectrons.buildSecondary("ulite/secondary_1e1_1e5_32_secondary.txt");
        fitElectrons.buildChi2("data/electrons_AMS02.txt");
        fitElectrons.buildElectrons("ulite/pwn_burst_1.80", "output/snr_2.36_nokn");
        auto epsilon_SNR = fitElectrons.run(20, 1e2);
        fitElectrons.print("output/snr_2.36_nokn.stats");
    }
}

void check_burst_vs_running() {
    {
        auto fitPositrons = STATS::TFitPositrons();
        fitPositrons.buildSecondary("output/secondary_1e2_1e5_16_secondary.txt");
        fitPositrons.buildChi2("data/positrons_AMS02.txt");
        fitPositrons.buildPositrons("ulite/pwn-1.70-2.60-running");
        auto epsilon_PWN = fitPositrons.run(1e2, 1e4);
        fitPositrons.print("output/pwn-HE-running.stats");
    }

    //    {
    //        auto fitPositrons = STATS::TFitPositrons();
    //        fitPositrons.buildSecondary("output/secondary_HE_secondary.txt");
    //        fitPositrons.buildChi2("data/positrons_AMS02.txt");
    //        fitPositrons.buildPositrons("output/pwn-HE-burst");
    //        auto epsilon_PWN = fitPositrons.run(1e2, 1e4);
    //        fitPositrons.print("output/pwn-HE-burst.stats");
    //    }

    {
        auto fitPositrons = STATS::TFitPositrons();
        fitPositrons.buildSecondary("output/secondary_HE_secondary.txt");
        fitPositrons.buildChi2("data/positrons_AMS02.txt");
        fitPositrons.buildPositrons("ulite/pwn-1.70-2.60-fast-running");
        auto epsilon_PWN = fitPositrons.run(1e2, 1e4);
        fitPositrons.print("output/pwn-HE-fast-running.stats");
    }

    //    {
    //        auto fitPositrons = STATS::TFitPositrons();
    //        fitPositrons.buildSecondary("output/secondary_HE_secondary.txt");
    //        fitPositrons.buildChi2("data/positrons_AMS02.txt");
    //        fitPositrons.buildPositrons("output/pwn-HE-burst");
    //        auto epsilon_PWN = fitPositrons.run(1e2, 1e4);
    //        fitPositrons.print("output/pwn-HE-burst.stats");
    //    }
}

int main(int argc, char* argv[]) {
    try {
        // plot_onlyelectrons();
        plot_fullobservables();
        // plot_nokn();
        // check_burst_vs_running();
    } catch (std::exception& e) {
        std::cout << "!Fatal Error: " << e.what() << '\n';
    }
    return 0;
}
