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

static void setParameter(ThreshParameters* params,
                         const CHOP_FloatInput& input) {
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
    setParameter(&params, inputs[i]);
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

static bool
loadPoints(PointSet* points, const CHOP_InputArrays* inputs) {
  if (!hasEnoughInputs(inputs)) {
    
  }
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
  if (!xInput || !yInput || !zInput)
    return false;
  for (int i = 0; chopIn.length; ++i) {
    ThreshPoint point;
    point.position.x = xInput[i];
    point.position.y = yInput[i];
    point.position.z = zInput[i];
    point.index = i;
    points->push_back(point);
  }
  return true;
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
      outputs->channels[OUT_TX1][i] = line.start->position.x;
      outputs->channels[OUT_TY1][i] = line.start->position.y;
      outputs->channels[OUT_TZ1][i] = line.start->position.z;
      outputs->channels[OUT_TX2][i] = line.end->position.x;
      outputs->channels[OUT_TY2][i] = line.end->position.y;
      outputs->channels[OUT_TZ2][i] = line.end->position.z;
      outputs->channels[OUT_SQRDIST][i] = line.squareDistance;
      outputs->channels[OUT_CLOSENESS][i] = line.closeness;
      outputs->channels[OUT_INDEX1][i] = line.start->index;
      outputs->channels[OUT_INDEX2][i] = line.end->index;
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
  return true;
}

const char* ThresholderCHOP::getChannelName(int index,
                                            void *reserved) {
  return NULL;
}

void ThresholderCHOP::execute(const CHOP_Output *,
                              const CHOP_InputArrays *,
                              void *reserved) {
  
}

int ThresholderCHOP::getNumInfoCHOPChans() {
  return 0;
}

void ThresholderCHOP::getInfoCHOPChan(int index,
                                      CHOP_InfoCHOPChan *chan) {
  
}

