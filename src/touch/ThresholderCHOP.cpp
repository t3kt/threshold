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
  //
  OUT_R1,
  OUT_G1,
  OUT_B1,
  OUT_A1,
  OUT_R2,
  OUT_G2,
  OUT_B2,
  OUT_A2
};

enum {
  NUM_MAIN_OUTS = OUT_INDEX2 + 1,
  NUM_COLOR_OUTS = 4 * 2,
  NUM_TOTAL_OUTS = NUM_MAIN_OUTS + NUM_COLOR_OUTS
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

void
ThresholderCHOP::loadParameters(const CHOP_FloatInput* inputs) {
  ThreshParameters params;
  for (int i = 0; i < NUM_SETTINGS; ++i) {
    setParameter(inputs[i], &params);
  }
  _thresholder.configure(params);
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

void ThresholderCHOP::loadPoints(const CHOP_InputArrays *inputs) {
  _points.clear();
  if (!hasEnoughInputs(inputs))
    return;
  const float* xInput = NULL;
  const float* yInput = NULL;
  const float* zInput = NULL;
  const float* rInput = NULL;
  const float* gInput = NULL;
  const float* bInput = NULL;
  const float* aInput = NULL;
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
    } else if (strcmp(name, "r") == 0) {
      rInput = vals;
    } else if (strcmp(name, "g") == 0) {
      gInput = vals;
    } else if (strcmp(name, "b") == 0) {
      bInput = vals;
    } else if (strcmp(name, "a") == 0) {
      aInput = vals;
    }
  }
  if (xInput && yInput && zInput) {
    if (rInput && gInput && bInput) {
      _hasColor = true;
    }
    for (int i = 0; chopIn.length; ++i) {
      ThreshPoint point;
      point.position.x = xInput[i];
      point.position.y = yInput[i];
      point.position.z = zInput[i];
      point.index = i;
      if (_hasColor) {
        point.color.r = rInput[i];
        point.color.g = gInput[i];
        point.color.b = bInput[i];
        point.color.a = aInput ? aInput[0] : 1.0f;
      }
      _points.push_back(point);
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
  loadPoints(info->inputArrays);
  info->numChannels = NUM_MAIN_OUTS;
  if (_hasColor)
    info->numChannels += NUM_COLOR_OUTS;
  if (_points.empty())
    return false;
  loadParameters(info->inputArrays->floatInputs);
  _thresholder.generate(_points, &_lines);
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
    case OUT_TX1: return "tx1";
    case OUT_TY1: return "ty1";
    case OUT_TZ1: return "tz1";
    case OUT_TX2: return "tx2";
    case OUT_TY2: return "ty2";
    case OUT_TZ2: return "tz2";
    case OUT_SQRDIST: return "sqrdist";
    case OUT_CLOSENESS: return "closeness";
    case OUT_INDEX1: return "i1";
    case OUT_INDEX2: return "i2";
    case OUT_R1: return "r1";
    case OUT_G1: return "g1";
    case OUT_B1: return "b1";
    case OUT_A1: return "a1";
    case OUT_R2: return "r2";
    case OUT_G2: return "g2";
    case OUT_B2: return "b2";
    case OUT_A2: return "a2";
  }
  return NULL;
}

static void outputLine(const ThreshLine& line,
                       float** channels,
                       int i,
                       bool hasColor) {
  channels[OUT_TX1][i] = line.start.position.x;
  channels[OUT_TY1][i] = line.start.position.y;
  channels[OUT_TZ1][i] = line.start.position.z;
  channels[OUT_TX2][i] = line.end.position.x;
  channels[OUT_TY2][i] = line.end.position.y;
  channels[OUT_TZ2][i] = line.end.position.z;
  channels[OUT_SQRDIST][i] = line.squareDistance;
  channels[OUT_CLOSENESS][i] = line.closeness;
  channels[OUT_INDEX1][i] = line.start.index;
  channels[OUT_INDEX2][i] = line.end.index;
  if (hasColor) {
    channels[OUT_R1][i] = line.start.color.r;
    channels[OUT_G1][i] = line.start.color.g;
    channels[OUT_B1][i] = line.start.color.b;
    channels[OUT_A1][i] = line.start.color.a;
    channels[OUT_R2][i] = line.end.color.r;
    channels[OUT_G2][i] = line.end.color.g;
    channels[OUT_B2][i] = line.end.color.b;
    channels[OUT_A2][i] = line.end.color.a;
  }
}

void ThresholderCHOP::execute(const CHOP_Output *outputs,
                              const CHOP_InputArrays *inputs,
                              void *reserved) {
  if (_lines.empty()) {
    ThreshLine dummy;
    outputLine(dummy, outputs->channels, 0, _hasColor);
  } else {
    auto length = _lines.size();
    for (int i = 0; i < length; ++i) {
      const auto& line = _lines[i];
      outputLine(line, outputs->channels, i, _hasColor);
    }
  }
}

int ThresholderCHOP::getNumInfoCHOPChans() {
  return 0;
}

void ThresholderCHOP::getInfoCHOPChan(int index,
                                      CHOP_InfoCHOPChan *chan) {
  
}

