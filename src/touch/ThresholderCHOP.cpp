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
#include "TouchCommon.h"

enum {
  SETTING_DISTRANGE,
  SETTING_MAXLINES,
  SETTING_MAXPERSOURCE,
  SETTING_RESET_CHANS,
  //  SETTING_SEPARATESOURCE,
  NUM_SETTINGS
};

enum {
  OUT_INDEX1,
  OUT_INDEX2,
  OUT_SQRDIST,
  OUT_CLOSENESS,
  
  NUM_MAIN_OUTS
};

static inline void addChannel(std::vector<OutputChannel>& channels,
                              std::string outName,
                              bool isStart,
                              std::size_t sourceIndex) {
  OutputChannel channel;
  channel.outName = outName;
  channel.outIndex = NUM_MAIN_OUTS + channels.size();
  channel.isStart = isStart;
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

void ThresholderCHOP::loadChannels(const CHOP_InputArrays *inputs) {
  if (!hasEnoughInputs(inputs))
    return;
  auto& chopIn = inputs->CHOPInputs[0];
  const float* xInput = NULL;
  const float* yInput = NULL;
  const float* zInput = NULL;
  if (_xInputIndex < 0 ||
      _yInputIndex < 0 ||
      _zInputIndex < 0 ||
      inputs->floatInputs[SETTING_RESET_CHANS].values[0] != 0) {
    _xInputIndex = 0;
    _yInputIndex = 1;
    _zInputIndex = 2;
    _pointChannels.clear();
    for (int i = 0; i < chopIn.numChannels; ++i) {
      const auto& name = chopIn.names[i];
      const float* vals = chopIn.channels[i];
      if (strcmp(name, "x") == 0) {
        xInput = vals;
        _xInputIndex = i;
        addChannel(_pointChannels, "tx1", true, i);
        addChannel(_pointChannels, "tx2", false, i);
      } else if (strcmp(name, "y") == 0) {
        yInput = vals;
        _yInputIndex = i;
        addChannel(_pointChannels, "ty1", true, i);
        addChannel(_pointChannels, "ty2", false, i);
      } else if (strcmp(name, "z") == 0) {
        zInput = vals;
        _zInputIndex = i;
        addChannel(_pointChannels, "tz1", true, i);
        addChannel(_pointChannels, "tz2", false, i);
      } else {
        addChannel(_pointChannels, std::string(name) + '0',
                   true, i);
        addChannel(_pointChannels, std::string(name) + '1',
                   false, i);
      }
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
  loadChannels(info->inputArrays);
  info->numChannels = NUM_MAIN_OUTS + static_cast<int>(_pointChannels.size());
  if (!hasEnoughInputs(info->inputArrays))
    return false;
  loadParameters(info->inputArrays->floatInputs);
  CHOPInputPointSet pointsA(&info->inputArrays->CHOPInputs[0],
                           _xInputIndex,
                           _yInputIndex,
                           _zInputIndex);
  if (info->inputArrays->numCHOPInputs > 1) {
    CHOPInputPointSet pointsB(&info->inputArrays->CHOPInputs[1],
                              _xInputIndex,
                              _yInputIndex,
                              _zInputIndex);
    _thresholder.generate(&pointsA, &pointsB, &_lines);
  } else {
    _thresholder.generate(&pointsA, NULL, &_lines);
  }
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
    default:
      int i = index - NUM_MAIN_OUTS;
      if (i >= _pointChannels.size())
        return NULL;
      return _pointChannels[i].outName.c_str();
  }
  return NULL;
}

void ThresholderCHOP::outputLineSingle(const ThreshLine& line,
                                       std::size_t i,
                                       float **channels,
                                       const CHOP_InputArrays *inputs) const {
  const auto& startIndex = line.startIndex;
  const auto& endIndex = line.endIndex;
  channels[OUT_INDEX1][i] = static_cast<float>(startIndex);
  channels[OUT_INDEX2][i] = static_cast<float>(endIndex);
  channels[OUT_SQRDIST][i] = line.squareDistance;
  channels[OUT_CLOSENESS][i] = line.closeness;
  auto inChans = inputs->CHOPInputs[0].channels;
  for (const auto& outChan : _pointChannels) {
    auto srcIndex = outChan.isStart ? startIndex : endIndex;
    auto val = inChans[outChan.sourceIndex][srcIndex];
    channels[outChan.outIndex][i] = val;
  }
}

void ThresholderCHOP::outputLineSeparate(const ThreshLine& line,
                                         std::size_t i,
                                         float **channels,
                                         const CHOP_InputArrays *inputs) const {
  const auto& startIndex = line.startIndex;
  const auto& endIndex = line.endIndex;
  channels[OUT_INDEX1][i] = static_cast<float>(startIndex);
  channels[OUT_INDEX2][i] = static_cast<float>(endIndex);
  channels[OUT_SQRDIST][i] = line.squareDistance;
  channels[OUT_CLOSENESS][i] = line.closeness;
  auto inChansA = inputs->CHOPInputs[0].channels;
  auto inChansB = inputs->CHOPInputs[1].channels;
  for (const auto& outChan : _pointChannels) {
    if (outChan.isStart) {
      auto val = inChansA[outChan.sourceIndex][startIndex];
      channels[outChan.outIndex][i] = val;
    } else {
      auto val = inChansB[outChan.sourceIndex][endIndex];
      channels[outChan.outIndex][i] = val;
    }
  }
}

void ThresholderCHOP::execute(const CHOP_Output *outputs,
                              const CHOP_InputArrays *inputs,
                              void *reserved) {
  if (_lines.empty()) {
    ThreshLine dummy;
    outputLineSingle(dummy, 0, outputs->channels, inputs);
  } else {
    auto length = _lines.size();
    if (inputs->numCHOPInputs > 1 &&
        _thresholder.params().useSeparateSource) {
      for (std::size_t i = 0; i < length; ++i) {
        const auto& line = _lines[i];
        outputLineSeparate(line, i, outputs->channels, inputs);
      }
    } else {
      for (std::size_t i = 0; i < length; ++i) {
        const auto& line = _lines[i];
        outputLineSingle(line, i, outputs->channels, inputs);
      }
    }
  }
}

int ThresholderCHOP::getNumInfoCHOPChans() {
  return 0;
}

void ThresholderCHOP::getInfoCHOPChan(int index,
                                      CHOP_InfoCHOPChan *chan) {
  
}

