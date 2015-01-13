//
//  PointSet.cpp
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#include "PointSet.h"

ThVec3f PointSource::getPosition(int i) const {
  return (*this)[i].position;
}

ThColor PointSource::getColor(int i) const {
  return (*this)[i].color;
}

int PointSet::size() const {
  return static_cast<int>(std::vector<ThreshPoint>::size());
}

ThreshPoint PointSet::operator[](int i) const {
  return std::vector<ThreshPoint>::operator[](i);
}
