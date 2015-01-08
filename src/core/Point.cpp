//
//  Point.cpp
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#include "Point.h"

ThreshPoint::ThreshPoint() { }

ThreshPoint::ThreshPoint(const ofVec3f& pos, int i)
: position(pos), index(i) { }

std::ostream& operator<<(std::ostream& os, const ThreshPoint& pt) {
  os << "Point{#" << pt.index;
  const auto& pos = pt.position;
  os << ", (" << pos.x << ", " << pos.y << ", " << pos.z << ")";
  os << ", color:" << pt.color;
  os << "}";
  return os;
}
