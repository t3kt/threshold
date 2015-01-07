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
: _start(&start), _end(&end)
, _squareDist(start.distanceSquared(end)) {

}

ThreshLine::ThreshLine(const ThreshLine& other)
: _start(other._start), _end(other._end)
, _squareDist(other._squareDist) {
  
}
