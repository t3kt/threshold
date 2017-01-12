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

#include "Point.h"

template<typename TPoint>
class PointSource {
public:
  using PointT = TPoint;
  using VecT = typename TPoint::VecT;
  using ColorT = typename TPoint::ColorT;

  virtual int size() const = 0;
  virtual PointT operator[](int i) const = 0;
  virtual VecT getPosition(int i) const {
    return (*this)[i].position;
  }
  virtual ColorT getColor(int i) const {
    return (*this)[i].color;
  }
};

template<typename TPoint>
class PointSet
: public PointSource<TPoint> {
public:
  using PointT = TPoint;
  using StorageT = std::vector<PointT>;

  int size() const override {
    return points.size();
  }
  PointT operator[](int i) const override {
    return points[i];
  }
private:
  StorageT points;
};

#endif /* defined(__threshold__PointSet__) */
