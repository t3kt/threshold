//
//  FieldPointSystem.h
//  threshold
//
//  Created by tekt on 1/11/15.
//
//

#ifndef __threshold__FieldPointSystem__
#define __threshold__FieldPointSystem__

#include "PointSystem.h"
#include "AppParameters.h"

class FieldPointSystem : public PointSystem {
public:
  FieldPointSystem(ThreshAppParameters& appParams,
                   ofParameter<ofFloatColor>& color1,
                   ofParameter<ofFloatColor>& color2);
  ~FieldPointSystem();
  void update() override;
  void draw() override;
  int size() const override;
  ThreshPoint operator[](int i) const override;
private:
  void onPointColorChanged(ofFloatColor&);
  void assignPointColors();
  
  ThreshAppParameters& _appParams;
  ofParameter<ofFloatColor>& _color1;
  ofParameter<ofFloatColor>& _color2;
  std::vector<ThreshPoint> _points;
  std::vector<ofVec3f> _pointNoiseOffsets;
};

#endif /* defined(__threshold__FieldPointSystem__) */
