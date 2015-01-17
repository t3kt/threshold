#include "stdafx.h"
#include "CppUnitTest.h"
#include "Thresholder.h"
#include "Parameters.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static ThreshPoint& addPt(PointSet* points, float x, float y, float z) {
  ThreshPoint pt;
  pt.index = points->size();
  pt.position.x = x;
  pt.position.y = y;
  pt.position.z = z;
  points->push_back(pt);
  return points->back();
}

namespace ThresholdCoreTest
{
TEST_CLASS(ThresholderUnitTest)
{
 public:

  TEST_METHOD(TestBasicPointHandling)
  {
    Thresholder thresholder;
    ThreshParameters params;
    PointSet points;
    LineSet lines;

    auto pt1 = addPt(&points, 1, 0, 0);
    auto pt2 = addPt(&points, 2, 0, 0);
    auto pt3 = addPt(&points, 5, 0, 0);

    params.minDist = 0;
    params.maxDist = 2;

    thresholder.configure(params);
    thresholder.generate(points, &lines);

    Assert::AreEqual(std::size_t(1), lines.size());
  }
};
}