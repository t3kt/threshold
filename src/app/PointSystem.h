//
//  PointSystem.h
//  threshold
//
//  Created by tekt on 1/11/15.
//
//

#ifndef __threshold__PointSystem__
#define __threshold__PointSystem__

#include "Point.h"
#include "PointSet.h"

class PointSystem : public PointSource {
public:
  virtual void update() = 0;
  virtual void draw() = 0;
};

#endif /* defined(__threshold__PointSystem__) */
