//
//  Point.h
//  threshold
//

#pragma once

//#include "Common.h"

template<typename TVec, typename TColor>
class ThreshPoint {
public:
  using VecT = TVec;
  using ColorT = TColor;

  int index;
  VecT position;
  ColorT color;
};
