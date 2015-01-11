//
//  Thresholder.cpp
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#include <math.h>
#include <float.h>

#include "Thresholder.h"

void Thresholder::configure(const ThreshParameters &params) {
  _params = params;
}

void Thresholder::generate(const PointSet &points,
                           LineSet *lines) {
  for (int indexA = 0; indexA < points.size(); indexA++) {
    const auto& pointA = points[indexA];
    int linesFromPointA = 0;
    for (int indexB = indexA + 1; indexB < points.size(); indexB++) {
      const auto& pointB = points[indexB];
      auto line = createLine(pointA, pointB);
      if (testLine(line)) {
        lines->push_back(line);
        linesFromPointA++;
        if (_params.hasMaxLinesPerSource() &&
            linesFromPointA >= _params.maxLinesPerSource) {
          break;
        }
        if (lines->size() >= _params.maxLines) {
          break;
        }
      }
      if (lines->size() >= _params.maxLines) {
        break;
      }
    }
  }
}

static float distanceSquared(float x1, float y1, float z1,
                             float x2, float y2, float z2) {
  float vx = x2 - x1;
  float vy = y2 - y1;
  float vz = z2 - z1;
  return vx*vx + vy*vy + vz*vz;
}
static float mapRange(float value, float inputMin, float inputMax,
                      float outputMin, float outputMax) {
  
  if (fabs(inputMin - inputMax) < FLT_EPSILON){
    return outputMin;
  } else {
    float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
    return outVal;
  }
}

ThreshLine Thresholder::createLine(const ThreshPoint &start,
                                   const ThreshPoint &end) {
  ThreshLine line;
  line.start = start;
  line.end = end;
  line.squareDistance = distanceSquared(start.x, start.y, start.z,
                                        end.x, end.y, end.z);
  line.closeness = 0;
  if (_params.hasMaxDist()) {
    auto minDist = _params.hasMinDist() ? _params.minDist : 0;
    auto maxDist = _params.maxDist;
    if (fabs(minDist - maxDist) < FLT_EPSILON ||
        minDist >= maxDist) {
      line.closeness = 1;
    } else {
      line.closeness = mapRange(line.squareDistance,
                                minDist, maxDist, 1.0f, 0.0f);
    }
  }
  return line;
}

bool Thresholder::testLine(const ThreshLine &line) {
  auto dist = line.squareDistance;
  if (_params.hasMinDist() && dist < _params.minDist) {
    return false;
  }
  if (_params.hasMaxDist() && dist > _params.maxDist) {
    return false;
  }
  return true;
}

LineSet Thresholder::generate(const PointSet &points) {
  LineSet lines;
  generate(points, &lines);
  return lines;
}


