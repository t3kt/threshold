//
//  PrimitivePointSystem.cpp
//  threshold
//
//  Created by tekt on 1/15/15.
//
//

#include "PrimitivePointSystem.h"

PrimitivePointSystem::PrimitivePointSystem(shared_ptr<of3dPrimitive> prim)
: _primitive(prim) {
  
}

PrimitivePointSystem::~PrimitivePointSystem() {
}

void PrimitivePointSystem::update() {
  if (!_primitive)
    return;
//  ofVec3f rotation;
//  auto time = ofGetElapsedTimef();
//  rotation.x = time * 10;
//  rotation.y = time * 17;
//  rotation.z = time * 12;
//  _primitive->setOrientation(rotation);
}

void PrimitivePointSystem::draw() {
  if (!_primitive)
    return;
  ofPushStyle();
  ofSetLineWidth(0.5);
  _primitive->drawWireframe();
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
