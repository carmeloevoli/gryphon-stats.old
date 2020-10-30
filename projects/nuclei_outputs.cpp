// Copyright 2020 Carmelo Evoli (GSSI) - MIT License
#include <iostream>

#include "include/TFitProtons.h"

int main(int argc, char* argv[]) {
    try {
        {
            double epsilon = 0;
            auto fitProtons = STATS::TFitProtons();
            fitProtons.buildChi2("data/H_AMS02_rigidity.txt");
            fitProtons.buildProtons("ulite/protons_uniform");
            epsilon = fitProtons.run(40, 200);
            fitProtons.print("output/protons_uniform.stats", epsilon);
        }

        {
            double epsilon = 0;
            auto fitProtons = STATS::TFitProtons();
            fitProtons.buildChi2("data/H_AMS02_rigidity.txt");
            fitProtons.buildProtons("ulite/protons_jelly");
            epsilon = fitProtons.run(40, 200);
            fitProtons.print("output/protons_jelly.stats", epsilon);
        }

        {
            double epsilon = 0;
            auto fitProtons = STATS::TFitProtons();
            fitProtons.buildChi2("data/H_AMS02_rigidity.txt");
            fitProtons.buildProtons("ulite/protons_spirals");
            epsilon = fitProtons.run(40, 200);
            fitProtons.print("output/protons_spirals.stats", epsilon);
        }

        //        {
        //            double epsilon = 0;
        //            auto fitHelium = STATS::TFitProtons();
        //            fitHelium.buildChi2("data/He_AMS02_rigidity.txt");
        //            fitHelium.buildProtons("output/nuclei_Z2");
        //            epsilon = fitHelium.run(200, 400);
        //            fitHelium.print("output/nuclei_Z2.stats");
        //        }
    } catch (std::exception& e) {
        std::cout << "!Fatal Error: " << e.what() << '\n';
    }
    return 0;
}
