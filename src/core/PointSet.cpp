//
//  PointSet.cpp
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#include "PointSet.h"

int PointSet::size() const {
  return std::vector<ThreshPoint>::size();
}

ThreshPoint PointSet::operator[](int i) const {
  return (*this)[i];
}
