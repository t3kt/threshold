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
  
  ofParameter<float> pointOpacity;
  ofParameter<float> pointSize;
  
  ofParameterGroup paramGroup;
};

#endif /* defined(__threshold__ThreshAppParameters__) */
