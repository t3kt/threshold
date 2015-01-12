//
//  Thresholder.h
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#ifndef __threshold__Thresholder__
#define __threshold__Thresholder__

#include "Common.h"
#include "Parameters.h"
#include "LineSet.h"
#include "PointSet.h"

class Thresholder {
public:
  void configure(const ThreshParameters& params);
  void generate(const PointSource& points, LineSet* lines);
private:
  ThreshLine createLine(const ThreshPoint& start,
                        const ThreshPoint& end);
  bool testLine(const ThreshLine& line);
  
  ThreshParameters _params;
};

#endif /* defined(__threshold__Thresholder__) */
