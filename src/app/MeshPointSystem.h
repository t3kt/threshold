//
//  MeshPointSystem.h
//  threshold
//
//  Created by tekt on 1/17/15.
//
//

#ifndef __threshold__MeshPointSystem__
#define __threshold__MeshPointSystem__

#include <ofMesh.h>

#include "PointSystem.h"
#include "AppParameters.h"

class MeshPointSystem : public PointSystem {
public:
  MeshPointSystem(ThreshAppParameters& appParams,
                  ofMesh mesh,
                  ofParameter<ofFloatColor>& color1,
                  ofParameter<ofFloatColor>& color2);
  ~MeshPointSystem();
  void update() override;
  void draw() override;
  int size() const override;
  ThreshPoint operator[](int i) const override;
  ofVec3f spinRate;
private:
  void onPointColorChanged(ofFloatColor&);
  void assignPointColors();
  
  ThreshAppParameters& _appParams;
  ofParameter<ofFloatColor>& _color1;
  ofParameter<ofFloatColor>& _color2;
  ofMesh _mesh;
};

#endif /* defined(__threshold__MeshPointSystem__) */
