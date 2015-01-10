//
//  Point.cpp
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#include "Point.h"

ThreshPoint::ThreshPoint() { }

ThreshPoint::ThreshPoint(const ofVec3f& pos, int i)
: position(pos), index(i) { }
