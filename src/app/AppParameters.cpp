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
  paramGroup.add(maxLines.set("Max Lines", 1000, 1, 20000));
  paramGroup.add(hasMaxPerSource.set("Max Per Source?", false));
  paramGroup.add(maxLinesPerSource.set("Max Lines Per Source", 10, 1, 100));
  paramGroup.add(useSeparateSource.set("Use Separate Source?", false));
  hasMinDist.enableEvents();
  hasMaxDist.enableEvents();
  distRange.enableEvents();
  maxLines.enableEvents();
  hasMaxPerSource.enableEvents();
  maxLinesPerSource.enableEvents();
  useSeparateSource.enableEvents();
  
  paramGroup.add(numPoints.set("Points", 800, 10, 2000));
  paramGroup.add(pointOpacity.set("Point Opacity", .6, 0, 1));
  paramGroup.add(pointSize.set("Point Size", 0.005, 0.0001, 0.02));
  paramGroup.add(pointColor1.set("Point Color 1",
                                 ofFloatColor(0, .4f, .7f),
                                 ofFloatColor(0, 0, 0, 0),
                                 ofFloatColor(1, 1, 1, 1)));
  paramGroup.add(pointColor2.set("Point Color 2",
                                 ofFloatColor(0, .9f, .2f),
                                 ofFloatColor(0, 0, 0, 0),
                                 ofFloatColor(1, 1, 1, 1)));
  paramGroup.add(lineWidth.set("Line Width", .2, 0.001, 5));
  
  ofParameterGroup postParams;
  postParams.setName("Post-Processing");
  postParams.add(postParams);
  paramGroup.add(enableBloom.set("Bloom?", false));
  paramGroup.add(enableKaliedoscope.set("Kaleidoscope?", false));
  paramGroup.add(kaliedoscopeSegments.set("Kaleidoscope Segments", 3., 0., 12.));
  paramGroup.add(showDebugInfo.set("Show Debug Info?", false));
}

void ThreshAppParameters::applyTo(ThreshParameters &params) const {
  ofVec2f dist = distRange.get();
  dist = ofVec2f(min(dist[0], dist[1]), max(dist[0], dist[1]));
  params.minDist = hasMinDist.get() ? dist[0] : -1;
  params.maxDist = hasMaxDist.get() ? dist[1] : -1;
  params.maxLines = maxLines.get();
  params.maxLinesPerSource = hasMaxPerSource.get() ? maxLinesPerSource.get() : -1;
  params.useSeparateSource = useSeparateSource.get();
}

void ThreshAppParameters::readFrom(const ThreshParameters &params) {
  hasMinDist.set(params.hasMinDist());
  hasMaxDist.set(params.hasMaxDist());
  distRange.set(ofVec2f(params.hasMinDist() ? params.minDist : 0,
                        params.hasMaxDist() ? params.maxDist : 100));
  maxLines.set(params.maxLines);
  hasMaxPerSource.set(params.hasMaxLinesPerSource());
  maxLinesPerSource.set(params.hasMaxLinesPerSource() ? params.maxLinesPerSource : 1);
  useSeparateSource.set(params.useSeparateSource);
}
