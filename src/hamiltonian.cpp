#include "hamiltonian.h"

#include <cassert>

#include "xtensor/xtensor.hpp"

HamiltonElements::BaseElement::BaseElement(std::vector<int> rel_lattice_point,
                                           std::vector<uint8_t> orbitals,
                                           double value)
    : rel_lattice_point(rel_lattice_point), orbitals(orbitals), value(value) {
    assert(rel_lattice_point.size() == 3 &&
           "Only three dimensions are supported for the "
           "definitions of lattice points.");
}

HamiltonElements::HoppingElement::HoppingElement(
    std::vector<int> rel_lattice_point, std::vector<uint8_t> orbitals,
    double value)
    : BaseElement(rel_lattice_point, orbitals, value) {
    assert(orbitals.size() == 2 &&
           "Hopping Elements only employ the use of two orbitals.");
}

HamiltonElements::InteractionElement::InteractionElement(
    std::vector<int> rel_lattice_point, std::vector<uint8_t> orbitals,
    double value)
    : BaseElement(rel_lattice_point, orbitals, value) {
    assert(orbitals.size() == 4 &&
           "Hopping Elements only employ the use of four orbitals.");
}

Hamiltonian::Hamiltonian(std::vector<double> t_tp_tpp_hoppings)
    : type(HamiltonConstants::Type::T_TP_TPP) {
    assert(t_tp_tpp_hoppings.size() == 3 &&
           "Only three hopping elements are allowed.");
}

Hamiltonian::Hamiltonian(std::string wannier_filename)
    : type(HamiltonConstants::Type::FROM_WANNIER90) {}

void Hamiltonian::create_one_band_kinetic_hamiltonian_from_2d_t_tp_tpp(
    const std::vector<double> t_tp_tpp_hoppings) {
    const double t = t_tp_tpp_hoppings.at(0), tp = t_tp_tpp_hoppings.at(1),
                 tpp = t_tp_tpp_hoppings.at(2);

    const std::vector<uint8_t> local_orbs{1, 1};
    const std::vector<HamiltonElements::HoppingElement> hopping_elements = {
        HamiltonElements::HoppingElement(std::vector<int>{1, 0, 0}, local_orbs,
                                         t),
        HamiltonElements::HoppingElement(std::vector<int>{0, 1, 0}, local_orbs,
                                         t),
        HamiltonElements::HoppingElement(std::vector<int>{-1, 0, 0}, local_orbs,
                                         t),
        HamiltonElements::HoppingElement(std::vector<int>{0, -1, 0}, local_orbs,
                                         t),
        HamiltonElements::HoppingElement(std::vector<int>{1, 1, 0}, local_orbs,
                                         tp),
        HamiltonElements::HoppingElement(std::vector<int>{1, -1, 0}, local_orbs,
                                         tp),
        HamiltonElements::HoppingElement(std::vector<int>{-1, 1, 0}, local_orbs,
                                         tp),
        HamiltonElements::HoppingElement(std::vector<int>{-1, -1, 0},
                                         local_orbs, tp),
        HamiltonElements::HoppingElement(std::vector<int>{2, 0, 0}, local_orbs,
                                         tpp),
        HamiltonElements::HoppingElement(std::vector<int>{0, 2, 0}, local_orbs,
                                         tpp),
        HamiltonElements::HoppingElement(std::vector<int>{-2, 0, 0}, local_orbs,
                                         tpp),
        HamiltonElements::HoppingElement(std::vector<int>{0, -2, 0}, local_orbs,
                                         tpp)};

    create_kinetic_hamiltonian(hopping_elements);

    _kinetic_r_grid =
        xt::xarray<int>::from_shape({hopping_elements.size(), 1, 1, 3});
    _kinetic_r_grid = {{1, 0, 0}, {0, 1, 0},  {-1, 0, 0}, {0, -1, 0},
                       {1, 1, 0}, {1, -1, 0}, {-1, 1, 0}, {-1, -1, 0},
                       {2, 0, 0}, {0, 2, 0},  {-2, 0, 0}, {0, -2, 0}};

    _kinetic_r_grid = xt::reshape_view(_kinetic_r_grid, {12, 1, 1, 3});

    _kinetic_r_weights = xt::ones<double>({12, 1});
    _kinetic_orbs = xt::ones<double>({12, 1, 1, 2});
}

void Hamiltonian::create_kinetic_hamiltonian(
    std::vector<HamiltonElements::HoppingElement> hopping_elements) {
    xt::xtensor<double, 3> test({3, 3, 3});

    uint8_t n_orbs = 0;
    for (const auto& he : hopping_elements) {
        n_orbs = std::max(
            n_orbs, *std::max_element(he.orbitals.begin(), he.orbitals.end()));
    }

    _kinetic_hamiltonian_real =
        xt::xarray<double>::from_shape({hopping_elements.size()});

    for (size_t i = 0; i < hopping_elements.size(); ++i) {
        _kinetic_hamiltonian_real.at(i) = hopping_elements.at(i).value;
    }

    _kinetic_hamiltonian_real = xt::reshape_view(
        _kinetic_hamiltonian_real,
        std::vector<size_t>{_kinetic_hamiltonian_real.size(), 1, 1});
}
