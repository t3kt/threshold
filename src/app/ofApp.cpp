#include "ofApp.h"

#include <math.h>

#include "Parameters.h"
#include "PointSet.h"
#include "Common.h"
#include "Logging.h"
#include "AppCommon.h"
#include "FieldPointSystem.h"
#include "MeshPointSystem.h"

static shared_ptr<PointSystem>
createFieldSystem1(ThreshAppParameters& appParams) {
  auto sys = new FieldPointSystem(appParams,
                                  appParams.pointColor1,
                                  appParams.pointColor2);
  return shared_ptr<PointSystem>(sys);
}

static shared_ptr<PointSystem>
createFieldSystem2(ThreshAppParameters& appParams) {
  auto sys = new FieldPointSystem(appParams,
                                  appParams.pointColor3,
                                  appParams.pointColor4);
  return shared_ptr<PointSystem>(sys);
}

static shared_ptr<PointSystem>
createBoxSystem1(ThreshAppParameters& appParams) {
  auto box = ofBoxPrimitive(0.5, 0.7, 0.4, 5, 4, 3);
  auto sys = new MeshPointSystem(appParams,
                                 box.getMesh(),
                                 appParams.pointColor1,
                                 appParams.pointColor2);
  sys->spinRate.set(.03, .008, .01);
  return shared_ptr<PointSystem>(sys);
}

static shared_ptr<PointSystem>
createSphereSystem2(ThreshAppParameters& appParams) {
  auto sphere = ofSpherePrimitive(0.5, 12);
  auto sys = new MeshPointSystem(appParams,
                                 sphere.getMesh(), // intentional copying
                                 appParams.pointColor3,
                                 appParams.pointColor4);
  sys->spinRate.set(-.03, .01, -.005);
  return shared_ptr<PointSystem>(sys);
}

void ofApp::setup() {
  ofSetWindowTitle("Threshold");
#ifdef USING_SYPHON
  _syphonServer.setName("Threshold Output");
#endif
  _threshParams.maxLines = _appParams.numPoints.get();
  _threshParams.minDist = 0;
  _threshParams.maxDist = 0.05f;
  _threshParams.maxLines = 10000;
  _thresholder.configure(_threshParams);
  _appParams.readFrom(_threshParams);
  _changingSystem1 = true;
  _changingSystem2 = true;
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
  _appParams.usePrimitive1.addListener(this,
                                       &ofApp::onUsePrimitive1Changed);
  _appParams.usePrimitive2.addListener(this,
                                       &ofApp::onUsePrimitive2Changed);
  _paramsChanged = true;
  _postProc.init(ofGetWidth(), ofGetHeight());
  _bloom = _postProc.createPass<BloomPass>();
  _kaleidoscope = _postProc.createPass<KaleidoscopePass>();
}

void ofApp::onUsePrimitive1Changed(bool &) {
  _changingSystem1 = true;
}

void ofApp::onUsePrimitive2Changed(bool &) {
  _changingSystem2 = true;
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
  if (_changingSystem1) {
    if (_appParams.usePrimitive1.get())
      _pointSystem = createBoxSystem1(_appParams);
    else
      _pointSystem = createFieldSystem1(_appParams);
    _changingSystem1 = false;
  }
  if (_changingSystem2) {
    if (_appParams.usePrimitive2.get())
      _pointSystem2 = createSphereSystem2(_appParams);
    else
      _pointSystem2 = createFieldSystem2(_appParams);
    _changingSystem2 = false;
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

static void extendPoints(ofVec3f& pos1, ofVec3f& pos2,
                         float extend1, float extend2) {
  auto diff = pos2 - pos1;
  pos1 = pos1 + (diff * extend1);
  pos2 = pos2 - (diff * extend2);
}

void ofApp::addLineToMesh(ofMesh *mesh,
                          const ThreshLine &line) const {
  ofVec3f pos1 = _pointSystem->getPosition(line.startIndex);
  ofFloatColor color1 = _pointSystem->getColor(line.startIndex);
  
  ofVec3f pos2;
  ofFloatColor color2;
  if (_appParams.useSeparateSource.get()) {
    pos2 = _pointSystem2->getPosition(line.endIndex);
    color2 = _pointSystem2->getColor(line.endIndex);
  } else {
    pos2 = _pointSystem->getPosition(line.endIndex);
    color2 = _pointSystem->getColor(line.endIndex);
  }
  if (_appParams.useExtend.get()) {
    extendPoints(pos1, pos2,
                 _appParams.extendRatioStart.get(),
                 _appParams.extendRatioEnd.get());
  }
//  color1.a *= line.closeness;
//  color2.a *= line.closeness;
  color1.a = color2.a = line.closeness;
  //...
  mesh->addVertex(pos1);
  mesh->addColor(color1);
  mesh->addVertex(pos2);
  mesh->addColor(color2);
}

void ofApp::drawLines() const {
  ofPushStyle();
  ofSetLineWidth(_appParams.lineWidth.get());
  ofMesh linesMesh;
  linesMesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& line : _threshLines) {
    addLineToMesh(&linesMesh, line);
  }
  ofNoFill();
  linesMesh.drawWireframe();
  ofPopStyle();
}

void ofApp::draw() {
  ofClear(0);
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
    if (_pointSystem2 && _appParams.useSeparateSource.get())
      _pointSystem2->draw();
  }
  
  if (_drawThreshLines) {
    drawLines();
  }
  
  ofPopMatrix();
  _postProc.end();
  glPopAttrib();
#ifdef USING_SYPHON
  _syphonServer.publishScreen();
#endif
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
