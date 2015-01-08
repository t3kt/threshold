//
//  Point.h
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#ifndef __threshold__Point__
#define __threshold__Point__

#include <iostream>
#include <ofVec3f.h>
#include <ofColor.h>

class ThreshPoint : public ofVec3f {
public:
  ThreshPoint();
  explicit ThreshPoint(const ofVec3f& position, int i);
  
  int index;
  ofFloatColor color;
};

std::ostream& operator<<(std::ostream& os, const ThreshPoint& pt);

#endif /* defined(__threshold__Point__) */
