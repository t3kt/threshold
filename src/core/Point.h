//
//  Point.h
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#ifndef __threshold__Point__
#define __threshold__Point__

#include <ofVec3f.h>

class ThreshPoint : public ofVec3f {
public:
  ThreshPoint();
  explicit ThreshPoint(const ofVec3f& position, int i);
  
  int index;
};

#endif /* defined(__threshold__Point__) */
