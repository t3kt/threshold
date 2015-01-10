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

class ThreshPoint {
public:
  ThreshPoint();
  ThreshPoint(const ofVec3f& pos, int i);
  
  int index;
  ofVec3f position;
  ofFloatColor color;
};

#endif /* defined(__threshold__Point__) */
