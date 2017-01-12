//
//  FieldPointSystem.cpp
//  threshold
//
//  Created by tekt on 1/11/15.
//
//

#include "FieldPointSystem.h"

#include <ofMain.h>

#include "AppCommon.h"

FieldPointSystem::FieldPointSystem(ThreshAppParameters& appParams,
                                   ofParameter<ofFloatColor>& color1,
                                   ofParameter<ofFloatColor>& color2)
: _appParams(appParams)
, _color1(color1), _color2(color2) {
  color1.addListener(this,
                     &FieldPointSystem::onPointColorChanged);
  color2.addListener(this,
                     &FieldPointSystem::onPointColorChanged);
  assignPointColors();
}

FieldPointSystem::~FieldPointSystem() {
  _color1.removeListener(this,
                     &FieldPointSystem::onPointColorChanged);
  _color2.removeListener(this,
                     &FieldPointSystem::onPointColorChanged);
}

void FieldPointSystem::onPointColorChanged(ofFloatColor &) {
  assignPointColors();
}

void FieldPointSystem::assignPointColors() {
  const auto& color1 = _color1.get();
  const auto& color2 = _color2.get();
  for (int i = 0; i < _points.size(); i++) {
    _points[i].color = (i % 2 == 1) ? color2 : color1;
  }
}

void FieldPointSystem::update() {
  auto count = _appParams.numPoints.get();
  if (count < _points.size()) {
    _points.resize(count);
    _pointNoiseOffsets.resize(count);
  } else if (count > _points.size()) {
    for (int i = _points.size(); i < count; ++i) {
      auto noisePos = createRandomVec3f(1007000.342f);
      auto pos = createSignedNoiseVec3f(-noisePos);
      AppPoint pt;
      pt.position = pos;
      pt.index = i;
      _points.push_back(pt);
      _pointNoiseOffsets.push_back(noisePos);
    }
    assignPointColors();
  }
  auto time = ofGetElapsedTimef();
  auto pointStep = ofVec3f(0.02f);
  for (int i = 0; i < count; i++) {
    auto& point = _points[i];
    auto noisePos = _pointNoiseOffsets[i] + time * 0.3f;
    auto position = point.position;
    position += createSignedNoiseVec3f(noisePos) * pointStep;
    position = wrapVec(position, -1, 1);
    point.position = position;
  }
}

void FieldPointSystem::draw() {
  ofPushStyle();
  ofFill();
  float radius = _appParams.pointSize.get();
  float opacity = _appParams.pointOpacity.get();
  auto numPoints = _points.size();
  for (int i = 0; i < numPoints; i++) {
    const auto& vertex = _points[i];
    auto color = vertex.color;
    color.a = opacity;
    ofSetColor(color);
    ofDrawSphere(vertex.position, radius);
  }
  ofPopStyle();
}

int FieldPointSystem::size() const {
  return _points.size();
}

AppPoint FieldPointSystem::getPoint(int i) const {
  return _points[i];
}
