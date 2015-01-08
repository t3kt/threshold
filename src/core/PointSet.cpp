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
//  auto size = points.size();
//  os << "PointSet{size:" << size << "\n";
//  for (auto i = 0; i < size; i++) {
//    const auto& pt = points[i];
//    os << "\t[" << i << "] " << pt << "\n";
//  }
//  os << "}" << std::endl;
  return os << std::endl;
}
