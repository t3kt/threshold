//
//  ThresholderCHOP.cpp
//  threshold
//
//  Created by tekt on 1/9/15.
//
//

#include "ThresholderCHOP.h"

#include <assert.h>
#include <string.h>
#include <map>

#include "Parameters.h"
#include "Point.h"
#include "Line.h"
#include "TouchCommon.h"

enum {
  OUT_INDEX1,
  OUT_INDEX2,
  OUT_SQRDIST,
  OUT_CLOSENESS,
  
  NUM_MAIN_OUTS
};

ThresholderCHOP::ThresholderCHOP(const OP_NodeInfo* info)
: _xInputIndex(-1, -1)
, _yInputIndex(-1, -1)
, _zInputIndex(-1, -1)
, _resetChans(false) {
  //_debugInfo.setLabel(INFO_ROW_STATUS, "status");
}

static const char* PAR_PAGE = "Threshold";

void ThresholderCHOP::setupParameters(OP_ParameterManager* manager) {
  // Hasmindist
  {
    auto par = OP_NumericParameter("Hasmindist");
    par.label = "Has Minimum Distance";
    par.defaultValues[0] = 1.0;
    par.page = PAR_PAGE;

    auto result = manager->appendToggle(par);
    assert(result == PARAMETER_APPEND_SUCCESS);
  }
  // Mindist
  {
    auto par = OP_NumericParameter("Mindist");
    par.label = "Minimum Distance";
    par.defaultValues[0] = 0.0;
    par.clampMins[0] = true;
    par.minValues[0] = 0.0;
    par.minSliders[0] = 0.0;
    par.maxSliders[0] = 5.0;
    par.page = PAR_PAGE;

    auto result = manager->appendFloat(par, 1);
    assert(result == PARAMETER_APPEND_SUCCESS);
  }
  // Hasmaxdist
  {
    auto par = OP_NumericParameter("Hasmaxdist");
    par.label = "Has Maximum Distance";
    par.defaultValues[0] = 1.0;
    par.page = PAR_PAGE;

    auto result = manager->appendToggle(par);
    assert(result == PARAMETER_APPEND_SUCCESS);
  }
  // Maxdist
  {
    auto par = OP_NumericParameter("Maxdist");
    par.label = "Maximum Distance";
    par.defaultValues[0] = 1.0;
    par.clampMins[0] = true;
    par.minValues[0] = 0.0;
    par.minSliders[0] = 0.0;
    par.maxSliders[0] = 5.0;
    par.page = PAR_PAGE;

    auto result = manager->appendFloat(par, 1);
    assert(result == PARAMETER_APPEND_SUCCESS);
  }
  // Maxlines
  {
    auto par = OP_NumericParameter("Maxlines");
    par.label = "Maximum Lines";
    par.defaultValues[0] = 1000.0;
    par.minSliders[0] = 1.0;
    par.minValues[0] = 1.0;
    par.maxSliders[0] = 10000.0;
    par.clampMins[0] = true;
    par.page = PAR_PAGE;

    auto result = manager->appendInt(par, 1);
    assert(result == PARAMETER_APPEND_SUCCESS);
  }
  // Hasmaxpersource
  {
    auto par = OP_NumericParameter("Hasmaxpersource");
    par.label = "Has Maximum Lines Per Source";
    par.defaultValues[0] = 0.0;
    par.page = PAR_PAGE;

    auto result = manager->appendToggle(par);
    assert(result == PARAMETER_APPEND_SUCCESS);
  }
  // Maxpersource
  {
    auto par = OP_NumericParameter("Maxpersource");
    par.label = "Maximum Lines Per Source";
    par.defaultValues[0] = 10.0;
    par.minSliders[0] = 1.0;
    par.minValues[0] = 1.0;
    par.maxSliders[0] = 1000.0;
    par.clampMins[0] = true;
    par.page = PAR_PAGE;

    auto result = manager->appendInt(par, 1);
    assert(result == PARAMETER_APPEND_SUCCESS);
  }
  // Useseparatesource
  {
    auto par = OP_NumericParameter("Useseparatesource");
    par.label = "Use Separate Source";
    par.defaultValues[0] = 0.0;
    par.page = PAR_PAGE;

    auto result = manager->appendToggle(par);
    assert(result == PARAMETER_APPEND_SUCCESS);
  }
  // Distinctgroups
  {
    auto par = OP_NumericParameter("Distinctgroups");
    par.label = "Require Distinct Groups";
    par.defaultValues[0] = 0.0;
    par.page = PAR_PAGE;

    auto result = manager->appendToggle(par);
    assert(result == PARAMETER_APPEND_SUCCESS);
  }
  // Resetchans
  {
    auto par = OP_NumericParameter("Resetchans");
    par.label = "Reset Channels";
    par.defaultValues[0] = 0.0;
    par.page = PAR_PAGE;

    auto result = manager->appendToggle(par);
    assert(result == PARAMETER_APPEND_SUCCESS);
  }
  // Resetchanspulse
  {
    auto par = OP_NumericParameter("Resetchanspulse");
    par.label = "Reset Channels (pulse)";
    par.page = PAR_PAGE;

    auto result = manager->appendPulse(par);
    assert(result == PARAMETER_APPEND_SUCCESS);
  }
  // Debug
  {
    auto par = OP_NumericParameter("Debug");
    par.label = "Enable Debugging";
    par.page = PAR_PAGE;
    par.defaultValues[0] = 0.0;

    auto result = manager->appendToggle(par);
    assert(result == PARAMETER_APPEND_SUCCESS);
  }
}

