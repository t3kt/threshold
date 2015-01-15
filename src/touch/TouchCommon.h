//
//  TouchCommon.h
//  threshold
//
//  Created by tekt on 1/9/15.
//
//

#ifndef __threshold__TouchCommon__
#define __threshold__TouchCommon__

#include "PointSet.h"
#include "ChopCPP_wrapper.h"

class CHOPInputPointSet : public PointSource {
public:
  CHOPInputPointSet(const CHOP_CHOPInput* inputs,
                    int xI, int yI, int zI)
  : _inputs(inputs)
  , _xInputIndex(xI), _yInputIndex(yI) , _zInputIndex(zI) { }
  
  int size() const override { return _inputs->length; }
  ThreshPoint operator[](int i) const override;
  ThVec3f getPosition(int i) const override;
  //  ThColor getColor(int i) const override;
  
  //  int rInputIndex;
  //  int gInputIndex;
  //  int bInputIndex;
  //  int aInputIndex;
private:
  int _xInputIndex;
  int _yInputIndex;
  int _zInputIndex;
  const CHOP_CHOPInput* _inputs;
};

ThreshPoint CHOPInputPointSet::operator[](int i) const {
  ThreshPoint point;
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


#endif /* defined(__threshold__TouchCommon__) */
