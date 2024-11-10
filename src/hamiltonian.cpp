#include "hamiltonian.h"

#include <cassert>

HamiltonElements::BaseElement::BaseElement()
    : BaseElement(xt::xarray<int8_t>(), xt::xarray<int8_t>(), 0.0) {}

HamiltonElements::BaseElement::BaseElement(
    const xt::xarray<int8_t> &rel_lattice_point,
    const xt::xarray<size_t> &orbitals, double value)
    : rel_lattice_point(rel_lattice_point), orbitals(orbitals), value(value) {
    assert(rel_lattice_point.size() == 3 &&
           "Only three dimensions are supported for the "
           "definitions of lattice points.");
}

HamiltonElements::HoppingElement::HoppingElement() : BaseElement() {}

HamiltonElements::HoppingElement::HoppingElement(
    const xt::xarray<int8_t> &rel_lattice_point,
    const xt::xarray<size_t> &orbitals, double value)
    : BaseElement(rel_lattice_point, orbitals, value) {
    assert(orbitals.size() == 2 && "Hopping Elements need two orbitals.");
}

HamiltonElements::InteractionElement::InteractionElement() : BaseElement() {}

HamiltonElements::InteractionElement::InteractionElement(
    const xt::xarray<int8_t> &rel_lattice_point,
    const xt::xarray<size_t> &orbitals, double value)
    : BaseElement(rel_lattice_point, orbitals, value) {
    assert(orbitals.size() == 4 && "Hopping Elements need four orbitals.");
}

Hamiltonian::Hamiltonian() : type(HamiltonConstants::Type::NONE) {}

Hamiltonian::Hamiltonian(const xt::xarray<double> &t_tp_tpp_hoppings)
    : type(HamiltonConstants::Type::T_TP_TPP) {
    assert(t_tp_tpp_hoppings.size() == 3 &&
           "Only three hopping elements are allowed.");
    create_one_band_2d_kinetic_hamiltonian_from_t_tp_tpp(t_tp_tpp_hoppings);
}

Hamiltonian::Hamiltonian(std::string wannier_filename)
    : type(HamiltonConstants::Type::FROM_WANNIER90) {}

Hamiltonian::Hamiltonian(
    const xt::xarray<HamiltonElements::HoppingElement> &hopping_elements,
    const xt::xarray<HamiltonElements::InteractionElement>
        &interaction_elements) {
    create_kinetic_hamiltonian(hopping_elements);
    create_interaction_hamiltonian(interaction_elements);
}

void Hamiltonian::create_one_band_2d_kinetic_hamiltonian_from_t_tp_tpp(
    const xt::xarray<double> &t_tp_tpp_hoppings) {
    const double t = t_tp_tpp_hoppings.at(0), tp = t_tp_tpp_hoppings.at(1),
                 tpp = t_tp_tpp_hoppings.at(2);

    const xt::xarray<size_t> local_orbs{1, 1};
    const xt::xarray<HamiltonElements::HoppingElement> hopping_elements = {
        HamiltonElements::HoppingElement(xt::xarray<int8_t>{1, 0, 0},
                                         local_orbs, t),
        HamiltonElements::HoppingElement(xt::xarray<int8_t>{0, 1, 0},
                                         local_orbs, t),
        HamiltonElements::HoppingElement(xt::xarray<int8_t>{-1, 0, 0},
                                         local_orbs, t),
        HamiltonElements::HoppingElement(xt::xarray<int8_t>{0, -1, 0},
                                         local_orbs, t),
        HamiltonElements::HoppingElement(xt::xarray<int8_t>{1, 1, 0},
                                         local_orbs, tp),
        HamiltonElements::HoppingElement(xt::xarray<int8_t>{1, -1, 0},
                                         local_orbs, tp),
        HamiltonElements::HoppingElement(xt::xarray<int8_t>{-1, 1, 0},
                                         local_orbs, tp),
        HamiltonElements::HoppingElement(xt::xarray<int8_t>{-1, -1, 0},
                                         local_orbs, tp),
        HamiltonElements::HoppingElement(xt::xarray<int8_t>{2, 0, 0},
                                         local_orbs, tpp),
        HamiltonElements::HoppingElement(xt::xarray<int8_t>{0, 2, 0},
                                         local_orbs, tpp),
        HamiltonElements::HoppingElement(xt::xarray<int8_t>{-2, 0, 0},
                                         local_orbs, tpp),
        HamiltonElements::HoppingElement(xt::xarray<int8_t>{0, -2, 0},
                                         local_orbs, tpp)};

    const xt::xarray<HamiltonElements::InteractionElement>
        interaction_elements = {HamiltonElements::InteractionElement(
            xt::xarray<int8_t>{0, 2, 0}, xt::xarray<size_t>{1, 1, 1, 1}, 8.0)};

    create_kinetic_hamiltonian(hopping_elements);
    create_interaction_hamiltonian(interaction_elements);
}

