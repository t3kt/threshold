//
//  AppParameters.cpp
//  threshold
//
//  Created by tekt on 1/8/15.
//
//

#include "AppParameters.h"

#include <math.h>

#include "Parameters.h"

ThreshAppParameters::ThreshAppParameters() {
  paramGroup.add(hasMinDist.set("Min Dist?", false));
  paramGroup.add(hasMaxDist.set("Max Dist?", true));
  paramGroup.add(distRange.set("Distance Range",
                               ofVec2f(0, .03),
                               ofVec2f(0.000000001, 0.000000001),
                               ofVec2f(.4, .4)));
  paramGroup.add(maxLines.set("Max Lines", 1000, 1, 10000));
  paramGroup.add(hasMaxPerSource.set("Max Per Source?", false));
  paramGroup.add(maxLinesPerSource.set("Max Lines Per Source", 10, 1, 100));
  hasMinDist.enableEvents();
  hasMaxDist.enableEvents();
  distRange.enableEvents();
  maxLines.enableEvents();
  hasMaxPerSource.enableEvents();
  maxLinesPerSource.enableEvents();
  
  paramGroup.add(numPoints.set("Points", 800, 10, 2000));
  paramGroup.add(pointOpacity.set("Point Opacity", .6, 0, 1));
  paramGroup.add(pointSize.set("Point Size", 0.005, 0.0001, 0.02));
  
  ofParameterGroup postParams;
  postParams.setName("Post-Processing");
  postParams.add(postParams);
  paramGroup.add(enableBloom.set("Bloom?", false));
  paramGroup.add(enableKaliedoscope.set("Kaleidoscope?", false));
  paramGroup.add(kaliedoscopeSegments.set("Kaleidoscope Segments", 3., 0., 12.));
}

void ThreshAppParameters::applyTo(ThreshParameters &params) const {
  ofVec2f dist = distRange.get();
  dist = ofVec2f(min(dist[0], dist[1]), max(dist[0], dist[1]));
  params.minDist = hasMinDist.get() ? dist[0] : -1;
  params.maxDist = hasMaxDist.get() ? dist[1] : -1;
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
