//
//  Logging.h
//

#pragma once

#include "AppCommon.h"
#include <iostream>

class ThreshLine;
class LineSet;

std::ostream& operator<<(std::ostream& os, const AppPoint& pt);

std::ostream& operator<<(std::ostream& os, const ThreshLine& ln);

std::ostream& operator<<(std::ostream& os, const AppPointSource& points);

std::ostream& operator<<(std::ostream& os, const LineSet& lines);
