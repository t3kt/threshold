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
  ThreshLine(const ThreshPoint& start,
             const ThreshPoint& end);
  
  const ThreshPoint& start() const { return _start; }
  const ThreshPoint& end() const { return _end; }
  float squareDistance() const { return _squareDist; }
private:
  float _squareDist;
  const ThreshPoint& _start;
  const ThreshPoint& _end;
};

#endif /* defined(__threshold__Line__) */
