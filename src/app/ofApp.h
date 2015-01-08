#pragma once

#include <ofMain.h>
#include "Thresholder.h"
#include "LineSet.h"
#include "PointSet.h"

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
  
  bool _drawInputPoints;
  bool _drawThreshLines;
  ofMesh _pointsMesh;
  Thresholder _thresholder;
  PointSet _inputPoints;
  LineSet _threshLines;
  ofEasyCam _cam;
};
