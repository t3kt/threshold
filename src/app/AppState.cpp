//
//  AppState.cpp
//  threshold
//
//  Created by tekt on 1/19/15.
//
//

#include "AppState.h"

bool ThreshAppState::getLinePointsRaw(const ThreshLine &line,
                                      ThreshPoint *pt1,
                                      ThreshPoint *pt2) const {
  if (!points1)
    return false;
  *pt1 = (*points1)[line.startIndex];
  if (points2 && appParams.useSeparateSource.get())
    *pt2 = (*points2)[line.endIndex];
  else
    *pt2 = (*points1)[line.endIndex];
  return true;
}

static void extendPoints(ThVec3f& pos1, ThVec3f& pos2,
                         float extend1, float extend2) {
  auto diff = pos2 - pos1;
  pos1 = pos1 + (diff * extend1);
  pos2 = pos2 - (diff * extend2);
}

bool ThreshAppState::getLinePoints(const ThreshLine &line,
                                   ThreshPoint *pt1,
                                   ThreshPoint *pt2) const {
  if (!getLinePointsRaw(line, pt1, pt2))
    return false;
  if (appParams.useExtend.get()) {
    ThVec3f pos1 = pt1->position;
    ThVec3f pos2 = pt2->position;
    extendPoints(pos1, pos2,
                 appParams.extendRatioStart,
                 appParams.extendRatioEnd);
    pt1->position = pos1;
    pt2->position = pos2;
  }
  if (appParams.applyClosenessToAlpha.get()) {
//    pt1->color.a *= line.closeness;
//    pt2->color.b *= line.closeness;
    pt1->color.a = pt2->color.a = line.closeness;
  }
  return true;
}
