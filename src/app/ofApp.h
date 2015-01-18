#pragma once

#include <ofMain.h>
#include <ofxGui.h>
#include <ofxPostProcessing.h>
#include <vector>

#include "Thresholder.h"
#include "LineSet.h"
#include "PointSet.h"
#include "Parameters.h"
#include "AppParameters.h"
#include "PointSystem.h"

class ofApp : public ofBaseApp {
public:
  void setup();
  void update();
  void draw();

  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y );
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);
  
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
  
  bool _drawInputPoints;
  bool _drawThreshLines;
  ThreshAppParameters _appParams;
  ThreshParameters _threshParams;
  bool _paramsChanged;
  shared_ptr<PointSystem> _pointSystem;
  shared_ptr<PointSystem> _pointSystem2;
  Thresholder _thresholder;
  LineSet _threshLines;
  ofxPanel _gui;
  ofEasyCam _cam;
  ofxPostProcessing _postProc;
  shared_ptr<BloomPass> _bloom;
  shared_ptr<KaleidoscopePass> _kaleidoscope;
  bool _changingSystem1;
  bool _changingSystem2;
};
