//
//  TouchCommon.h
//

#pragma once

#include "Common.h"
#include "Point.h"
#include "PointSet.h"

using TouchPoint = ThreshPoint<ThVec3f, ThColor>;
using TouchPointSource = PointSource<TouchPoint>;

class OP_CHOPInput;

class CHOPInputPointSet : public TouchPointSource {
public:
  CHOPInputPointSet(const OP_CHOPInput* inputs,
                    int xI, int yI, int zI)
  : _inputs(inputs)
  , _xInputIndex(xI), _yInputIndex(yI) , _zInputIndex(zI) { }
  
  int size() const override;
  PointT getPoint(int i) const override;
  VecT getPosition(int i) const override;
  //  ColorT getColor(int i) const override;
  
  //  int rInputIndex;
  //  int gInputIndex;
  //  int bInputIndex;
  //  int aInputIndex;
private:
  int _xInputIndex;
  int _yInputIndex;
  int _zInputIndex;
  const OP_CHOPInput* _inputs;
};

