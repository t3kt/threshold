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

std::ostream& operator<<(std::ostream& os, const AppPoint& pt) {
  os << "Point{#" << pt.index;
  os << ", (" << pt.position.x <<
        ", " << pt.position.y <<
        ", " << pt.position.z << ")";
  os << ", color:(" << pt.color.r
            << ", " << pt.color.g
            << ", " << pt.color.b
            << ", " << pt.color.a << ")";
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const ThreshLine& ln) {
  os << "Line{start:#" << ln.startIndex
  << ", end:#" << ln.endIndex
  << ", sqdist:" << ln.squareDistance
  << ", closeness:" << ln.closeness
  << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const AppPointSource& points) {
  outputSet(os, points, "PointSet");
  return os << std::endl;
}

std::ostream& operator<<(std::ostream& os, const LineSet& lines) {
  outputSet(os, lines, "LineSet");
  return os << std::endl;
}
