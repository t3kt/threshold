#pragma once

#include <ofMain.h>
#include <ofxGui.h>
#include <ofxVboParticles.h>
#include <vector>

#include "Thresholder.h"
#include "LineSet.h"
#include "PointSet.h"
#include "Parameters.h"
#include "AppParameters.h"

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
  
  bool _drawInputPoints;
  bool _drawThreshLines;
  ThreshAppParameters _appParams;
  ThreshParameters _threshParams;
  bool _paramsChanged;
  ofMesh _pointsMesh;
  Thresholder _thresholder;
  PointSet _inputPoints;
  LineSet _threshLines;
  std::vector<ofVec3f> _pointNoiseOffsets;
  ofEasyCam _cam;
  ofxPanel _gui;
  ofxVboParticles *_particles;
};
