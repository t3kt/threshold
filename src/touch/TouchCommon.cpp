//
//  TouchCommon.cpp
//  threshold
//
//  Created by tekt on 1/9/15.
//
//

#include "TouchCommon.h"

#include "ChopCPP_wrapper.h"

int CHOPInputPointSet::size() const  { return _inputs->length; }

ThreshPoint CHOPInputPointSet::operator[](int i) const {
  ThreshPoint point;
  point.index = i;
  point.position.x = _inputs->channels[_xInputIndex][i];
  point.position.y = _inputs->channels[_yInputIndex][i];
  point.position.z = _inputs->channels[_zInputIndex][i];
  //color ... probably going to remove that...?
  return point;
}

ThVec3f CHOPInputPointSet::getPosition(int i) const {
  ThVec3f position;
  position.x = _inputs->channels[_xInputIndex][i];
  position.y = _inputs->channels[_yInputIndex][i];
  position.z = _inputs->channels[_zInputIndex][i];
  return position;
}