void
ThresholderCHOP::loadParameters(OP_Inputs* inputs) {
  ThreshParameters params;
  if (inputs->getParInt("Hasmindist")) {
    inputs->enablePar("Mindist", true);
    params.minDist = static_cast<float>(inputs->getParDouble("Mindist"));
  } else {
    inputs->enablePar("Mindist", false);
    params.minDist = -1.0;
  }
  if (inputs->getParInt("Hasmaxdist")) {
    inputs->enablePar("Maxdist", true);
    params.maxDist = static_cast<float>(inputs->getParDouble("Maxdist"));
  } else {
    inputs->enablePar("Maxdist", false);
    params.maxDist = -1.0;
  }
  params.maxLines = inputs->getParInt("Maxlines");
  if (inputs->getParInt("Hasmaxpersource")) {
    inputs->enablePar("Maxpersource", true);
    params.maxLinesPerSource = inputs->getParInt("Maxpersource");
  } else {
    inputs->enablePar("Maxpersource", false);
    params.maxLinesPerSource = -1;
  }

  params.useSeparateSource = inputs->getParInt("Useseparatesource") && inputs->getNumInputs() > 1;

  params.distinctGroups = inputs->getParInt("Distinctgroups") != 0;
  
  _thresholder.configure(params);
}

static bool
hasEnoughInputs(OP_Inputs* inputs) {
  if (inputs->getNumInputs() < 1)
    return false;
  const auto chop = inputs->getInputCHOP(0);
  if (chop->numChannels < 3 || chop->numSamples <= 1)
    return false;
  return true;
}

static inline OutputChannel&
addChannel(std::vector<OutputChannel>& channels,
           std::string outName, bool isStart,
           int sourceIndexA, int sourceIndexB) {
  OutputChannel channel;
  channel.outName = outName;
  channel.outIndex = NUM_MAIN_OUTS + static_cast<int>(channels.size());
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
    pair.chan1ListIndex = static_cast<int>(channels.size()) - 2;
    pair.chan2ListIndex = static_cast<int>(channels.size()) - 1;
    (*channelsByName)[sourceName] = pair;
  }
}

bool ThresholderCHOP::shouldLoadChannels(OP_Inputs* inputs) const {
  if (!hasEnoughInputs(inputs))
    return false;
  if (_xInputIndex.first == -1)
    return true;
  if (inputs->getParInt("Resetchans") != 0 || _resetChans) {
    return true;
  }
  return false;
}

void ThresholderCHOP::loadChannels(OP_Inputs *inputs) {
  auto chopIn = inputs->getInputCHOP(0);
  _groupIndex.first = -1;
  for (int i = 0; i < chopIn->numChannels; ++i) {
    const auto& name = chopIn->getChannelName(i);
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
    } else if (strcmp(name, "group") == 0) {
      _groupIndex.first = i;
      addChannel(_pointChannels, "group1", true, i, -1);
      addChannel(_pointChannels, "group1", false, i, -1);
    } else {
      addChannel(_pointChannels, std::string(name) + '1',
                  true, i, -1);
      addChannel(_pointChannels, std::string(name) + '2',
                  false, i, -1);
    }
  }
}

