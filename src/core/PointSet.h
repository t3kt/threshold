//
//  PointSet.h
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#ifndef __threshold__PointSet__
#define __threshold__PointSet__

#include <vector>

#include "Point.h"

class PointSource {
public:
  virtual int size() const = 0;
  virtual ThreshPoint operator[](int i) const = 0;
  virtual ThVec3f getPosition(int i) const;
  virtual ThColor getColor(int i) const;
};

class PointSet
: public std::vector<ThreshPoint>
, public PointSource {
public:
  int size() const override;
  ThreshPoint operator[](int i) const override;
};

#endif /* defined(__threshold__PointSet__) */
