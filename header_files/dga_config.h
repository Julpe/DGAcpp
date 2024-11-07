#pragma once

#include <stdint.h>

#include <tuple>

using namespace std;

class ConfigBase {};

class BoxSizeConfig : public ConfigBase {
   public:
    uint8_t niw_core, niv_core, niv_shell, niv_asympt;
    BoxSizeConfig(uint8_t niw_core, uint8_t niv_core, uint8_t niv_shell,
                  uint8_t niv_asympt);
};

class LatticeConfig : public ConfigBase {
   public:
    tuple<uint8_t, uint8_t, uint8_t> nk, nq;
    LatticeConfig(tuple<uint8_t, uint8_t, uint8_t> nk,
                  tuple<uint8_t, uint8_t, uint8_t> nq);
};