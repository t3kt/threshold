//
//  Point.h
//

#pragma once

template<typename TVec>
class ThreshPoint {
public:
  using VecT = TVec;

  int index;
  VecT position;
};
