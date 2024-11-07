#pragma once

#include <tuple>
#include <xtensor/xarray.hpp>

namespace KGridConstants {
enum class SymmetrySet {
    TWO_DIMENSIONAL_SQUARE,
    TWO_DIMENSIONAL_NEMATIC,
    QUASI_ONE_DIMENSIONAL_SQUARE,
    QUASI_TWO_DIMENSIONAL_SQUARE,
    SUMULTANEOUS_X_Y_INVERSION,
    NONE
};

enum class Symmetry { X_INV, Y_INV, Z_INV, X_Y_SYM, X_Y_INV };

enum class KPoint { GAMMA, X, Y, M, M2, Z, R, A, COUNT };

extern const std::map<KPoint, xt::xarray<double>> KNOWN_K_POINTS;
extern const std::map<KPoint, std::string> K_POINT_LABELS;
}  // namespace KGridConstants

class KGrid {
   private:
    xt::xarray<KGridConstants::Symmetry> _symmetries;
    xt::xarray<KGridConstants::Symmetry> get_symmetry_operations(
        KGridConstants::SymmetrySet symmetry_set);

   public:
    std::tuple<uint8_t, uint8_t, uint8_t> nk, nq;
    KGrid(std::tuple<uint8_t, uint8_t, uint8_t> nk,
          std::tuple<uint8_t, uint8_t, uint8_t> nq,
          KGridConstants::SymmetrySet symmetry_set);
};