//
//  Logging.cpp
//  threshold
//
//  Created by tekt on 1/9/15.
//
//

#include "Logging.h"

#include "Point.h"
#include "Line.h"
#include "PointSet.h"
#include "LineSet.h"
#include "Common.h"

std::ostream& operator<<(std::ostream& os, const ThreshPoint& pt) {
  os << "Point{#" << pt.index;
  const auto& pos = pt.position;
  os << ", (" << pos.x << ", " << pos.y << ", " << pos.z << ")";
  os << ", color:" << pt.color;
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const ThreshLine& ln) {
  os << "Line{start:" << ln.start
  << ", end:" << ln.end
  << ", sqdist:" << ln.squareDistance
  << ", closeness:" << ln.closeness
  << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const PointSet& points) {
  outputSet(os, points, "PointSet");
  return os << std::endl;
}

std::ostream& operator<<(std::ostream& os, const LineSet& lines) {
  outputSet(os, lines, "LineSet");
  return os << std::endl;
}
