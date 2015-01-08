//
//  Line.h
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#ifndef __threshold__Line__
#define __threshold__Line__

#include <iostream>

#include "Point.h"

struct ThreshLine {
  const ThreshPoint* start;
  const ThreshPoint* end;
  float squareDistance;
  float closeness;
};

std::ostream& operator<<(std::ostream& os, const ThreshLine& ln);

#endif /* defined(__threshold__Line__) */
