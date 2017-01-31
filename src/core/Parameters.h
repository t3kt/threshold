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
  bool useSeparateSource;
  bool distinctGroups;
  
  ThreshParameters();
  
  inline bool hasMinDist() const { return minDist >= 0; }
  inline bool hasMaxDist() const { return maxDist >= 0; }
  inline bool hasMaxLinesPerSource() const {
    return maxLinesPerSource > 0;
  }
};

#endif /* defined(__threshold__Parameters__) */
