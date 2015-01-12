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
  OUT_INDEX1,
  OUT_INDEX2,
  OUT_SQRDIST,
  OUT_CLOSENESS,
  OUT_TX1,
  OUT_TY1,
  OUT_TZ1,
  OUT_TX2,
  OUT_TY2,
  OUT_TZ2,
  
  NUM_MAIN_OUTS
};

static inline void addChannel(std::vector<OutputChannel>& channels,
                              std::string outName,
                              std::size_t sourceIndex = -1) {
  OutputChannel channel;
  channel.outName = outName;
  channel.outIndex = NUM_MAIN_OUTS + channels.size();
  channel.sourceIndex = sourceIndex;
  channels.push_back(channel);
}

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
  _extraPointChannels.clear();
  const float* xInput = NULL;
  const float* yInput = NULL;
  const float* zInput = NULL;
  const float* rInput = NULL;
  const float* gInput = NULL;
  const float* bInput = NULL;
  const float* aInput = NULL;
  auto chopIn = inputs->CHOPInputs[0];
  for (int i = 0; i < chopIn.numChannels; ++i) {
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
    } else {
      addChannel(_extraPointChannels, std::string(name) + '0', i);
      addChannel(_extraPointChannels, std::string(name) + '1', i);
    }
  }
  if (xInput && yInput && zInput) {
    _hasColor = rInput && gInput && bInput;
    for (int i = 0; i < chopIn.length; ++i) {
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
  info->numChannels = NUM_MAIN_OUTS + _extraPointChannels.size();
  if (_points.empty())
    return false;
  loadParameters(info->inputArrays->floatInputs);
  _thresholder.generate(_points, &_lines);
  if (_lines.empty()) {
    info->length = 1;
  } else {
    info->length = static_cast<int>(_lines.size());
  }
  return true;
}

const char* ThresholderCHOP::getChannelName(int index,
                                            void *reserved) {
  switch(index)
  {
    case OUT_SQRDIST: return "sqrdist";
    case OUT_CLOSENESS: return "closeness";
    case OUT_INDEX1: return "i1";
    case OUT_INDEX2: return "i2";
    case OUT_TX1: return "tx1";
    case OUT_TY1: return "ty1";
    case OUT_TZ1: return "tz1";
    case OUT_TX2: return "tx2";
    case OUT_TY2: return "ty2";
    case OUT_TZ2: return "tz2";
    default:
      int i = index - NUM_MAIN_OUTS;
      if (i >= _extraPointChannels.size())
        return NULL;
      return _extraPointChannels[i].outName.c_str();
  }
  return NULL;
}

void ThresholderCHOP::outputLine(const ThreshLine& line,
                                 std::size_t i,
                                 float **channels,
                                 const CHOP_InputArrays *inputs) const {
  channels[OUT_INDEX1][i] = static_cast<float>(line.start.index);
  channels[OUT_INDEX2][i] = static_cast<float>(line.end.index);
  channels[OUT_SQRDIST][i] = line.squareDistance;
  channels[OUT_CLOSENESS][i] = line.closeness;
  channels[OUT_TX1][i] = line.start.position.x;
  channels[OUT_TY1][i] = line.start.position.y;
  channels[OUT_TZ1][i] = line.start.position.z;
  channels[OUT_TX2][i] = line.end.position.x;
  channels[OUT_TY2][i] = line.end.position.y;
  channels[OUT_TZ2][i] = line.end.position.z;
  auto inChans = inputs->CHOPInputs[0].channels;
  for (const auto& outChan : _extraPointChannels) {
    auto val = inChans[outChan.sourceIndex][line.start.index];
    channels[outChan.outIndex][i] = val;
  }
}

void ThresholderCHOP::execute(const CHOP_Output *outputs,
                              const CHOP_InputArrays *inputs,
                              void *reserved) {
  if (_lines.empty()) {
    ThreshLine dummy;
    outputLine(dummy, 0, outputs->channels, inputs);
  } else {
    auto length = _lines.size();
    for (std::size_t i = 0; i < length; ++i) {
      const auto& line = _lines[i];
      outputLine(line, i, outputs->channels, inputs);
    }
  }
}

int ThresholderCHOP::getNumInfoCHOPChans() {
  return 0;
}

void ThresholderCHOP::getInfoCHOPChan(int index,
                                      CHOP_InfoCHOPChan *chan) {
  
}

