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
  _state.params.maxLines = _state.appParams.numPoints.get();
  _state.params.minDist = 0;
  _state.params.maxDist = 0.05f;
  _state.params.maxLines = 10000;
  _thresholder.configure(_state.params);
  _state.appParams.readFrom(_state.params);
  _lineRenderer.reset(new LineRenderer(_state));
  _changingSystem1 = true;
  _changingSystem2 = true;
  _drawInputPoints = true;
  _drawThreshLines = true;
  _cam.setAutoDistance(true);
  ofEnableAlphaBlending();
  ofEnableAntiAliasing();
  ofEnableSmoothing();
  _gui.setup(_state.appParams.paramGroup);
  auto& appParams = _state.appParams;
  ofAddListener(appParams.paramGroup.parameterChangedE(),
                this, &ofApp::onParameterChanged);
  appParams.hasMinDist.addListener(this,
                                    &ofApp::onTypedParameterChanged<bool>);
  appParams.hasMaxDist.addListener(this,
                                    &ofApp::onTypedParameterChanged<bool>);
  appParams.distRange.addListener(this,
                                   &ofApp::onTypedParameterChanged<ofVec2f>);
  appParams.maxLines.addListener(this,
                                  &ofApp::onTypedParameterChanged<int>);
  appParams.hasMaxPerSource.addListener(this,
                                         &ofApp::onTypedParameterChanged<bool>);
  appParams.maxLinesPerSource.addListener(this,
                                           &ofApp::onTypedParameterChanged<int>);
  appParams.useSeparateSource.addListener(this,
                                           &ofApp::onTypedParameterChanged<bool>);
  appParams.usePrimitive1.addListener(this,
                                       &ofApp::onUsePrimitive1Changed);
  appParams.usePrimitive2.addListener(this,
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
  _state.appParams.applyTo(_state.params);
  _paramsChanged = true;
}

void ofApp::update() {
  if (_paramsChanged) {
    _thresholder.configure(_state.params);
    _paramsChanged = false;
  }
  if (_changingSystem1) {
    if (_state.appParams.usePrimitive1.get())
      _pointSystem = createBoxSystem1(_state.appParams);
    else
      _pointSystem = createFieldSystem1(_state.appParams);
    _state.points1 = _pointSystem.get();
    _changingSystem1 = false;
  }
  if (_changingSystem2) {
    if (_state.appParams.usePrimitive2.get())
      _pointSystem2 = createSphereSystem2(_state.appParams);
    else
      _pointSystem2 = createFieldSystem2(_state.appParams);
    _state.points2 = _pointSystem2.get();
    _changingSystem2 = false;
  }
  _state.lines.clear();
  if (_pointSystem2) {
    _pointSystem2->update();
  }
  if (_pointSystem) {
    _pointSystem->update();
    _thresholder.generate(_pointSystem.get(), _pointSystem2.get(),
                          &_state.lines);
  }
  _bloom->setEnabled(_state.appParams.enableBloom.get());
  _kaleidoscope->setEnabled(_state.appParams.enableKaliedoscope.get());
  _kaleidoscope->setSegments(_state.appParams.kaliedoscopeSegments.get());
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
    if (_pointSystem2 && _state.appParams.useSeparateSource.get())
      _pointSystem2->draw();
  }
  
  if (_drawThreshLines && _lineRenderer) {
    _lineRenderer->draw();
  }
  
  ofPopMatrix();
  _postProc.end();
  glPopAttrib();
#ifdef USING_SYPHON
  _syphonServer.publishScreen();
#endif
  if (_pointSystem && _state.appParams.showDebugInfo.get()) {
    ofVec2f position;
    position.x = 10;
    position.y = winSize.y - 100;
    ofDrawBitmapString("Points: " +
                       ofToString(_pointSystem->size()),
                       position);
    position.y -= 40;
    ofDrawBitmapString("Lines: " +
                       ofToString(_state.lines.size()),
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
  ofLogNotice() << "THRESH LINES:\n" << _state.lines;
}
