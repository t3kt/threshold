//
//  ThresholderCHOP.cpp
//  threshold
//
//  Created by tekt on 1/9/15.
//
//

#include "ThresholderCHOP.h"

#include <string.h>

#include "Parameters.h"
#include "Point.h"
#include "Line.h"
#include "PointSet.h"

enum {
  SETTING_DISTRANGE,
  SETTING_MAXLINES,
  SETTING_MAXPERSOURCE,
  //  SETTING_SEPARATESOURCE,
  NUM_SETTINGS
};

enum {
  OUT_TX1,
  OUT_TY1,
  OUT_TZ1,
  OUT_TX2,
  OUT_TY2,
  OUT_TZ2,
  OUT_SQRDIST,
  OUT_CLOSENESS,
  OUT_INDEX1,
  OUT_INDEX2,
  NUM_OUTS
};

static void setParameter(const CHOP_FloatInput& input,
                         ThreshParameters* params) {
  switch (input.inputNumber) {
    case SETTING_DISTRANGE:
      params->minDist = input.values[0];
      params->maxDist = input.values[1];
      break;
    case SETTING_MAXLINES:
      params->maxLines = static_cast<int>(input.values[0]);
      break;
    case SETTING_MAXPERSOURCE:
      params->maxLinesPerSource = static_cast<int>(input.values[0]);
      break;
    default:
      break;
  }
}

static ThreshParameters
loadParameters(const CHOP_FloatInput* inputs) {
  ThreshParameters params;
  for (int i = 0; NUM_SETTINGS; ++i) {
    setParameter(inputs[i], &params);
  }
  return params;
}

static bool
hasEnoughInputs(const CHOP_InputArrays* inputs) {
  if (inputs->numCHOPInputs < 1)
    return false;
  if (inputs->CHOPInputs[0].numChannels < 3 ||
      inputs->CHOPInputs[0].length <= 1)
    return false;
  return true;
}

static PointSet
loadPoints(const CHOP_InputArrays* inputs) {
  PointSet points;
  if (hasEnoughInputs(inputs)) {
    const float* xInput = NULL;
    const float* yInput = NULL;
    const float* zInput = NULL;
    auto chopIn = inputs->CHOPInputs[0];
    for (int i = 0; chopIn.numChannels; ++i) {
      const auto& name = chopIn.names[i];
      const float* vals = chopIn.channels[i];
      if (strcmp(name, "x") == 0) {
        xInput = vals;
      } else if (strcmp(name, "y") == 0) {
        yInput = vals;
      } else if (strcmp(name, "z") == 0) {
        zInput = vals;
      }
    }
    if (xInput && yInput && zInput) {
      for (int i = 0; chopIn.length; ++i) {
        ThreshPoint point;
        point.position.x = xInput[i];
        point.position.y = yInput[i];
        point.position.z = zInput[i];
        point.index = i;
        points.push_back(point);
      }
    }
  }
  return points;
}

static void
outputValues(const LineSet& lines, const CHOP_Output* outputs) {
  if (lines.empty()) {
    outputs->channels[OUT_TX1][0] = 0;
    outputs->channels[OUT_TY1][0] = 0;
    outputs->channels[OUT_TZ1][0] = 0;
    outputs->channels[OUT_TX2][0] = 0;
    outputs->channels[OUT_TY2][0] = 0;
    outputs->channels[OUT_TZ2][0] = 0;
    outputs->channels[OUT_SQRDIST][0] = 0;
    outputs->channels[OUT_CLOSENESS][0] = 0;
    outputs->channels[OUT_INDEX1][0] = 0;
    outputs->channels[OUT_INDEX2][0] = 0;
  } else {
    auto length = lines.size();
    for (int i = 0; i < length; ++i) {
      const auto& line = lines[i];
      outputs->channels[OUT_TX1][i] = line.start.position.x;
      outputs->channels[OUT_TY1][i] = line.start.position.y;
      outputs->channels[OUT_TZ1][i] = line.start.position.z;
      outputs->channels[OUT_TX2][i] = line.end.position.x;
      outputs->channels[OUT_TY2][i] = line.end.position.y;
      outputs->channels[OUT_TZ2][i] = line.end.position.z;
      outputs->channels[OUT_SQRDIST][i] = line.squareDistance;
      outputs->channels[OUT_CLOSENESS][i] = line.closeness;
      outputs->channels[OUT_INDEX1][i] = line.start.index;
      outputs->channels[OUT_INDEX2][i] = line.end.index;
    }
  }
}

void ThresholderCHOP::getGeneralInfo(CHOP_GeneralInfo *ginfo)
{
  // This will cause the node to cook every frame
  //ginfo->cookEveryFrameIfAsked = true;
  //ginfo->timeslice = true;
  ginfo->timeslice = false;
  ginfo->inputMatchIndex = 0;
}

bool ThresholderCHOP::getOutputInfo(CHOP_OutputInfo *info) {
  _lines.clear();
  info->length = 0;
  info->numChannels = NUM_OUTS;
  PointSet points =  loadPoints(info->inputArrays);
  if (points.empty())
    return false;
  ThreshParameters params = loadParameters(info->inputArrays->floatInputs);
  _thresholder.configure(params);
  _thresholder.generate(points, &_lines);
  if (_lines.empty()) {
    info->length = 1;
  } else {
    info->length = _lines.size();
  }
  return true;
}

const char* ThresholderCHOP::getChannelName(int index,
                                            void *reserved) {
  switch(index)
  {
    case OUT_TX1:
      return "tx1";
    case OUT_TY1:
      return "ty1";
    case OUT_TZ1:
      return "tz1";
    case OUT_TX2:
      return "tx2";
    case OUT_TY2:
      return "ty2";
    case OUT_TZ2:
      return "tz2";
    case OUT_SQRDIST:
      return "sqrdist";
    case OUT_INDEX1:
      return "index1";
    case OUT_INDEX2:
      return "index2";
  }
  return NULL;
}

void ThresholderCHOP::execute(const CHOP_Output *outputs,
                              const CHOP_InputArrays *inputs,
                              void *reserved) {
  outputValues(_lines, outputs);
}

int ThresholderCHOP::getNumInfoCHOPChans() {
  return 0;
}

void ThresholderCHOP::getInfoCHOPChan(int index,
                                      CHOP_InfoCHOPChan *chan) {
  
}

