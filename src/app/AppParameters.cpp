//
//  AppParameters.cpp
//  threshold
//
//  Created by tekt on 1/8/15.
//
//

#include "AppParameters.h"
#include <float.h>
#include <limits.h>

#include "Parameters.h"

ThreshAppParameters::ThreshAppParameters() {
  paramGroup.add(hasMinDist.set("Min Dist?", false));
  paramGroup.add(hasMaxDist.set("Max Dist?", true));
  paramGroup.add(distRange.set("Distance Range",
                               ofVec2f(0, .5),
                               ofVec2f::zero(),
                               ofVec2f(FLT_MAX, FLT_MAX)));
  paramGroup.add(maxLines.set("Max Lines", 1000, 0, INT_MAX));
  paramGroup.add(hasMaxPerSource.set("Max Per Source?", false));
  paramGroup.add(maxLinesPerSource.set("Max Lines Per Source", 10));
  hasMinDist.enableEvents();
  hasMaxDist.enableEvents();
  distRange.enableEvents();
  maxLines.enableEvents();
  hasMaxPerSource.enableEvents();
  maxLinesPerSource.enableEvents();
  hasMinDist.setParent(&paramGroup);
  hasMaxDist.setParent(&paramGroup);
  distRange.setParent(&paramGroup);
  maxLines.setParent(&paramGroup);
  hasMaxPerSource.setParent(&paramGroup);
  maxLinesPerSource.setParent(&paramGroup);
}

void ThreshAppParameters::applyTo(ThreshParameters &params) const {
  params.minDist = hasMinDist.get() ? distRange.get()[0] : -1;
  params.maxDist = hasMaxDist.get() ? distRange.get()[1] : -1;
  params.maxLines = maxLines.get();
  params.maxLinesPerSource = hasMaxPerSource.get() ? maxLinesPerSource.get() : -1;
}
