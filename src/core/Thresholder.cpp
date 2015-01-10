//
//  Thresholder.cpp
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#include <vector>
#include <ofMath.h>
#include <math.h>
#include <float.h>

#include "Thresholder.h"
#include "Parameters.h"
#include "LineSet.h"
#include "PointSet.h"

class ThresholderImpl {
public:
  void configure(const ThreshParameters& params);
  void generate(const PointSet& points, LineSet* lines);
private:
  ThreshLine createLine(const ThreshPoint& start,
                        const ThreshPoint& end);
  bool testLine(const ThreshLine& line);
  ThreshParameters _params;
};

void ThresholderImpl::configure(const ThreshParameters &params) {
  _params = params;
}

void ThresholderImpl::generate(const PointSet &points,
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

ThreshLine ThresholderImpl::createLine(const ThreshPoint &start,
                                       const ThreshPoint &end) {
  ThreshLine line;
  line.start = start;
  line.end = end;
  line.squareDistance = start.position.distanceSquared(end.position);
  line.closeness = 0;
  if (_params.hasMaxDist()) {
    auto minDist = _params.hasMinDist() ? _params.minDist : 0;
    auto maxDist = _params.maxDist;
    if (fabs(minDist - maxDist) < FLT_EPSILON ||
        minDist >= maxDist) {
      line.closeness = 1;
    } else {
      line.closeness = ofMap(line.squareDistance,
                             minDist, maxDist, 1.0f, 0.0f);
    }
  }
  return line;
}

bool ThresholderImpl::testLine(const ThreshLine &line) {
  auto dist = line.squareDistance;
  if (_params.hasMinDist() && dist < _params.minDist) {
    return false;
  }
  if (_params.hasMaxDist() && dist > _params.maxDist) {
    return false;
  }
  return true;
}

void Thresholder::configure(const ThreshParameters& params) {
  if (!_impl)
    _impl.reset(new ThresholderImpl());
  _impl->configure(params);
}

void Thresholder::generate(const PointSet &points, LineSet *lines) {
  if (_impl)
    _impl->generate(points, lines);
}

LineSet Thresholder::generate(const PointSet &points) {
  LineSet lines;
  generate(points, &lines);
  return lines;
}


