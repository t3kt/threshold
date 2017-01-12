//
//  PointSet.h
//

#pragma once

#include <vector>

#include "Point.h"

template<typename TPoint>
class PointSource {
public:
  using PointT = TPoint;
  using VecT = typename TPoint::VecT;
  using ColorT = typename TPoint::ColorT;

  virtual int size() const = 0;
  PointT operator[](int i) const {
    return getPoint(i);
  }
  virtual PointT getPoint(int i) const = 0;
  virtual VecT getPosition(int i) const {
    return getPoint(i).position;
  }
  virtual ColorT getColor(int i) const {
    return getPoint(i).color;
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
  PointT getPoint(int i) const override {
    return points[i];
  }

private:
  StorageT points;
};

