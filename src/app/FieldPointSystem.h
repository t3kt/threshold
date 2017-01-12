//
//  FieldPointSystem.h
//

#pragma once

#include "PointSystem.h"
#include "AppParameters.h"
#include "AppCommon.h"

class FieldPointSystem : public AppPointSystem {
public:
  FieldPointSystem(ThreshAppParameters& appParams,
                   ofParameter<ofFloatColor>& color1,
                   ofParameter<ofFloatColor>& color2);
  ~FieldPointSystem();
  void update() override;
  void draw() override;
  int size() const override;
  PointT operator[](int i) const override;
private:
  void onPointColorChanged(ofFloatColor&);
  void assignPointColors();
  
  ThreshAppParameters& _appParams;
  ofParameter<ofFloatColor>& _color1;
  ofParameter<ofFloatColor>& _color2;
  std::vector<PointT> _points;
  std::vector<ofVec3f> _pointNoiseOffsets;
};
