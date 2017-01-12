//
//  PointSystem.h
//

#pragma once

#include "Point.h"
#include "PointSet.h"

template<typename TPoint>
class PointSystem : public PointSource<TPoint> {
public:
  virtual void update() = 0;
  virtual void draw() = 0;
};
