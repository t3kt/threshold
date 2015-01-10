//
//  AppCommon.cpp
//  threshold
//
//  Created by tekt on 1/10/15.
//
//

#include "AppCommon.h"

#include <ofMath.h>

#include "Point.h"

ofVec3f createSignedNoiseVec3f(const ofVec3f& position) {
  return ofVec3f(ofSignedNoise(position.x),
                 ofSignedNoise(position.y),
                 ofSignedNoise(position.z));
}

ofVec3f createRandomVec3f(const ofVec3f& mins,
                          const ofVec3f& maxs) {
  return ofVec3f(ofRandom(mins.x, maxs.x),
                 ofRandom(mins.y, maxs.y),
                 ofRandom(mins.z, maxs.z));
}
ofVec3f createRandomVec3f(float max) {
  return ofVec3f(ofRandom(max),
                 ofRandom(max),
                 ofRandom(max));
}
ofVec3f wrapVec(ofVec3f vec, float min, float max) {
  vec.x = ofWrap(vec.x, min, max);
  vec.y = ofWrap(vec.y, min, max);
  vec.z = ofWrap(vec.z, min, max);
  return vec;
}

ofVec3f getPointPos(const ThreshPoint& point) {
  return ofVec3f(point.x, point.y, point.z);
}

void setPointPos(ThreshPoint& point, const ofVec3f& pos) {
  point.x = pos.x;
  point.y = pos.y;
  point.z = pos.z;
}

ofFloatColor getPointColor(const ThreshPoint& point) {
  return ofFloatColor(point.r, point.g, point.g, point.a);
}

void setPointColor(ThreshPoint& point, const ofFloatColor& color) {
  point.r = color.r;
  point.g = color.g;
  point.b = color.b;
  point.a = color.a;
}
