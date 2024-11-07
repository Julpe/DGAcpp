#pragma once

#include <kgrid.h>

#include <complex>

namespace HamiltonConstants {
enum class Type { T_TP_TPP, FROM_WANNIER90, EMERY };
}

namespace HamiltonElements {
class BaseElement {
   protected:
    BaseElement(std::vector<int> rel_lattice_point,
                std::vector<uint8_t> orbitals, double value);

   public:
    std::vector<int> rel_lattice_point;
    std::vector<uint8_t> orbitals;
    double value;
};
class HoppingElement : public BaseElement {
   public:
    HoppingElement(std::vector<int> rel_lattice_point,
                   std::vector<uint8_t> orbitals, double value);
};
class InteractionElement : public BaseElement {
   public:
    InteractionElement(std::vector<int> rel_lattice_point,
                       std::vector<uint8_t> orbitals, double value);
};
}  // namespace HamiltonElements

class Hamiltonian {
   private:
    void create_one_band_kinetic_hamiltonian_from_2d_t_tp_tpp(
        std::vector<double> t_tp_tpp_hoppings);
    void create_kinetic_hamiltonian_from_wannier_file();
    void create_kinetic_hamiltonian(
        std::vector<HamiltonElements::HoppingElement> hopping_elements);
    void create_interaction_hamiltonian(
        std::vector<HamiltonElements::InteractionElement> interaction_elements);

    xt::xtensor<int, 4> _kinetic_r_grid;
    xt::xtensor<int, 6> _interaction_r_grid;
    xt::xtensor<uint8_t, 2> _kinetic_r_weights, _interaction_r_weights;
    xt::xtensor<uint8_t, 4> _kinetic_orbs;
    xt::xtensor<uint8_t, 6> _interaction_orbs;
    xt::xarray<std::complex<double>> _kinetic_hamiltonian_real,
        _interaction_hamiltonian_real, _kinetic_hamiltonian_fourier,
        _interaction_hamiltonian_fourier;

   public:
    HamiltonConstants::Type type;

    Hamiltonian(std::vector<double> t_tp_tpp_hoppings);
    Hamiltonian(std::string wannier_filename);

    xt::xarray<double> get_ek(KGrid grid);
    xt::xarray<double> get_uk(KGrid grid);
};