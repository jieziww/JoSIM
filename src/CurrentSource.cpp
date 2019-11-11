// Copyright (c) 2019 Johannes Delport
// This code is licensed under MIT license (see LICENSE for details)

#include "JoSIM/CurrentSource.hpp"
#include "JoSIM/Misc.hpp"
#include "JoSIM/Errors.hpp"

CurrentSource CurrentSource::create_currentsource(
    const std::pair<std::string, std::string> &s,
    const std::unordered_map<std::string, int> &nm) {
  std::vector<std::string> tokens = Misc::tokenize_space(s.first);

  CurrentSource temp;
  temp.set_label(tokens.at(0));
  temp.set_indices(std::make_pair(tokens.at(1), tokens.at(2)), nm);
  return temp;
}

void CurrentSource::set_indices(const std::pair<std::string, std::string> &n, const std::unordered_map<std::string, int> &nm) {
  if(n.second.find("GND") != std::string::npos || n.second == "0") {
    posIndex_ = nm.at(n.first);
  } else if(n.first.find("GND") != std::string::npos || n.first == "0") {
    negIndex_ = nm.at(n.second);
  } else {
    posIndex_ = nm.at(n.first);
    negIndex_ = nm.at(n.second);
  }
}
