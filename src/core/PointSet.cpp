//
//  PointSet.cpp
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#include "PointSet.h"
#include "Common.h"

std::ostream& operator<<(std::ostream& os, const PointSet& points) {
  outputSet(os, points, "PointSet");
  return os << std::endl;
}
