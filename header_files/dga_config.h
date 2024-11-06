#pragma once

#include <tuple>

using namespace std;

class ConfigBase {};

class BoxSizeConfig : public ConfigBase {
   public:
    int niw_core, niv_core, niv_shell, niv_asympt;
    BoxSizeConfig(int niw_core = 30, int niv_core = 30, int niv_shell = 0,
                  int niv_asympt = 0);
};

class LatticeConfig : public ConfigBase {
   public:
    tuple<int, int, int> nq, nk;
    LatticeConfig(tuple<int, int, int> nq, tuple<int, int, int> nk);
};