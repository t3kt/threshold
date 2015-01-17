//
//  PrimitivePointSystem.cpp
//  threshold
//
//  Created by tekt on 1/15/15.
//
//

#include "PrimitivePointSystem.h"

PrimitivePointSystem::PrimitivePointSystem(ThreshAppParameters& appParams,
                                           shared_ptr<of3dPrimitive> prim)
: _appParams(appParams), _primitive(prim) {
  
}

PrimitivePointSystem::~PrimitivePointSystem() {
}

PrimitivePointSystem& PrimitivePointSystem::setSpinRate(ofVec3f spin) {
  _spinRate = spin;
  return *this;
}

void PrimitivePointSystem::update() {
  if (!_primitive)
    return;
  auto time = ofGetElapsedTimef();
  ofVec3f rotation = time * _spinRate;
  _primitive->setOrientation(rotation);
  auto mesh = _primitive->getMesh();
  auto matrix = _primitive->getLocalTransformMatrix();
  for (int i = 0; i < mesh.getNumVertices(); ++i) {
    auto pt = mesh.getVertex(i);
    pt.rotate(rotation.x, rotation.y, rotation.z);
    mesh.setVertex(i, pt);
  }
}

void PrimitivePointSystem::draw() {
  if (!_primitive)
    return;
  ofPushStyle();
  ofSetLineWidth(0.5);
  _primitive->drawWireframe();
  ofPopStyle();
  ofPushStyle();
  ofFill();
  float radius = _appParams.pointSize.get();
  float opacity = _appParams.pointOpacity.get();
  const auto& mesh = _primitive->getMesh();
  auto numPoints = mesh.getNumVertices();
  for (int i = 0; i < numPoints; i++) {
    auto position = mesh.getVertex(i);
    auto color = mesh.getColor(i);
    color.a = opacity;
    ofSetColor(color);
    ofDrawSphere(position, radius);
  }
  ofPopStyle();
}

int PrimitivePointSystem::size() const {
  if (_primitive) {
    return _primitive->getMesh().getNumVertices();
  }
  return 0;
}

ThreshPoint PrimitivePointSystem::operator[](int i) const {
  ThreshPoint point;
  point.index = i;
  if (_primitive) {
    const auto& mesh = _primitive->getMesh();
    point.position = mesh.getVertex(i);
    if (mesh.hasColors())
      point.color = mesh.getColor(i);
  }
  return point;
}
