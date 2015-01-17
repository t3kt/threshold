//
//  AppParameters.h
//  threshold
//
//  Created by tekt on 1/8/15.
//
//

#ifndef __threshold__ThreshAppParameters__
#define __threshold__ThreshAppParameters__

#include <ofParameter.h>
#include <ofParameterGroup.h>
#include <ofVec2f.h>
#include <ofColor.h>

class ThreshParameters;

class ThreshAppParameters {
public:
  ThreshAppParameters();
  
  void applyTo(ThreshParameters& params) const;
  void readFrom(const ThreshParameters& params);
  
  ofParameter<bool> hasMinDist;
  ofParameter<bool> hasMaxDist;
  ofParameter<ofVec2f> distRange;
  ofParameter<int> maxLines;
  ofParameter<bool> hasMaxPerSource;
  ofParameter<int> maxLinesPerSource;
  ofParameter<bool> useSeparateSource;
  
  ofParameter<int> numPoints;
  ofParameter<float> pointOpacity;
  ofParameter<float> pointSize;
  ofParameter<ofFloatColor> pointColor1;
  ofParameter<ofFloatColor> pointColor2;
  ofParameter<ofFloatColor> pointColor3;
  ofParameter<ofFloatColor> pointColor4;
  ofParameter<float> lineWidth;
  ofParameter<bool> usePrimitive1;
  ofParameter<bool> usePrimitive2;
  
  ofParameter<bool> enableBloom;
  ofParameter<bool> enableKaliedoscope;
  ofParameter<float> kaliedoscopeSegments;
  ofParameter<bool> showDebugInfo;
  
  ofParameterGroup paramGroup;
};

#endif /* defined(__threshold__ThreshAppParameters__) */
