//
//  Line.cpp
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#include "Line.h"

ThreshLine::ThreshLine(const ThreshPoint& start,
                       const ThreshPoint& end)
: _start(start), _end(end)
, _squareDist(start.distanceSquared(end)) {

}
