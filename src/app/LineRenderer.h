//
//  LineRenderer.h
//

#pragma once

#include "AppState.h"

class LineRenderer {
public:
  LineRenderer(ThreshAppState& state);
  
  void draw();
private:
  ThreshAppState& _state;
};

