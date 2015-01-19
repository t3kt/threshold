//
//  LineRenderer.h
//  threshold
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __threshold__LineRenderer__
#define __threshold__LineRenderer__

#include "AppState.h"

class LineRenderer {
public:
  LineRenderer(ThreshAppState& state);
  
  void draw();
private:
  ThreshAppState& _state;
};

#endif /* defined(__threshold__LineRenderer__) */
