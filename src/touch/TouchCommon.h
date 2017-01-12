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

class OP_CHOPInput;

class CHOPInputPointSet : public PointSource {
public:
  CHOPInputPointSet(const OP_CHOPInput* inputs,
                    int xI, int yI, int zI)
  : _inputs(inputs)
  , _xInputIndex(xI), _yInputIndex(yI) , _zInputIndex(zI) { }
  
  int size() const override;
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
  const OP_CHOPInput* _inputs;
};

#endif /* defined(__threshold__TouchCommon__) */
