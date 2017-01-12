//
//  Common.h
//  threshold
//
//  Created by tekt on 1/7/15.
//
//

#pragma once

class ThVec3f {
public:
  ThVec3f() { }
  ThVec3f(float x0, float y0, float z0) : x(x0), y(y0), z(z0) { }

  ThVec3f operator+(const ThVec3f& vec) const {
    return ThVec3f(x + vec.x, y + vec.y, z + vec.z);
  }

  ThVec3f operator-(const ThVec3f& vec) const {
    return ThVec3f(x - vec.x, y - vec.y, z - vec.z);
  }

  ThVec3f operator*(const float f) const {
    return ThVec3f(x * f, y * f, z * f);
  }

  float x, y, z;
};

class ThColor {
public:
  ThColor() { }
  ThColor(float r0, float g0, float b0, float a0 = 1.0f)
  : r(r0), g(g0), b(b0), a(a0) { }

  float r, g, b, a;
};