void Hamiltonian::create_kinetic_hamiltonian(
    const xt::xarray<HamiltonElements::HoppingElement> &hopping_elements) {
    size_t hopping_elements_size = hopping_elements.size();

    size_t n_orbs = 0;
    for (const HamiltonElements::HoppingElement &he : hopping_elements) {
        n_orbs = std::max(
            n_orbs, *std::max_element(he.orbitals.begin(), he.orbitals.end()));
    }

    _kinetic_r_grid = xt::zeros<int8_t>(
        xt::xarray<size_t>{hopping_elements_size, n_orbs, n_orbs, 3});

    _kinetic_orbs = xt::ones<uint8_t>(
        xt::xarray<size_t>{hopping_elements_size, n_orbs, n_orbs, 2});

    _kinetic_r_weights =
        xt::ones<uint8_t>(xt::xarray<size_t>{hopping_elements_size, 1});

    _kinetic_ham_real =
        xt::zeros<float>({hopping_elements_size, n_orbs, n_orbs});

    for (size_t r = 0; r < hopping_elements_size; ++r) {
        HamiltonElements::HoppingElement he = hopping_elements.at(r);

        xt::view(_kinetic_r_grid, r, xt::all(), xt::all(), xt::all()) =
            he.rel_lattice_point;

        for (size_t i = 0; i < n_orbs; ++i) {
            for (size_t j = 0; j < n_orbs; ++j) {
                _kinetic_orbs(r, i, j, 0) = i + 1;
                _kinetic_orbs(r, i, j, 1) = j + 1;
            }
        }

        _kinetic_ham_real(r, he.orbitals.at(0) - 1, he.orbitals.at(1) - 1) =
            he.value;
    }
}

void Hamiltonian::create_interaction_hamiltonian(
    const xt::xarray<HamiltonElements::InteractionElement>
        &interaction_elements) {
    const size_t int_elements_size = interaction_elements.size();

    size_t n_orbs = 0;
    for (const auto &he : interaction_elements) {
        n_orbs = std::max(
            n_orbs, *std::max_element(he.orbitals.begin(), he.orbitals.end()));
    }

    _interaction_r_grid = xt::zeros<int8_t>(xt::xarray<size_t>{
        int_elements_size, n_orbs, n_orbs, n_orbs, n_orbs, 3});

    _interaction_orbs = xt::ones<uint8_t>(xt::xarray<size_t>{
        int_elements_size, n_orbs, n_orbs, n_orbs, n_orbs, 4});

    _interaction_r_weights =
        xt::ones<uint8_t>(xt::xarray<size_t>{int_elements_size, 1});

    _local_interaction_ham_real =
        xt::zeros<float>({int_elements_size, n_orbs, n_orbs, n_orbs, n_orbs});
    _nonlocal_interaction_ham_real =
        xt::zeros<float>({int_elements_size, n_orbs, n_orbs, n_orbs, n_orbs});

    const xt::xarray<int8_t> zero_lattice_point{0, 0, 0};

    for (size_t r = 0; r < int_elements_size; ++r) {
        HamiltonElements::InteractionElement ie = interaction_elements.at(r);

        xt::view(_interaction_r_grid, r, xt::all(), xt::all(), xt::all(),
                 xt::all(), xt::all()) = ie.rel_lattice_point;

        for (size_t i = 0; i < n_orbs; ++i) {
            for (size_t j = 0; j < n_orbs; ++j) {
                for (size_t k = 0; k < n_orbs; ++k) {
                    for (size_t l = 0; l < n_orbs; ++l) {
                        _interaction_orbs(r, i, j, k, l, 0) = i + 1;
                        _interaction_orbs(r, i, j, k, l, 1) = j + 1;
                        _interaction_orbs(r, i, j, k, l, 2) = k + 1;
                        _interaction_orbs(r, i, j, k, l, 3) = l + 1;
                    }
                }
            }
        }

        if (xt::all(xt::equal(ie.rel_lattice_point, zero_lattice_point))) {
            _local_interaction_ham_real(
                r, ie.orbitals.at(0) - 1, ie.orbitals.at(1) - 1,
                ie.orbitals.at(2) - 1, ie.orbitals.at(3) - 1) = ie.value;
        } else {
            _nonlocal_interaction_ham_real(
                r, ie.orbitals.at(0) - 1, ie.orbitals.at(1) - 1,
                ie.orbitals.at(2) - 1, ie.orbitals.at(3) - 1) = ie.value;
        }
    }
}
