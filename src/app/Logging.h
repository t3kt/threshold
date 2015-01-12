//
//  Logging.h
//  threshold
//
//  Created by tekt on 1/9/15.
//
//

#ifndef __threshold__Logging__
#define __threshold__Logging__

#include <iostream>

class ThreshPoint;
class ThreshLine;
class PointSource;
class LineSet;

std::ostream& operator<<(std::ostream& os, const ThreshPoint& pt);

std::ostream& operator<<(std::ostream& os, const ThreshLine& ln);

std::ostream& operator<<(std::ostream& os, const PointSource& points);

std::ostream& operator<<(std::ostream& os, const LineSet& lines);

#endif /* defined(__threshold__Logging__) */