void ThresholderCHOP::loadChannelsSeparate(OP_Inputs *inputs) {
  auto chopInA = inputs->getInputCHOP(0);
  auto chopInB = inputs->getInputCHOP(1);
  ChannelMap channelsByName;
  _groupIndex.first = -1;
  for (int i = 0; i < chopInA->numChannels; ++i) {
    std::string name = chopInA->getChannelName(i);
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
    } else if (name == "group") {
      _groupIndex.first = i;
      outNameBase = name;
    } else {
      outNameBase = name;
    }
    addChannelPair(_pointChannels, name, outNameBase,
                   i, -1, &channelsByName);
  }
  _groupIndex.second = -1;
  for (int i = 0; i < chopInB->numChannels; ++i) {
    std::string name = chopInB->getChannelName(i);
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
    } else if (name == "group") {
      _groupIndex.second = i;
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
  info->numSamples = 0;
  _error.clear();
  _warning.clear();
  _info.clear();
  //_debugEnabled = info->opInputs->getParInt("Debug");
  //if (_debugEnabled) {
  //  _debugInfo.clearValues();
  //}
  if (!hasEnoughInputs(info->opInputs)) {
    if (_debugEnabled) {
      //_debugInfo[INFO_ROW_STATUS] = "not enough inputs";
      _error = "not enough inputs";
    }
    return false;
  }
  loadParameters(info->opInputs);
  if (shouldLoadChannels(info->opInputs)) {
    _xInputIndex.first = _xInputIndex.second = 0;
    _yInputIndex.first = _yInputIndex.second = 1;
    _zInputIndex.first = _zInputIndex.second = 2;
    _pointChannels.clear();
    if (_thresholder.params().useSeparateSource) {
      loadChannelsSeparate(info->opInputs);
    } else {
      loadChannels(info->opInputs);
    }
    _resetChans = false;
  }
  info->numChannels = NUM_MAIN_OUTS + static_cast<int>(_pointChannels.size());
  CHOPInputPointSet pointsA(info->opInputs->getInputCHOP(0),
                           _xInputIndex.first,
                           _yInputIndex.first,
                           _zInputIndex.first,
                           _groupIndex.first);
  if (info->opInputs->getNumInputs() > 1) {
    _info = "generating for separate inputs";
    CHOPInputPointSet pointsB(info->opInputs->getInputCHOP(1),
                              _xInputIndex.second,
                              _yInputIndex.second,
                              _zInputIndex.second,
                              _groupIndex.second);
    _thresholder.generate(&pointsA, &pointsB, &_lines);
  } else {
    _info = "generating for single input";
    _thresholder.generate(&pointsA, NULL, &_lines);
  }
  if (_lines.empty()) {
    info->numSamples = 1;
  } else {
    info->numSamples = static_cast<int>(_lines.size());
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
                                       OP_Inputs *inputs) const {
  const auto& startIndex = line.startIndex;
  const auto& endIndex = line.endIndex;
  channels[OUT_INDEX1][i] = static_cast<float>(startIndex);
  channels[OUT_INDEX2][i] = static_cast<float>(endIndex);
  channels[OUT_SQRDIST][i] = line.squareDistance;
  channels[OUT_CLOSENESS][i] = line.closeness;
  auto inCHOP = inputs->getInputCHOP(0);
  for (const auto& outChan : _pointChannels) {
    auto srcIndex = outChan.isStart ? startIndex : endIndex;
    auto val = inCHOP->getChannelData(outChan.sourceIndex.first)[srcIndex];
    channels[outChan.outIndex][i] = val;
  }
}

void ThresholderCHOP::outputLineSeparate(const ThreshLine& line,
                                         std::size_t i,
                                         float **channels,
                                         OP_Inputs *inputs) const {
  const auto& startIndex = line.startIndex;
  const auto& endIndex = line.endIndex;
  channels[OUT_INDEX1][i] = static_cast<float>(startIndex);
  channels[OUT_INDEX2][i] = static_cast<float>(endIndex);
  channels[OUT_SQRDIST][i] = line.squareDistance;
  channels[OUT_CLOSENESS][i] = line.closeness;
  auto inCHOPA = inputs->getInputCHOP(0);
  auto inCHOPB = inputs->getInputCHOP(1);
  for (const auto& outChan : _pointChannels) {
    float val = 0;
    if (outChan.isStart) {
      if (outChan.sourceIndex.first >= 0) {
        val = inCHOPA->getChannelData(outChan.sourceIndex.first)[startIndex];
      }
    } else {
      if (outChan.sourceIndex.second >= 0) {
        val = inCHOPB->getChannelData(outChan.sourceIndex.second)[endIndex];
      }
    }
    channels[outChan.outIndex][i] = val;
  }
}

void ThresholderCHOP::execute(const CHOP_Output *outputs,
                              OP_Inputs *inputs,
                              void *reserved) {
  if (!outputs->channels || outputs->numChannels == 0 || outputs->numSamples == 0) {
    return;
  }
  if (_lines.empty()) {
    ThreshLine dummy;
    dummy.startIndex = 0;
    dummy.endIndex = 0;
    outputLineSingle(dummy, 0, outputs->channels, inputs);
  } else {
    auto length = _lines.size();
    if (inputs->getNumInputs() > 1 &&
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

void ThresholderCHOP::pulsePressed(const char* name) {
  if (0 == strcmp(name, "Resetchanspulse")) {
    _resetChans = true;
  }
}

