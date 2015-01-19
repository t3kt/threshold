//
//  ThresholderCHOP.cpp
//  threshold
//
//  Created by tekt on 1/9/15.
//
//

#include "ThresholderCHOP.h"

#include <string.h>
#include <map>

#include "Parameters.h"
#include "Point.h"
#include "Line.h"
#include "TouchCommon.h"

enum {
  SETTING_DISTRANGE,
  SETTING_MAXLINES,
  SETTING_MAXPERSOURCE,
  SETTING_RESET_CHANS,
  SETTING_SEPARATESOURCE,
  NUM_SETTINGS
};

enum {
  OUT_INDEX1,
  OUT_INDEX2,
  OUT_SQRDIST,
  OUT_CLOSENESS,
  
  NUM_MAIN_OUTS
};

ThresholderCHOP::ThresholderCHOP(const CHOP_NodeInfo* info)
: _xInputIndex(-1, -1)
, _yInputIndex(-1, -1)
, _zInputIndex(-1, -1) { }

void
ThresholderCHOP::loadParameters(const CHOP_FloatInput* inputs) {
  ThreshParameters params;
  params.minDist = inputs[SETTING_DISTRANGE].values[0];
  params.maxDist = inputs[SETTING_DISTRANGE].values[1];
  params.maxLines = static_cast<int>(inputs[SETTING_MAXLINES].values[0]);
  params.maxLinesPerSource = static_cast<int>(inputs[SETTING_MAXPERSOURCE].values[0]);
  params.useSeparateSource = inputs[SETTING_SEPARATESOURCE].values[0] > 0;
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

static inline OutputChannel&
addChannel(std::vector<OutputChannel>& channels,
           std::string outName, bool isStart,
           int sourceIndexA, int sourceIndexB) {
  OutputChannel channel;
  channel.outName = outName;
  channel.outIndex = NUM_MAIN_OUTS + channels.size();
  channel.isStart = isStart;
  channel.sourceIndex.first = sourceIndexA;
  channel.sourceIndex.second = sourceIndexB;
  channels.push_back(channel);
  return channels.back();
}

struct ChannelRefPair {
  int chan1ListIndex;
  int chan2ListIndex;
};

typedef std::map<std::string, ChannelRefPair> ChannelMap;

static inline void
addChannelPair(std::vector<OutputChannel>& channels,
               const std::string& sourceName,
               const std::string& outNameBase,
               int sourceIndexA,
               int sourceIndexB,
               ChannelMap* channelsByName) {
  addChannel(channels, outNameBase + '1',
             true, sourceIndexA, sourceIndexB);
  addChannel(channels, outNameBase + '2',
             true, sourceIndexA, sourceIndexB);
  if (channelsByName) {
    ChannelRefPair pair;
    pair.chan1ListIndex = channels.size() - 2;
    pair.chan2ListIndex = channels.size() - 1;
    (*channelsByName)[sourceName] = pair;
  }
}

bool ThresholderCHOP::shouldLoadChannels(const CHOP_InputArrays* inputs) const {
  if (!hasEnoughInputs(inputs))
    return false;
  if (_xInputIndex.first == -1)
    return true;
  if (inputs->floatInputs[SETTING_RESET_CHANS].values[0] != 0)
    return true;
  return false;
}

void ThresholderCHOP::loadChannels(const CHOP_InputArrays *inputs) {
  auto& chopIn = inputs->CHOPInputs[0];
  for (int i = 0; i < chopIn.numChannels; ++i) {
    const auto& name = chopIn.names[i];
    if (strcmp(name, "x") == 0) {
      _xInputIndex.first = i;
      addChannel(_pointChannels, "tx1", true, i, -1);
      addChannel(_pointChannels, "tx2", false, i, -1);
    } else if (strcmp(name, "y") == 0) {
      _yInputIndex.first = i;
      addChannel(_pointChannels, "ty1", true, i, -1);
      addChannel(_pointChannels, "ty2", false, i, -1);
    } else if (strcmp(name, "z") == 0) {
      _zInputIndex.first = i;
      addChannel(_pointChannels, "tz1", true, i, -1);
      addChannel(_pointChannels, "tz2", false, i, -1);
    } else {
      addChannel(_pointChannels, std::string(name) + '1',
                  true, i, -1);
      addChannel(_pointChannels, std::string(name) + '2',
                  false, i, -1);
    }
  }
}

void ThresholderCHOP::loadChannelsSeparate(const CHOP_InputArrays *inputs) {
  auto& chopInA = inputs->CHOPInputs[0];
  auto& chopInB = inputs->CHOPInputs[1];
  ChannelMap channelsByName;
  for (int i = 0; i < chopInA.numChannels; ++i) {
    std::string name = chopInA.names[i];
    std::string outNameBase;
    if (name == "x") {
      _xInputIndex.first = i;
      outNameBase = "tx";
    } else if (name == "y") {
      _yInputIndex.first = i;
      outNameBase = "ty";
    } else if (name == "z") {
      _zInputIndex.first = i;
      outNameBase = "tz";
    } else {
      outNameBase = name;
    }
    addChannelPair(_pointChannels, name, outNameBase,
                   i, -1, &channelsByName);
  }
  for (int i = 0; i < chopInB.numChannels; ++i) {
    std::string name = chopInB.names[i];
    auto channelsIter = channelsByName.find(name);
    if (channelsIter == channelsByName.end()) {
      addChannelPair(_pointChannels, name, name, -1, i, NULL);
    } else {
      auto& channels = channelsIter->second;
      _pointChannels[channels.chan1ListIndex].sourceIndex.second = i;
      _pointChannels[channels.chan2ListIndex].sourceIndex.second = i;
    }
    if (name == "x") {
      _xInputIndex.second = i;
    } else if (name == "y") {
      _yInputIndex.second = i;
    } else if (name == "z") {
      _zInputIndex.second = i;
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
  if (!hasEnoughInputs(info->inputArrays))
    return false;
  loadParameters(info->inputArrays->floatInputs);
  if (shouldLoadChannels(info->inputArrays)) {
    _xInputIndex.first = _xInputIndex.second = 0;
    _yInputIndex.first = _yInputIndex.second = 1;
    _zInputIndex.first = _zInputIndex.second = 2;
    _pointChannels.clear();
    if (_thresholder.params().useSeparateSource) {
      loadChannelsSeparate(info->inputArrays);
    } else {
      loadChannels(info->inputArrays);
    }
  }
  info->numChannels = NUM_MAIN_OUTS + static_cast<int>(_pointChannels.size());
  CHOPInputPointSet pointsA(&info->inputArrays->CHOPInputs[0],
                           _xInputIndex.first,
                           _yInputIndex.first,
                           _zInputIndex.first);
  if (info->inputArrays->numCHOPInputs > 1) {
    CHOPInputPointSet pointsB(&info->inputArrays->CHOPInputs[1],
                              _xInputIndex.second,
                              _yInputIndex.second,
                              _zInputIndex.second);
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
    auto val = inChans[outChan.sourceIndex.first][srcIndex];
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
    float val = 0;
    if (outChan.isStart) {
      if (outChan.sourceIndex.first >= 0)
        val = inChansA[outChan.sourceIndex.first][startIndex];
    } else {
      if (outChan.sourceIndex.second >= 0)
        val = inChansB[outChan.sourceIndex.second][endIndex];
    }
    channels[outChan.outIndex][i] = val;
  }
}

void ThresholderCHOP::execute(const CHOP_Output *outputs,
                              const CHOP_InputArrays *inputs,
                              void *reserved) {
  if (_lines.empty()) {
    ThreshLine dummy;
    dummy.startIndex = 0;
    dummy.endIndex = 0;
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

