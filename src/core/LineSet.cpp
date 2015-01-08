//
//  LineSet.cpp
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#include "LineSet.h"
#include "Common.h"

std::ostream& operator<<(std::ostream& os, const LineSet& lines) {
  outputSet(os, lines, "LineSet");
//  auto size = lines.size();
//  os << "LineSet{size:" << size << "\n";
//  for (auto i = 0; i < size; i++) {
//    const auto& ln = lines[i];
//    os << "\t[" << i << "] " << ln << "\n";
//  }
//  os << "}" << std::endl;
  return os << std::endl;
}
