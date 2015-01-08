#include "ofApp.h"

#include <math.h>

#include "Parameters.h"
#include "PointSet.h"

void ofApp::setup() {
  int numPoints = 400;
  _pointsMesh.setMode(OF_PRIMITIVE_POINTS);
  for (int i = 0; i < numPoints; i++) {
    ofVec3f noisePos(ofRandom(939.4910),
                     ofRandom(8284.01),
                     ofRandom(-12562.23));
    ofVec3f pos;
    pos.x = ofSignedNoise(-noisePos.x);
    pos.y = ofSignedNoise(-noisePos.y);
    pos.z = ofSignedNoise(-noisePos.z);
    ThreshPoint pt(pos, i);
    pt.color = i % 2 == 1
      ? ofFloatColor(0, .4, .7)
      : ofFloatColor(0, .9, .2);
    _inputPoints.push_back(pt);
    _pointsMesh.addVertex(pos);
    _pointNoiseOffsets.push_back(noisePos);
  }
  _threshParams.maxLines = numPoints;
  _threshParams.minDist = 0;
  _threshParams.maxDist = 0.1;
  _threshParams.maxLines = 10000;
  _thresholder.configure(_threshParams);
  _drawInputPoints = true;
  _drawThreshLines = true;
  _cam.setTarget(ofVec3f::zero());
  _cam.setAutoDistance(true);
  ofEnableAlphaBlending();
}

void ofApp::update() {
  float time = ofGetElapsedTimef();
  auto width = 2;
  auto height = 2;
  auto pointStep = ofVec3f(0.02);
  auto numPoints = _inputPoints.size();
  for (int i = 0; i < numPoints; i++) {
    auto& point = _inputPoints[i];
    auto noisePos = _pointNoiseOffsets[i] + time * 0.3;
    point.x += ofSignedNoise(noisePos.x) * pointStep.x;
    point.y += ofSignedNoise(noisePos.y) * pointStep.y;
    point.z += ofSignedNoise(noisePos.z) * pointStep.z;
    point.x = ofWrap(point.x, -1, 1);
    point.y = ofWrap(point.y, -1, 1);
    point.z = ofWrap(point.z, -1, 1);
    _pointsMesh.setVertex(i, point);
  }
  _threshLines.clear();
  _thresholder.generate(_inputPoints, &_threshLines);
}

void ofApp::draw() {
  ofBackground(0);
  _cam.begin();
  ofPushMatrix();
  auto winSize = ofGetWindowSize();
  auto size = ::min(winSize.x, winSize.y) / 2;
  ofScale(size, size, size);
  
  if (_drawInputPoints) {
    ofPushStyle();
    ofSetColor(0, 0, 255);
    ofFill();
    float radius = .005;
    auto numPoints = _inputPoints.size();
    for (int i = 0; i < numPoints; i++) {
      const auto& vertex = _inputPoints[i];
      ofSetColor(vertex.color);
      ofDrawSphere(vertex, radius);
    }
    ofPopStyle();
  }
  
  if (_drawThreshLines) {
    ofPushStyle();
    ofMesh linesMesh;
    linesMesh.setMode(OF_PRIMITIVE_LINES);
    for (const auto& line : _threshLines) {
      auto alpha = line.closeness;
      auto color1 = line.start->color;
      auto color2 = line.end->color;
      color1.a = color2.a = alpha;
      linesMesh.addVertex(*line.start);
      linesMesh.addColor(color1);
      linesMesh.addVertex(*line.end);
      linesMesh.addColor(color2);
    }
    ofNoFill();
    linesMesh.drawWireframe();
    ofPopStyle();
  }
  
  {
    ofPushStyle();
    ofNoFill();
    ofSetColor(255, 0, 255, 63);
    ofDrawBox(1);
    ofPopStyle();
  }
  
  ofPopMatrix();
  _cam.end();
}

void ofApp::keyPressed(int key) {
  if (key == 'i') {
    _drawInputPoints = !_drawInputPoints;
  } else if (key == 'l') {
    _drawThreshLines = !_drawThreshLines;
  } else if (key == 'd') {
    dumpToLog();
  }
}

void ofApp::dumpToLog() const {
  ofLogNotice() << "INPUT POINTS:\n" << _inputPoints;
  ofLogNotice() << "THRESH LINES:\n" << _threshLines;
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
