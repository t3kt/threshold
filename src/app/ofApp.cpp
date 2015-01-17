#include "ofApp.h"

#include <math.h>

#include "Parameters.h"
#include "PointSet.h"
#include "Common.h"
#include "Logging.h"
#include "AppCommon.h"
#include "FieldPointSystem.h"
#include "PrimitivePointSystem.h"

void ofApp::setup() {
  _threshParams.maxLines = _appParams.numPoints.get();
  _threshParams.minDist = 0;
  _threshParams.maxDist = 0.05f;
  _threshParams.maxLines = 10000;
  _thresholder.configure(_threshParams);
  _appParams.readFrom(_threshParams);
  _pointSystem.reset(new FieldPointSystem(_appParams,
                                          _appParams.pointColor1,
                                          _appParams.pointColor2));
  {
//    shared_ptr<ofBoxPrimitive> box(new ofBoxPrimitive(0.5, 0.7, 0.4, 5, 4, 3));
//    box->enableColors();
//    for (int i = 0; i < box->getMesh().getNumVertices(); ++i) {
//      box->getMesh().addColor(ofFloatColor::blue);
//    }
//    _pointSystem2.reset(new PrimitivePointSystem(_appParams,
//                                                box));
  }
  _pointSystem2.reset(new FieldPointSystem(_appParams,
                                          _appParams.pointColor3,
                                          _appParams.pointColor4));
  {
//    shared_ptr<ofSpherePrimitive> sphere(new ofSpherePrimitive(0.5, 4));
//    sphere->enableColors();
//    for (int i = 0; i < sphere->getMesh().getNumVertices(); ++i) {
//      sphere->getMesh().addColor(ofFloatColor::red);
//    }
//    auto sys = new PrimitivePointSystem(_appParams, sphere);
////    sys->setSpinRate(ofVec3f(10, 17, 12));
//    _pointSystem2.reset(sys);
  }
  _drawInputPoints = true;
  _drawThreshLines = true;
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
  _appParams.useSeparateSource.addListener(this,
                                           &ofApp::onTypedParameterChanged<bool>);
  _paramsChanged = true;
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
  _threshLines.clear();
  if (_pointSystem2) {
    _pointSystem2->update();
  }
  if (_pointSystem) {
    _pointSystem->update();
    _thresholder.generate(_pointSystem.get(), _pointSystem2.get(),
                          &_threshLines);
  }
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
    if (_pointSystem)
      _pointSystem->draw();
    if (_pointSystem2)
      _pointSystem2->draw();
  }
  
  if (_drawThreshLines) {
    ofPushStyle();
    ofSetLineWidth(_appParams.lineWidth.get());
    ofMesh linesMesh;
    linesMesh.setMode(OF_PRIMITIVE_LINES);
    for (const auto& line : _threshLines) {
      auto alpha = line.closeness;
      auto color1 = _pointSystem->getColor(line.startIndex);
      ofFloatColor color2;
      if (_appParams.useSeparateSource.get()) {
        color2 = _pointSystem2->getColor(line.endIndex);
      } else {
        color2 = _pointSystem->getColor(line.endIndex);
      }
      color1.a = color2.a = alpha;
      linesMesh.addVertex(_pointSystem->getPosition(line.startIndex));
      linesMesh.addColor(color1);
      linesMesh.addVertex(_pointSystem->getPosition(line.endIndex));
      linesMesh.addColor(color2);
    }
    ofNoFill();
    linesMesh.drawWireframe();
    ofPopStyle();
  }
  
  ofPopMatrix();
  _postProc.end();
  glPopAttrib();
  if (_pointSystem && _appParams.showDebugInfo.get()) {
    ofVec2f position;
    position.x = 10;
    position.y = winSize.y - 100;
    ofDrawBitmapString("Points: " +
                       ofToString(_pointSystem->size()),
                       position);
    position.y -= 40;
    ofDrawBitmapString("Lines: " +
                       ofToString(_threshLines.size()),
                       position);
    position.y -= 40;
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()),
                       position);
  }
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
  if (_pointSystem) {
    ofLogNotice() << "INPUT POINTS:\n" << *_pointSystem;
  }
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
