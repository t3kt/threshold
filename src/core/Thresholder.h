//
//  Thresholder.h
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#ifndef __threshold__Thresholder__
#define __threshold__Thresholder__

#include <vector>

#include "LineSet.h"

class Thresholder {
public:
  void generate(std::vector<ThreshPoint>& points);
private:
  LineSet _lines;
};

#endif /* defined(__threshold__Thresholder__) */
