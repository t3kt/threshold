//
//  AppCommon.h
//  threshold
//
//  Created by tekt on 1/10/15.
//
//

#ifndef __threshold__AppCommon__
#define __threshold__AppCommon__

#include <string>
#include <iostream>
#include <iomanip>
#include <ofVec3f.h>
#include <ofColor.h>

// see http://stackoverflow.com/q/1489830/1456378
template <class T>
int numDigits(T number)
{
  int digits = 0;
  if (number < 0)
    digits = 1; // remove this line if '-' counts as a digit
  while (number) {
    number /= 10;
    digits++;
  }
  return digits;
}

template<typename TSet>
void outputSet(std::ostream& os, const TSet& things,
               const std::string& name) {
  auto size = things.size();
  auto digits = numDigits(size - 1);
  os << name << "{size:" << size << "\n";
  for (auto i = 0; i < size; ++i) {
    os << "\t[";
    os << std::setw(digits) << std::right << i;
    os << "] " << things[i] << "\n";
  }
  os << "}";
}

ofVec3f createSignedNoiseVec3f(const ofVec3f& position);
ofVec3f createRandomVec3f(const ofVec3f& mins,
                          const ofVec3f& maxs);
ofVec3f createRandomVec3f(float max);
ofVec3f wrapVec(ofVec3f vec, float min, float max);

class ThreshPoint;

ofFloatColor getPointColor(const ThreshPoint& point);
void setPointColor(ThreshPoint& point, const ofFloatColor& color);

#endif /* defined(__threshold__AppCommon__) */
