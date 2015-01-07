//
//  Parameters.h
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#ifndef __threshold__Parameters__
#define __threshold__Parameters__

class ThreshParameters {
public:
  float minDist;
  float maxDist;
  int maxLines;
  int maxLinesPerSource;
  
  ThreshParameters();
  
  bool hasMinDist() const { return minDist >= 0; }
  bool hasMaxDist() const { return maxDist >= 0; }
  bool hasMaxLinesPerSource() const {
    return maxLinesPerSource > 0;
  }
};

#endif /* defined(__threshold__Parameters__) */
