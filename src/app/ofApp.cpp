#include "ofApp.h"
#include "Parameters.h"
#include "PointSet.h"

void ofApp::setup() {
  auto width = ofGetWidth();
  auto height = ofGetHeight();
  int numPoints = 10;
  _pointsMesh.setMode(OF_PRIMITIVE_POINTS);
  for (int i = 0; i < numPoints; i++) {
    ofVec3f pos;
    pos.x = ofMap(ofSignedNoise(i), -1, 1, 0, width);
    pos.y = ofMap(ofSignedNoise(i + numPoints), -1, 1, 0, height);
//    pos.x = ofSignedNoise(i);
//    pos.y = ofSignedNoise(i + numPoints * 1000);
    _pointsMesh.addVertex(pos);
  }
  ThreshParameters params;
  params.maxLines = numPoints;
  params.minDist = 0;
  params.maxLines = 50;
  _thresholder.configure(params);
  _drawInputPoints = true;
  _drawThreshLines = true;
  _cam.move(0, 0, -5);
  _cam.lookAt(ofVec3f::zero());
}

void ofApp::update() {
  float time = ofGetElapsedTimef();
  auto width = ofGetWidth();
  auto height = ofGetHeight();
  auto pointStep = ofVec2f(width * 0.02, height * 0.02);
//  auto pointStep = ofVec3f(0.02);
  auto numPoints = _pointsMesh.getNumVertices();
  PointSet inputPoints;
  for (int i = 0; i < numPoints; i++) {
    ThreshPoint point(_pointsMesh.getVertex(i), i);
    float noisePosition = time + i * 200;
    point.x += ofSignedNoise(noisePosition) * pointStep.x;
    point.y += ofSignedNoise(noisePosition + numPoints * 400) * pointStep.y;
    point.x = ofWrap(point.x, 0, width);
    point.y = ofWrap(point.y, 0, height);
    _pointsMesh.setVertex(i, point);
    inputPoints.push_back(point);
  }
  _threshLines.clear();
  _thresholder.generate(inputPoints, &_threshLines);
}

void ofApp::draw() {
//  _cam.begin();
  ofPushMatrix();
  auto size = ofVec2f(ofGetWidth(), ofGetHeight());
  
  if (_drawInputPoints) {
    ofPushStyle();
    ofSetColor(0, 0, 255);
    ofFill();
//    _pointsMesh.draw();
    float radius = 2;
    for (int i = 0; i < _pointsMesh.getNumVertices(); i++) {
//      ofDrawSphere(_pointsMesh.getVertex(i), radius);
      auto vertex = _pointsMesh.getVertex(i);
      ofCircle(vertex.x,
               vertex.y,
               radius);
    }
    ofPopStyle();
  }
  
  if (_drawThreshLines) {
    ofPushStyle();
    ofMesh linesMesh;
    linesMesh.setMode(OF_PRIMITIVE_LINES);
    for (const auto& line : _threshLines) {
      linesMesh.addVertex(line.start());
      linesMesh.addVertex(line.end());
    }
    linesMesh.draw();
    ofPopStyle();
  }
  
  ofPopMatrix();
//  _cam.end();
}

void ofApp::keyPressed(int key) {
  if (key == 'i') {
    _drawInputPoints = !_drawInputPoints;
  } else if (key == 'l') {
    _drawThreshLines = !_drawThreshLines;
  }
}

void ofApp::keyReleased(int key) {

}

void ofApp::mouseMoved(int x, int y ) {

}

void ofApp::mouseDragged(int x, int y, int button) {

}

void ofApp::mousePressed(int x, int y, int button) {

}

void ofApp::mouseReleased(int x, int y, int button) {

}

void ofApp::windowResized(int w, int h) {

}

void ofApp::gotMessage(ofMessage msg) {

}

void ofApp::dragEvent(ofDragInfo dragInfo) {

}
