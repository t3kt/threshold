//
//  LineRenderer.cpp
//  threshold
//
//  Created by tekt on 1/19/15.
//
//

#include "LineRenderer.h"

#include <ofMain.h>

LineRenderer::LineRenderer(ThreshAppState& state)
: _state(state) { }

void LineRenderer::draw() {
  ofPushStyle();
  ofMesh mesh;
  mesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& line : _state.lines) {
    ThreshPoint pt1;
    ThreshPoint pt2;
    if (_state.getLinePoints(line, &pt1, &pt2)) {
      mesh.addVertex(pt1.position);
      mesh.addColor(pt1.color);
      mesh.addVertex(pt2.position);
      mesh.addColor(pt2.color);
    }
  }
  ofSetLineWidth(_state.appParams.lineWidth.get());
  ofNoFill();
  mesh.drawWireframe();
  ofPopStyle();
}
