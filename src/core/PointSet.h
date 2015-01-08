//
//  PointSet.h
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#ifndef __threshold__PointSet__
#define __threshold__PointSet__

#include <vector>
#include <iostream>

#include "Point.h"

class PointSet : public std::vector<ThreshPoint> {
  
};

std::ostream& operator<<(std::ostream& os, const PointSet& points);

#endif /* defined(__threshold__PointSet__) */
