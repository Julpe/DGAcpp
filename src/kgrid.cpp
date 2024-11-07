#include "kgrid.h"

#include <iostream>
#include <map>
#include <vector>
#include <xtensor/xarray.hpp>

using namespace KGridConstants;

namespace KGridConstants {
const std::map<KPoint, xt::xarray<double>> K_POINT_VALUES = {
    {KPoint::GAMMA, {0.0, 0.0, 0.0}}, {KPoint::X, {0.5, 0.0, 0.0}},
    {KPoint::Y, {0.0, 0.5, 0.0}},     {KPoint::M, {0.5, 0.5, 0.0}},
    {KPoint::M2, {0.25, 0.25, 0.0}},  {KPoint::Z, {0.0, 0.0, 0.5}},
    {KPoint::R, {0.5, 0.0, 0.5}},     {KPoint::A, {0.5, 0.5, 0.5}}};

const std::map<KPoint, std::string> K_POINT_LABELS = {
    {KPoint::GAMMA, "\u0393"}, {KPoint::X, "X"},    {KPoint::Y, "Y"},
    {KPoint::M, "M"},          {KPoint::M2, "M/2"}, {KPoint::Z, "Z"},
    {KPoint::R, "R"},          {KPoint::A, "A"}};
}  // namespace KGridConstants

xt::xarray<KGridConstants::Symmetry> KGrid::get_symmetry_operations(
    KGridConstants::SymmetrySet symmetry_set) {
    switch (symmetry_set) {
        case SymmetrySet::TWO_DIMENSIONAL_SQUARE:
            return xt::xarray<Symmetry>(
                {Symmetry::X_INV, Symmetry::Y_INV, Symmetry::X_Y_SYM});
        case SymmetrySet::TWO_DIMENSIONAL_NEMATIC:
        case SymmetrySet::QUASI_ONE_DIMENSIONAL_SQUARE:
            return xt::xarray<Symmetry>({Symmetry::X_INV, Symmetry::Y_INV});
        case SymmetrySet::QUASI_TWO_DIMENSIONAL_SQUARE:
            return xt::xarray<Symmetry>({Symmetry::X_INV, Symmetry::Y_INV,
                                         Symmetry::Z_INV, Symmetry::X_Y_SYM});
        case SymmetrySet::SUMULTANEOUS_X_Y_INVERSION:
            return xt::xarray<Symmetry>({Symmetry::X_Y_INV});
        case SymmetrySet::NONE:
        default:
            return xt::xarray<Symmetry>();
    }
}

KGrid::KGrid(std::tuple<uint8_t, uint8_t, uint8_t> nk,
             std::tuple<uint8_t, uint8_t, uint8_t> nq,
             KGridConstants::SymmetrySet symmetry_set)
    : nk(nk), nq(nq) {
    this->_symmetries = get_symmetry_operations(symmetry_set);
}
