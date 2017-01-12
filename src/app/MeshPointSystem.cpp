//
//  MeshPointSystem.cpp
//  threshold
//
//  Created by tekt on 1/17/15.
//
//

#include "AppCommon.h"
#include "MeshPointSystem.h"
#include <ofMain.h>
#include <glm/gtx/rotate_vector.hpp>

MeshPointSystem::MeshPointSystem(ThreshAppParameters& appParams,
                                 ofMesh mesh,
                                 ofParameter<ofFloatColor>& color1,
                                 ofParameter<ofFloatColor>& color2)
: _appParams(appParams), _mesh(mesh)
, _color1(color1), _color2(color2) {
  color1.addListener(this,
                     &MeshPointSystem::onPointColorChanged);
  color2.addListener(this,
                     &MeshPointSystem::onPointColorChanged);
  assignPointColors();
}

MeshPointSystem::~MeshPointSystem() {
  _color1.removeListener(this,
                         &MeshPointSystem::onPointColorChanged);
  _color2.removeListener(this,
                         &MeshPointSystem::onPointColorChanged);
}

void MeshPointSystem::onPointColorChanged(ofFloatColor &) {
  assignPointColors();
}

void MeshPointSystem::assignPointColors() {
  const auto& color1 = _color1.get();
  const auto& color2 = _color2.get();
  auto numPoints = size();
  _mesh.enableColors();
  _mesh.clearColors();
  for (int i = 0; i < numPoints; i++) {
    _mesh.addColor((i % 2 == 1) ? color2 : color1);
  }
}

void MeshPointSystem::update() {
  auto time = ofGetElapsedTimef();
  auto rotation = time * spinRate;
  auto numPoints = _mesh.getNumVertices();
  for (int i = 0; i < numPoints; ++i) {
    ofVec3f pt = _mesh.getVertex(i);
    pt.rotate(rotation.x, rotation.y, rotation.z);
    _mesh.setVertex(i, pt);
  }
}

void MeshPointSystem::draw() {
  ofPushStyle();
  ofFill();
  float radius = _appParams.pointSize.get();
  float opacity = _appParams.pointOpacity.get();
  auto numPoints = _mesh.getNumVertices();
  for (int i = 0; i < numPoints; i++) {
    auto position = _mesh.getVertex(i);
    auto color = _mesh.getColor(i);
    color.a = opacity;
    ofSetColor(color);
    ofDrawSphere(position, radius);
  }
  ofPopStyle();
}
int MeshPointSystem::size() const {
  return _mesh.getNumVertices();
}

ThreshPoint MeshPointSystem::operator[](int i) const {
  ThreshPoint pt;
  pt.index = i;
  pt.position = toThVec(_mesh.getVertex(i));
  if (_mesh.hasColors())
    pt.color = toThColor(_mesh.getColor(i));
  return pt;
}
