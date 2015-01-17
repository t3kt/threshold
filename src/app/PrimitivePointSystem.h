//
//  PrimitivePointSystem.h
//  threshold
//
//  Created by tekt on 1/15/15.
//
//

#ifndef __threshold__PrimitivePointSystem__
#define __threshold__PrimitivePointSystem__

#include <ofMain.h>

#include "PointSystem.h"
#include "AppParameters.h"

class PrimitivePointSystem : public PointSystem {
public:
  PrimitivePointSystem(ThreshAppParameters& appParams, shared_ptr<of3dPrimitive> prim);
  ~PrimitivePointSystem();
  PrimitivePointSystem& setSpinRate(ofVec3f spin);
  void update() override;
  void draw() override;
  int size() const override;
  ThreshPoint operator[](int i) const override;
private:
  ofVec3f _spinRate;
  ThreshAppParameters& _appParams;
  shared_ptr<of3dPrimitive> _primitive;
};

#endif /* defined(__threshold__PrimitivePointSystem__) */