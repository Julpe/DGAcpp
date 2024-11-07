#include "dga_config.h"

BoxSizeConfig::BoxSizeConfig(uint8_t niw_core = 30, uint8_t niv_core = 30,
                             uint8_t niv_shell = 0, uint8_t niv_asympt = 0)
    : niw_core(niw_core),
      niv_core(niv_core),
      niv_shell(niv_shell),
      niv_asympt(niv_asympt) {}

LatticeConfig::LatticeConfig(tuple<uint8_t, uint8_t, uint8_t> nk = {16, 16, 1},
                             tuple<uint8_t, uint8_t, uint8_t> nq = {0, 0, 0})
    : nk(nk),
      nq(nq == std::tuple<uint8_t, uint8_t, uint8_t>(0, 0, 0)
             ? std::make_tuple(std::get<0>(nk), std::get<1>(nk),
                               std::get<2>(nk))
             : nq) {}
