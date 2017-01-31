//
//  Thresholder.h
//

#pragma once

#include <float.h>
#include <cmath>

#include "Common.h"
#include "Parameters.h"
#include "LineSet.h"
#include "PointSet.h"


namespace _ThreshUtil {
  float distanceSquared(float x1, float y1, float z1,
                        float x2, float y2, float z2);
  float mapRange(float value, float inputMin, float inputMax,
                 float outputMin, float outputMax);
}

template<typename TPoint, typename TPointSource>
class Thresholder {
public:
  using PointT = TPoint;
  using PointSourceT = TPointSource;

  void configure(const ThreshParameters& params) {
    _params = params;
  }
  void generate(const PointSourceT* pointsA,
                const PointSourceT* pointsB,
                LineSet* lines) {
    if (pointsB && _params.useSeparateSource) {
      generateSeparateSources(*pointsA, *pointsB, lines);
    } else {
      generateSingleSource(*pointsA, lines);
    }
  }
  const ThreshParameters& params() const {
    return _params;
  }
private:
  void generateSingleSource(const PointSourceT& points,
                            LineSet* lines) {
    for (int indexA = 0; indexA < points.size(); indexA++) {
      const auto& pointA = points.getPoint(indexA);
      int linesFromPointA = 0;
      for (int indexB = indexA + 1; indexB < points.size(); indexB++) {
        const auto& pointB = points.getPoint(indexB);
        auto line = createLine(pointA, pointB);
        if (testLine(line)) {
          lines->push_back(line);
          linesFromPointA++;
          if (_params.hasMaxLinesPerSource() &&
              linesFromPointA >= _params.maxLinesPerSource) {
            break;
          }
          if (lines->size() >= _params.maxLines) {
            return;
          }
        }
      }
    }
  }
  void generateSeparateSources(const PointSourceT& pointsA,
                               const PointSourceT& pointsB,
                               LineSet* lines) {
    for (int indexA = 0; indexA < pointsA.size(); ++indexA) {
      const auto& pointA = pointsA.getPoint(indexA);
      int linesFromPointA = 0;
      for (int indexB = 0; indexB < pointsB.size(); ++indexB) {
        const auto& pointB = pointsB.getPoint(indexB);
        auto line = createLine(pointA, pointB);
        if (testLine(line)) {
          lines->push_back(line);
          linesFromPointA++;
          if (_params.hasMaxLinesPerSource() &&
              linesFromPointA >= _params.maxLinesPerSource) {
            break;
          }
          if (lines->size() >= _params.maxLines) {
            return;
          }
        }
      }
    }
  }

  ThreshLine createLine(const PointT& start,
                        const PointT& end) {
    ThreshLine line;
    line.startIndex = start.index;
    line.endIndex = end.index;
    line.startGroup = start.group;
    line.endGroup = end.group;
    line.squareDistance
    = _ThreshUtil::distanceSquared(start.position.x,
                                   start.position.y,
                                   start.position.z,
                                   end.position.x,
                                   end.position.y,
                                   end.position.z);
    line.closeness = 0;
    if (_params.hasMaxDist()) {
      auto minDist = _params.hasMinDist() ? _params.minDist : 0;
      auto maxDist = _params.maxDist;
      if (std::fabs(minDist - maxDist) < FLT_EPSILON ||
          minDist >= maxDist) {
        line.closeness = 1;
      } else {
        line.closeness
        = _ThreshUtil::mapRange(line.squareDistance,
                                minDist, maxDist, 1.0f, 0.0f);
      }
    }
    return line;
  }
  bool testLine(const ThreshLine& line) {
    auto dist = line.squareDistance;
    if (_params.hasMinDist() && dist < _params.minDist) {
      return false;
    }
    if (_params.hasMaxDist() && dist > _params.maxDist) {
      return false;
    }
    if (_params.distinctGroups && line.startGroup == line.endGroup) {
      return false;
    }
    return true;
  }
  
  ThreshParameters _params;
};
