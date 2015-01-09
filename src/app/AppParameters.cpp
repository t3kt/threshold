//
//  AppParameters.cpp
//  threshold
//
//  Created by tekt on 1/8/15.
//
//

#include "AppParameters.h"

#include "Parameters.h"

ThreshAppParameters::ThreshAppParameters() {
  paramGroup.add(hasMinDist.set("Min Dist?", false));
  paramGroup.add(hasMaxDist.set("Max Dist?", true));
  paramGroup.add(distRange.set("Distance Range",
                               ofVec2f(0, .5),
                               ofVec2f::zero(),
                               ofVec2f(30, 30)));
  paramGroup.add(maxLines.set("Max Lines", 1000, 0, 50000));
  paramGroup.add(hasMaxPerSource.set("Max Per Source?", false));
  paramGroup.add(maxLinesPerSource.set("Max Lines Per Source", 10, 0, 100));
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

void ThreshAppParameters::readFrom(const ThreshParameters &params) {
  hasMinDist.set(params.hasMinDist());
  hasMaxDist.set(params.hasMaxDist());
  distRange.set(ofVec2f(params.hasMinDist() ? params.minDist : 0,
                        params.hasMaxDist() ? params.maxDist : 100));
  maxLines.set(params.maxLines);
  hasMaxPerSource.set(params.hasMaxLinesPerSource());
  maxLinesPerSource.set(params.hasMaxLinesPerSource() ? params.maxLinesPerSource : 1);
}
