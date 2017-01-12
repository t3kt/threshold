//
//  Logging.cpp
//  threshold
//
//  Created by tekt on 1/9/15.
//
//

#include "Logging.h"

#include <iostream>
#include <iomanip>

#include "Point.h"
#include "Line.h"
#include "PointSet.h"
#include "LineSet.h"
#include "Common.h"
#include "AppCommon.h"

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
               const char* name) {
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


std::ostream& operator<<(std::ostream& os, const AppPoint& pt) {
  os << "Point{#" << pt.index;
  os << ", (" << pt.position.x <<
        ", " << pt.position.y <<
        ", " << pt.position.z << ")";
  os << ", color:(" << pt.color.r
            << ", " << pt.color.g
            << ", " << pt.color.b
            << ", " << pt.color.a << ")";
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const ThreshLine& ln) {
  os << "Line{start:#" << ln.startIndex
  << ", end:#" << ln.endIndex
  << ", sqdist:" << ln.squareDistance
  << ", closeness:" << ln.closeness
  << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const AppPointSource& points) {
  outputSet(os, points, "PointSet");
  return os << std::endl;
}

std::ostream& operator<<(std::ostream& os, const LineSet& lines) {
  outputSet(os, lines, "LineSet");
  return os << std::endl;
}
