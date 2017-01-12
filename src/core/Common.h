//
//  Common.h
//  threshold
//
//  Created by tekt on 1/7/15.
//
//

#ifndef __threshold__Common__
#define __threshold__Common__


#ifdef USING_OPEN_FRAMEWORKS

//#include <ofVec3f.h>
//#include <ofColor.h>
//#include <glm/glm.hpp>

#endif

#include <memory>

class ThVec3f {
public:
  ThVec3f() { }
  ThVec3f(float x0, float y0, float z0) : x(x0), y(y0), z(z0) { }
  
//#ifdef USING_OPEN_FRAMEWORKS
//  
//  ThVec3f(const ofVec3f& other)
//  : x(other.x), y(other.y), z(other.z) { }
//
////  ThVec3f(const glm::vec3& other)
////  : x(other.x), y(other.y), z(other.z) { }
//
////  ThVec3f& operator=(const glm::vec3& other) {
////    x = other.x;
////    y = other.y;
////    z = other.z;
////    return *this;
////  }
////
////  ThVec3f& operator=(const ofVec3f& other) {
////    x = other.x;
////    y = other.y;
////    z = other.z;
////    return *this;
////  }
//
//  template<typename T>
//  ThVec3f& operator=(const T& other) {
//    x = other.x;
//    y = other.y;
//    z = other.z;
//    return *this;
//  }
//
//  operator ofVec3f() const { return ofVec3f(x, y, z); }
//
////  operator glm::vec3() const { return glm::vec3(x, y, z); }
//
//#endif

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
  
//#ifdef USING_OPEN_FRAMEWORKS
//  
//  ThColor(const ofFloatColor& other)
//  : r(other.r), g(other.g), b(other.b), a(other.a) { }
//  
//  ThColor& operator=(const ofFloatColor& other) {
//    r = other.r;
//    g = other.g;
//    b = other.b;
//    a = other.a;
//    return *this;
//  }
//  
//  operator ofFloatColor() const { return ofFloatColor(r, g, b, a); }
//  
//#endif

  float r, g, b, a;
};


#endif /* defined(__threshold__Common__) */
