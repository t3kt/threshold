//
//  Thresholder.cpp
//

#include "Thresholder.h"

namespace _ThreshUtil {
  float distanceSquared(float x1, float y1, float z1,
                        float x2, float y2, float z2) {
    float vx = x2 - x1;
    float vy = y2 - y1;
    float vz = z2 - z1;
    return vx*vx + vy*vy + vz*vz;
  }

  float mapRange(float value, float inputMin, float inputMax,
                 float outputMin, float outputMax) {

    if (std::fabs(inputMin - inputMax) < FLT_EPSILON) {
      return outputMin;
    } else {
      float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
      return outVal;
    }
  }
}
