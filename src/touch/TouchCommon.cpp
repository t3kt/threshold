//
//  TouchCommon.cpp
//  threshold
//
//  Created by tekt on 1/9/15.
//
//

#include "TouchCommon.h"

#include "ChopCPP_wrapper.h"

int CHOPInputPointSet::size() const  { return _inputs->numSamples; }

TouchPoint CHOPInputPointSet::getPoint(int i) const {
  TouchPoint point;
  point.index = i;
  point.position = getPosition(i);
  point.group = getGroup(i);
  //color ... probably going to remove that...?
  return point;
}

ThVec3f CHOPInputPointSet::getPosition(int i) const {
  ThVec3f position;
  position.x = _inputs->getChannelData(_xInputIndex)[i];
  position.y = _inputs->getChannelData(_yInputIndex)[i];
  position.z = _inputs->getChannelData(_zInputIndex)[i];
  return position;
}

int CHOPInputPointSet::getGroup(int i) const {
  if (_groupInputIndex < 0 || _groupInputIndex >= _inputs->numChannels) {
    return 0;
  }
  return static_cast<int>(_inputs->getChannelData(_groupInputIndex)[i]);
}
