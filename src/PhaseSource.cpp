// Copyright (c) 2019 Johannes Delport
// This code is licensed under MIT license (see LICENSE for details)

#include "JoSIM/PhaseSource.hpp"
#include "JoSIM/Misc.hpp"
#include "JoSIM/Errors.hpp"
#include "JoSIM/Constants.hpp"

#include <utility>

using namespace JoSIM;

PhaseSource PhaseSource::create_phasesource(
    const std::pair<std::string, std::string> &s,
    const std::unordered_map<std::string, int> &nm, 
    std::unordered_set<std::string> &lm,
    std::vector<std::vector<std::pair<double, int>>> &nc,
    int &branchIndex) {
  std::vector<std::string> tokens = Misc::tokenize_space(s.first);
  
  PhaseSource temp;
  temp.set_label(tokens.at(0), lm);
  temp.set_nonZeros_and_columnIndex(std::make_pair(tokens.at(1), tokens.at(2)), nm, s.first, branchIndex);
  temp.set_indices(std::make_pair(tokens.at(1), tokens.at(2)), nm, nc, branchIndex);
  temp.set_currentIndex(branchIndex - 1);
  return temp;
}

void PhaseSource::set_label(const std::string &s, std::unordered_set<std::string> &lm) {
  if(lm.count(s) != 0) {
    Errors::invalid_component_errors(ComponentErrors::DUPLICATE_LABEL, s);
  } else {
    label_ = s;
    lm.emplace(s);
  }
}

void PhaseSource::set_nonZeros_and_columnIndex(const std::pair<std::string, std::string> &n, const std::unordered_map<std::string, int> &nm, const std::string &s, int &branchIndex) {
  nonZeros_.clear();
  columnIndex_.clear();
  if(n.first != "0" && n.first.find("GND") == std::string::npos) {
    if(nm.count(n.first) == 0) Errors::netlist_errors(NetlistErrors::NO_SUCH_NODE, n.first);
  }
  if(n.second != "0" && n.second.find("GND") == std::string::npos) {
    if(nm.count(n.second) == 0) Errors::netlist_errors(NetlistErrors::NO_SUCH_NODE, n.second);
  }
  if(n.second.find("GND") != std::string::npos || n.second == "0") {
    // 0 0
    if(n.first.find("GND") != std::string::npos || n.first == "0") {
      Errors::invalid_component_errors(ComponentErrors::GROUNDED_VOLTAGE_SOURCE, s);
    // 1 0
    } else {
      nonZeros_.emplace_back(1);
      rowPointer_.emplace_back(1);
      branchIndex++;
      columnIndex_.emplace_back(nm.at(n.first));
    }
  // 0 1
  } else if(n.first.find("GND") != std::string::npos || n.first == "0") {
      nonZeros_.emplace_back(-1);
      rowPointer_.emplace_back(1);
      branchIndex++;
      columnIndex_.emplace_back(nm.at(n.second));
  // 1 1
  } else {
    nonZeros_.emplace_back(1);
    nonZeros_.emplace_back(-1);
    rowPointer_.emplace_back(2);
    branchIndex++;
    columnIndex_.emplace_back(nm.at(n.first));
    columnIndex_.emplace_back(nm.at(n.second));
  }
}

void PhaseSource::set_indices(const std::pair<std::string, std::string> &n, const std::unordered_map<std::string, int> &nm, std::vector<std::vector<std::pair<double, int>>> &nc, const int &branchIndex) {
  if(n.second.find("GND") != std::string::npos || n.second == "0") {
    posIndex_ = nm.at(n.first);
    nc.at(nm.at(n.first)).emplace_back(std::make_pair(1, branchIndex - 1));
  } else if(n.first.find("GND") != std::string::npos || n.first == "0") {
    negIndex_ = nm.at(n.second);
    nc.at(nm.at(n.second)).emplace_back(std::make_pair(-1, branchIndex - 1));
  } else {
    posIndex_ = nm.at(n.first);
    negIndex_ = nm.at(n.second);
    nc.at(nm.at(n.first)).emplace_back(std::make_pair(1, branchIndex - 1));
    nc.at(nm.at(n.second)).emplace_back(std::make_pair(-1, branchIndex - 1));
  }
}