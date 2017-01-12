//
//  AppCommon.h
//

#pragma once

#include <ofVec3f.h>
#include <ofColor.h>

#include "Common.h"
#include "Point.h"
#include "PointSet.h"
#include "PointSystem.h"

using AppPoint = ThreshPoint<ofVec3f, ofFloatColor>;
using AppPointSource = PointSource<AppPoint>;
using AppPointSystem = PointSystem<AppPoint>;

ofVec3f createSignedNoiseVec3f(const ofVec3f& position);
ofVec3f createRandomVec3f(const ofVec3f& mins,
                          const ofVec3f& maxs);
ofVec3f createRandomVec3f(float max);
ofVec3f wrapVec(ofVec3f vec, float min, float max);

