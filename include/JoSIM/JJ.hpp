// Copyright (c) 2020 Johannes Delport
// This code is licensed under MIT license (see LICENSE for details)
#ifndef JOSIM_JJ_HPP
#define JOSIM_JJ_HPP

#include "JoSIM/BasicComponent.hpp"
#include "JoSIM/ParameterName.hpp"
#include "JoSIM/Parameters.hpp"
#include "JoSIM/Model.hpp"
#include "JoSIM/AnalysisType.hpp"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <optional>

namespace JoSIM {

/*
  Blabel V⁺ V⁻ jjmodel area=value

  V - (hbar/2e)(3/2h)φ = (hbar/2e)(-(2/h)φn-1 + (1/2h)φn-2)
  V - (2Rh/(3RC + 2h))Io = -(2Rh/(3RC + 2h))(Icsinφ0 - (2C/h)Vn-1 + (C/2h)Vn-2)
  φ0 = (4/3)φn-1 - (1/3)φn-2 + (2e/hbar)(2h/3)v0
  v0 = (5/2)Vn-1 - 2Vn-2 + (1/2)Vn-3

  ⎡ 0  0                0                 1⎤ ⎡ V⁺⎤   ⎡    0⎤
  ⎜ 0  0                0                -1⎟ ⎜ V⁻⎟ = ⎜    0⎟
  ⎜ 1 -1 -(hbar/2e)(3/2h)                 0⎟ ⎜ φ ⎟   ⎜ RHS1⎟
  ⎣ 1 -1                0 -(2Rh/(3RC + 2h))⎦ ⎣ Io⎦   ⎣ RHS2⎦

  RHS1 = (hbar/2e)(-(2/h)φn-1 + (1/2h)φn-2)
  RHS2 = -(2Rh/(3RC + 2h))(Icsinφ0 - (2C/h)Vn-1 + (C/2h)Vn-2)

  (PHASE)
  φ - (2e/hbar)(2h/3)V = (4/3)φn-1 - (1/3)φn-2

  ⎡ 0  0                0                 1⎤ ⎡ φ⁺⎤   ⎡    0⎤
  ⎜ 0  0                0                -1⎟ ⎜ φ⁻⎟ = ⎜    0⎟
  ⎜ 1 -1 -(2e/hbar)(2h/3)                 0⎟ ⎜ V ⎟   ⎜ RHS1⎟
  ⎣ 1 -1                0 -(2Rh/(3RC + 2h))⎦ ⎣ Io⎦   ⎣ RHS2⎦

  RHS1 = (4/3)φn-1 - (1/3)φn-2
*/ 

class JJ : public BasicComponent {
  private:
    int hDepPos_ = 0;
    int state_ = 0;
    double h_ = 0.0;
  public:
  int variableIndex_ = 0;
  double area_ = 0.0;
  std::optional<Model> model_;
  double phaseConst_ = 0.0;
  double lowerB_ = 0.0, upperB_ = 0.0, gLarge_ = 0.0;
  double del0_ = 0.0, del_ = 0.0, rncalc_ = 0.0;
  double pn1_ = 0.0, pn2_ = 0.0, phi0_ = 0.0;
  double vn1_ = 0.0, vn2_ = 0.0, vn3_ = 0.0;
  double transitionCurrent_ = 0.0;
  JoSIM::AnalysisType at_;

  JJ(
    const std::pair<tokens_t, string_o> &s, const NodeConfig &ncon,
    const nodemap &nm, std::unordered_set<std::string> &lm, nodeconnections &nc,
    const param_map &pm, const vector_pair_t<Model, string_o> &models,
    const AnalysisType &at, const double &h, int &bi);

  double subgap_impedance(const double factor = 1);
  double transient_impedance(const double factor = 1);
  double normal_impedance(const double factor = 1);

  void set_matrix_info();

  void set_model(
    const tokens_t &t, const vector_pair_t<Model, string_o> &models, 
    const string_o &subc);

  bool update_value(const double &v);

  void update_timestep(const double &factor) override;
}; // class JJ

} // namespace JoSIM
#endif