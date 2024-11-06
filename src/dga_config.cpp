#include "dga_config.h"

BoxSizeConfig::BoxSizeConfig(int niw_core = 30, int niv_core = 30,
                             int niv_shell = 0, int niv_asympt = 0) {
    this->niw_core = niw_core;
    this->niv_core = niv_core;
    this->niv_shell = niv_shell;
    this->niv_asympt = niv_asympt;
}

LatticeConfig::LatticeConfig(tuple<int, int, int> nq, tuple<int, int, int> nk) {
    this->nq = nq;
    this->nk = nk;
}
