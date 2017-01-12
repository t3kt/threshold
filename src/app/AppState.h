//
//  AppState.h
//  threshold
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __threshold__AppState__
#define __threshold__AppState__

#include "AppParameters.h"
#include "Common.h"
#include "Parameters.h"
#include "PointSet.h"
#include "LineSet.h"

class ThreshAppState {
public:
  
  bool getLinePointsRaw(const ThreshLine& line,
                        ThreshPoint* pt1,
                        ThreshPoint* pt2) const;
  
  bool getLinePoints(const ThreshLine& line,
                     ThreshPoint* pt1,
                     ThreshPoint* pt2) const;
  
  ThreshAppParameters appParams;
  ThreshParameters params;
  PointSource* points1;
  PointSource* points2;
  LineSet lines;
};

#endif /* defined(__threshold__AppState__) */
