//
//  Line.cpp
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#include "Line.h"

std::ostream& operator<<(std::ostream& os, const ThreshLine& ln) {
  os << "Line{start:" << ln.start
     << ", end:" << ln.end
     << ", sqdist:" << ln.squareDistance
     << ", closeness:" << ln.closeness
     << "}";
  return os;
}
