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
  return os << std::endl;
}
