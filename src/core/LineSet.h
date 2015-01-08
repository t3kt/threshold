//
//  LineSet.h
//  threshold
//
//  Created by tekt on 1/6/15.
//
//

#ifndef __threshold__LineSet__
#define __threshold__LineSet__

#include <vector>
#include <iostream>

#include "Line.h"

class LineSet
: public std::vector<ThreshLine> {
};

std::ostream& operator<<(std::ostream& os, const LineSet& lines);

#endif /* defined(__threshold__LineSet__) */
