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
#include "AppCommon.h"

std::ostream& operator<<(std::ostream& os, const ThreshPoint& pt) {
  os << "Point{#" << pt.index;
  os << ", (" << pt.x << ", " << pt.y << ", " << pt.z << ")";
  os << ", color:(" << pt.r << ", " << pt.g << ", " << pt.b
                    << ", " << pt.a << ")";
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
