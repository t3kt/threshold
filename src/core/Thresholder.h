//
//  Thresholder.h
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#ifndef __threshold__Thresholder__
#define __threshold__Thresholder__

#include <ofTypes.h>

class ThreshParameters;
class ThreshPoint;
class ThreshLine;
class LineSet;
class PointSet;

class ThresholderImpl;

class Thresholder {
public:
  void configure(const ThreshParameters& params);
  LineSet generate(const PointSet& points);
private:
  ofPtr<ThresholderImpl> _impl;
};

#endif /* defined(__threshold__Thresholder__) */
