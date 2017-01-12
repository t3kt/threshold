//
//  MeshPointSystem.h
//

#pragma once

#include <ofMesh.h>

#include "PointSystem.h"
#include "AppParameters.h"
#include "AppCommon.h"

class MeshPointSystem : public AppPointSystem {
public:
  MeshPointSystem(ThreshAppParameters& appParams,
                  ofMesh mesh,
                  ofParameter<ofFloatColor>& color1,
                  ofParameter<ofFloatColor>& color2);
  ~MeshPointSystem();
  void update() override;
  void draw() override;
  int size() const override;
  AppPoint operator[](int i) const override;
  ofVec3f spinRate;
private:
  void onPointColorChanged(ofFloatColor&);
  void assignPointColors();
  
  ThreshAppParameters& _appParams;
  ofParameter<ofFloatColor>& _color1;
  ofParameter<ofFloatColor>& _color2;
  ofMesh _mesh;
};

