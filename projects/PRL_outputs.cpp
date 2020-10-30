// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#include <iostream>
#include <memory>

#include "include/TAverageFlux.h"
#include "include/TFitElectrons.h"
#include "include/TFitPositrons.h"
#include "include/diffusionlosses_full.h"
#include "include/diffusionlosses_test.h"
#include "include/input.h"
#include "include/secondary.h"

int main(int argc, char* argv[]) {
    try {
        //        {
        //            if (argc != 2) throw std::runtime_error("Usage: ./gryphon params.ini");
        //
        //            auto input = std::make_shared<GRYPHON::Input>(argv[1]);
        //            input->print();
        //
        //            std::shared_ptr<GRYPHON::DiffusionLosses> Db;
        //            switch (input->lossesModel) {
        //                case GRYPHON::LossesModel::Test:
        //                    Db = std::make_shared<GRYPHON::DiffusionLossesTest>(input);
        //                    break;
        //                case GRYPHON::LossesModel::Full:
        //                    Db = std::make_shared<GRYPHON::DiffusionLossesFull>(input);
        //                    break;
        //                default:
        //                    throw std::runtime_error("Losses model not implemented yet");
        //            }
        //
        //            Db->save_individuals_components();
        //            Db->save_homogeneous_solution();
        //        }

        {
            auto flux = STATS::TAverageFlux("output/PRL_testhomo_kpc", 17000);
            flux.print("output/PRL_testhomo_kpc.stats");
        }

        {
            auto flux = STATS::TAverageFlux("output/PRL_uniform_kpc", 4300);
            flux.print("output/PRL_uniform_kpc.stats");
        }

        //        {
        //            auto flux = STATS::TAverageFlux("PRL/snr-2.52-uniform-PRL", 1000);
        //            flux.print("PRL/snr-uniform-test-PRL.stats");
        //        }

        /*{
            for (size_t i = 100; i < 120; ++i) {
                auto flux = STATS::TAverageFlux("HOMO/snr-uniform-test", i * 100);
                flux.print("PRL/snr-uniform-test-" + Utilities::str(i * 100) + ".stats");
            }
        }*/

        //        {
        //            auto flux = STATS::TAverageFlux("HOMO/snr-uniform-test", 2000);
        //            flux.print("PRL/snr-uniform-test-2000.stats");
        //        }
        //
        //        {
        //            auto flux = STATS::TAverageFlux("HOMO/snr-uniform-test", 3000);
        //            flux.print("PRL/snr-uniform-test-3000.stats");
        //        }
        //
        //        {
        //            auto flux = STATS::TAverageFlux("HOMO/snr-uniform-test", 5000);
        //            flux.print("PRL/snr-uniform-test-5000.stats");
        //        }
        //
        //        {
        //            auto flux = STATS::TAverageFlux("HOMO/snr-uniform-test", 6500);
        //            flux.print("PRL/snr-uniform-test-10000.stats");
        //        }

        /*{
            auto input = std::make_shared<GRYPHON::Input>(argv[1]);
            auto Db = std::make_shared<GRYPHON::DiffusionLossesFull>(input);

            auto secondary = std::make_shared<GRYPHON::SecondaryLeptons>(input, Db);
            secondary->run();
        }*/

        /*{
                auto fitPositrons = STATS::TFitPositrons();
                fitPositrons.buildSecondary("PRL/PRL_secondary.txt");
                fitPositrons.buildChi2("data/positrons_AMS02.txt");
                fitPositrons.buildPositrons("PRL/pwn-1.62-PRL");
                auto epsilon_PWN = fitPositrons.run(30, 1e4);
                fitPositrons.print("PRL/pwn-PRL.stats");

                auto fitElectrons = STATS::TFitElectrons(epsilon_PWN);
                fitElectrons.buildSecondary("PRL/PRL_secondary.txt");
                fitElectrons.buildChi2("data/electrons_AMS02.txt");
                fitElectrons.buildElectrons("PRL/pwn-1.62-PRL", "PRL/snr-2.39-PRL");
                auto epsilon_SNR = fitElectrons.run(30, 1e4);
                fitElectrons.print("PRL/snr-PRL.stats");
            }*/

        /*{
            auto fitPositrons = STATS::TFitPositrons();
            fitPositrons.buildSecondary("PRL/PRL_secondary.txt");
            fitPositrons.buildChi2("data/positrons_AMS02.txt");
            fitPositrons.buildPositrons("PRL/pwn-1.62-PRL");
            auto epsilon_PWN = fitPositrons.run(30, 1e4);
            fitPositrons.print("PRL/pwn-PRL.stats");

            auto fitElectrons = STATS::TFitElectrons(epsilon_PWN);
            fitElectrons.buildSecondary("PRL/PRL_secondary.txt");
            fitElectrons.buildChi2("data/electrons_AMS02.txt");
            fitElectrons.buildElectrons("PRL/pwn-1.62-PRL", "PRL/snr-2.39-nokn-PRL");
            auto epsilon_SNR = fitElectrons.run(30, 1e4);
            fitElectrons.print("PRL/snr-nokn-PRL.stats");
        }*/

        /* {
             auto fitPositrons = STATS::TFitPositrons();
             fitPositrons.buildSecondary("PRL/PRL_secondary.txt");
             fitPositrons.buildChi2("data/positrons_AMS02.txt");
             fitPositrons.buildPositrons("PRL/pwn-1.62-PRL");
             auto epsilon_PWN = fitPositrons.run(30, 1e4);
             fitPositrons.print("PRL/pwn-PRL.stats");

             auto fitElectrons = STATS::TFitElectrons(epsilon_PWN);
             fitElectrons.buildSecondary("PRL/PRL_secondary.txt");
             fitElectrons.buildChi2(
                 "data/electrons_AMS02.txt");  // TODO(CE): put an error if not built
             fitElectrons.buildElectrons("PRL/pwn-1.62-PRL", "PRL/snr-uniform-test");
             auto epsilon_SNR = fitElectrons.run(30, 1e4);
             fitElectrons.print("PRL/snr-uniform-test.stats");
         }*/

        /*{
            auto fitPositrons = STATS::TFitPositrons();
            fitPositrons.buildSecondary("PRL/PRL_secondary.txt");
            fitPositrons.buildChi2("data/positrons_AMS02.txt");
            fitPositrons.buildPositrons("PRL/pwn-1.62-PRL");
            auto epsilon_PWN = fitPositrons.run(30, 1e4);
            fitPositrons.print("PRL/pwn-PRL.stats");

            auto fitElectrons = STATS::TFitElectrons(epsilon_PWN);
            fitElectrons.buildSecondary("PRL/PRL_secondary.txt");
            fitElectrons.buildChi2(
                "data/electrons_AMS02.txt");  // TODO(CE): put an error if not built
            fitElectrons.buildElectrons("PRL/pwn-1.62-PRL", "PRL/snr-2.39-lowsnr-PRL");
            auto epsilon_SNR = fitElectrons.run(30, 1e4);
            fitElectrons.print("PRL/snr-2.39-lowsnr-PRL.stats");
        }*/
    } catch (std::exception& e) {
        std::cout << "!Fatal Error: " << e.what() << '\n';
    }
    return 0;
}
