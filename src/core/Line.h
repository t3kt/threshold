//
//  Line.h
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#ifndef __threshold__Line__
#define __threshold__Line__

#include "Point.h"

class ThreshLine {
public:
  const ThreshPoint* start;
  const ThreshPoint* end;
  float squareDistance;
  float closeness;
};

#endif /* defined(__threshold__Line__) */
