#include "ofApp.h"

#include <math.h>

#include "Parameters.h"
#include "PointSet.h"
#include "Common.h"
#include "Logging.h"
#include "AppCommon.h"

void ofApp::setup() {
  int numPoints = 400;
  _pointsMesh.setMode(OF_PRIMITIVE_POINTS);
  for (int i = 0; i < numPoints; i++) {
    auto noisePos = createRandomVec3f(1007000.342f);
    auto pos = createSignedNoiseVec3f(-noisePos);
    ThreshPoint pt;
    pt.position = pos;
    pt.index = i;
    pt.color = (i % 2 == 1)
                ? ofFloatColor(0, .4f, .7f)
                : ofFloatColor(0, .9f, .2f);
    _inputPoints.push_back(pt);
    _pointsMesh.addVertex(pos);
    _pointNoiseOffsets.push_back(noisePos);
  }
  _threshParams.maxLines = numPoints;
  _threshParams.minDist = 0;
  _threshParams.maxDist = 0.1f;
  _threshParams.maxLines = 10000;
  _thresholder.configure(_threshParams);
  _drawInputPoints = true;
  _drawThreshLines = true;
  _cam.setTarget(ofVec3f::zero());
  _cam.setAutoDistance(true);
  ofEnableAlphaBlending();
  _gui.setup(_appParams.paramGroup);
  ofAddListener(_appParams.paramGroup.parameterChangedE,
                this, &ofApp::onParameterChanged);
  _appParams.hasMinDist.addListener(this,
                                    &ofApp::onTypedParameterChanged<bool>);
  _appParams.hasMaxDist.addListener(this,
                                    &ofApp::onTypedParameterChanged<bool>);
  _appParams.distRange.addListener(this,
                                   &ofApp::onTypedParameterChanged<ofVec2f>);
  _appParams.maxLines.addListener(this,
                                  &ofApp::onTypedParameterChanged<int>);
  _appParams.hasMaxPerSource.addListener(this,
                                         &ofApp::onTypedParameterChanged<bool>);
  _appParams.maxLinesPerSource.addListener(this,
                                           &ofApp::onTypedParameterChanged<int>);
  _paramsChanged = true;
  _appParams.readFrom(_threshParams);
  _postProc.init(ofGetWidth(), ofGetHeight());
  _bloom = _postProc.createPass<BloomPass>();
  _kaleidoscope = _postProc.createPass<KaleidoscopePass>();
}

void ofApp::onParameterChanged(ofAbstractParameter&) {
  updateParameters();
}

void ofApp::updateParameters() {
  _appParams.applyTo(_threshParams);
  _paramsChanged = true;
}

void ofApp::update() {
  if (_paramsChanged) {
    _thresholder.configure(_threshParams);
    _paramsChanged = false;
  }
  auto time = ofGetElapsedTimef();
  auto pointStep = ofVec3f(0.02f);
  auto numPoints = _inputPoints.size();
  for (int i = 0; i < numPoints; i++) {
    auto& point = _inputPoints[i];
    auto noisePos = _pointNoiseOffsets[i] + time * 0.3f;
    ofVec3f position = point.position;
    position += createSignedNoiseVec3f(noisePos) * pointStep;
    position = wrapVec(position, -1, 1);
    _pointsMesh.setVertex(i, position);
    point.position = position;
  }
  _threshLines.clear();
  _thresholder.generate(_inputPoints, &_threshLines);
  _bloom->setEnabled(_appParams.enableBloom.get());
  _kaleidoscope->setEnabled(_appParams.enableKaliedoscope.get());
  _kaleidoscope->setSegments(_appParams.kaliedoscopeSegments.get());
}

void ofApp::draw() {
  ofBackground(0);
  glPushAttrib(GL_ENABLE_BIT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  _postProc.begin(_cam);
  ofPushMatrix();
  auto winSize = ofGetWindowSize();
  auto size = ::min(winSize.x, winSize.y) / 2;
  ofScale(size, size, size);
  
  if (_drawInputPoints) {
    ofPushStyle();
    ofFill();
    float radius = _appParams.pointSize.get();
    float opacity = _appParams.pointOpacity.get();
    auto numPoints = _inputPoints.size();
    for (int i = 0; i < numPoints; i++) {
      const auto& vertex = _inputPoints[i];
      ofFloatColor color = vertex.color;
      color.a = opacity;
      ofSetColor(color);
      ofDrawSphere(vertex.position, radius);
    }
    ofPopStyle();
  }
  
  if (_drawThreshLines) {
    ofPushStyle();
    ofMesh linesMesh;
    linesMesh.setMode(OF_PRIMITIVE_LINES);
    for (const auto& line : _threshLines) {
      auto alpha = line.closeness;
      auto color1 = line.start.color;
      auto color2 = line.end.color;
      color1.a = color2.a = alpha;
      linesMesh.addVertex(line.start.position);
      linesMesh.addColor(color1);
      linesMesh.addVertex(line.end.position);
      linesMesh.addColor(color2);
    }
    ofNoFill();
    linesMesh.drawWireframe();
    ofPopStyle();
  }
  
  ofPopMatrix();
  _postProc.end();
  glPopAttrib();
  _gui.draw();
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
