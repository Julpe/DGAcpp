#pragma once

#include <kgrid.h>

#include <complex>
#include <xtensor/xarray.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>

namespace HamiltonConstants {
enum class Type { T_TP_TPP, FROM_WANNIER90, EMERY, CUSTOM, NONE };
}

namespace HamiltonElements {
class BaseElement {
   protected:
    BaseElement();
    BaseElement(const xt::xarray<int8_t> &rel_lattice_point,
                const xt::xarray<size_t> &orbitals, double value);

   public:
    xt::xarray<int8_t> rel_lattice_point;
    xt::xarray<size_t> orbitals;
    double value;
};
class HoppingElement : public BaseElement {
   public:
    HoppingElement();
    HoppingElement(const xt::xarray<int8_t> &rel_lattice_point,
                   const xt::xarray<size_t> &orbitals, double value);
};
class InteractionElement : public BaseElement {
   public:
    InteractionElement();
    InteractionElement(const xt::xarray<int8_t> &rel_lattice_point,
                       const xt::xarray<size_t> &orbitals, double value);
};
}  // namespace HamiltonElements

class Hamiltonian {
   private:
    void create_one_band_2d_kinetic_hamiltonian_from_t_tp_tpp(
        const xt::xarray<double> &t_tp_tpp_hoppings);
    void create_kinetic_hamiltonian_from_wannier_file();
    void create_kinetic_hamiltonian(
        const xt::xarray<HamiltonElements::HoppingElement> &hopping_elements);
    void create_interaction_hamiltonian(
        const xt::xarray<HamiltonElements::InteractionElement>
            &interaction_elements);

    xt::xtensor<int8_t, 4> _kinetic_r_grid;
    xt::xtensor<int8_t, 6> _interaction_r_grid;
    xt::xtensor<uint8_t, 2> _kinetic_r_weights, _interaction_r_weights;
    xt::xtensor<uint8_t, 4> _kinetic_orbs;
    xt::xtensor<uint8_t, 6> _interaction_orbs;
    xt::xtensor<float, 3> _kinetic_ham_real;
    xt::xtensor<float, 5> _local_interaction_ham_real,
        _nonlocal_interaction_ham_real;
    xt::xtensor<std::complex<float>, 3> _kinetic_ham_fourier;
    xt::xtensor<std::complex<float>, 5> _local_interaction_ham_fourier,
        _nonlocal_interaction_ham_fourier;

   public:
    HamiltonConstants::Type type;

    Hamiltonian();
    Hamiltonian(const xt::xarray<double> &t_tp_tpp_hoppings);
    Hamiltonian(std::string wannier_filename);
    Hamiltonian(
        const xt::xarray<HamiltonElements::HoppingElement> &hopping_elements,
        const xt::xarray<HamiltonElements::InteractionElement>
            &interaction_elements);

    xt::xarray<double> get_ek(const KGrid &grid);
    xt::xarray<double> get_uk(const KGrid &grid);
};