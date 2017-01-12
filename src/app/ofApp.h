#pragma once

#ifdef TARGET_OSX
#define USING_SYPHON
#endif

#include <ofMain.h>
#include <ofxGui.h>
#include <ofxPostProcessing.h>
#include <vector>

#ifdef USING_SYPHON
#include <ofxSyphon.h>
#endif

#include "Thresholder.h"
#include "LineSet.h"
#include "PointSet.h"
#include "Parameters.h"
#include "AppParameters.h"
#include "PointSystem.h"
#include "AppState.h"
#include "LineRenderer.h"

using AppThresholder = Thresholder<AppPoint, AppPointSystem>;

class ofApp : public ofBaseApp {
public:
  void setup() override;
  void update() override;
  void draw() override;

  void keyPressed(int key) override;
  
private:
  void dumpToLog() const;
  void updateParameters();
  void onParameterChanged(ofAbstractParameter&);
  template<typename T>
  void onTypedParameterChanged(T&) {
    updateParameters();
  }
  void onUsePrimitive1Changed(bool&);
  void onUsePrimitive2Changed(bool&);
  
  ThreshAppState _state;
  bool _drawInputPoints;
  bool _drawThreshLines;
  bool _paramsChanged;
  shared_ptr<AppPointSystem> _pointSystem;
  shared_ptr<AppPointSystem> _pointSystem2;
  AppThresholder _thresholder;
  shared_ptr<LineRenderer> _lineRenderer;
  ofxPanel _gui;
  ofEasyCam _cam;
  ofxPostProcessing _postProc;
  shared_ptr<BloomPass> _bloom;
  shared_ptr<KaleidoscopePass> _kaleidoscope;
  bool _changingSystem1;
  bool _changingSystem2;
#ifdef USING_SYPHON
  ofxSyphonServer _syphonServer;
#endif
};
