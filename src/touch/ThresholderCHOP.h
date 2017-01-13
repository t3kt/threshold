//
//  ThresholderCHOP.h
//  threshold
//
//  Created by tekt on 1/9/15.
//
//

#ifndef __threshold__ThresholderCHOP__
#define __threshold__ThresholderCHOP__

#include <string>
#include <vector>
#include <utility>

#include "ChopCPP_wrapper.h"

#include "Thresholder.h"
#include "TouchCommon.h"
#include "PointSet.h"
#include "LineSet.h"

typedef std::pair<int, int> IndexPair;

struct OutputChannel {
  std::string outName;
  int outIndex;
  bool isStart;
  IndexPair sourceIndex;
};

using TouchThresholder = Thresholder<TouchPoint, CHOPInputPointSet>;

class ThresholderCHOP : public CHOP_CPlusPlusBase {

  //enum {
  //  INFO_ROW_WARNING,
  //  INFO_ROW_RESULT,

  //  NUM_INFO_ROWS
  //};
public:
  explicit ThresholderCHOP(const OP_NodeInfo* info);
  virtual ~ThresholderCHOP() {}

  virtual void		getGeneralInfo(CHOP_GeneralInfo*) override;
  virtual bool		getOutputInfo(CHOP_OutputInfo*) override;
  virtual const char* getErrorString() override {
    if (_error.empty()) {
      return nullptr;
    }
    return _error.c_str();
  }
  virtual const char* getWarningString() override {
    if (_warning.empty()) {
      return nullptr;
    }
    return _warning.c_str();
  }
  virtual const char* getInfoPopupString() override {
    if (_info.empty()) {
      return nullptr;
    }
    return _info.c_str();
  }

  virtual const char*	getChannelName(int index, void* reserved) override;

  virtual void		execute(const CHOP_Output*,
    OP_Inputs*,
    void* reserved) override;

  virtual void		setupParameters(OP_ParameterManager* manager) override;
  virtual void		pulsePressed(const char* name) override;
private:
  void loadParameters(OP_Inputs* inputs);
  bool shouldLoadChannels(OP_Inputs* inputs) const;
  void loadChannels(OP_Inputs* inputs);
  void loadChannelsSeparate(OP_Inputs* inputs);
  void outputLineSingle(const ThreshLine& line,
                        std::size_t i,
                        float** channels,
                        OP_Inputs *inputs) const;
  void outputLineSeparate(const ThreshLine& line,
                          std::size_t i,
                          float** channels,
                          OP_Inputs *inputs) const;
  
  TouchThresholder _thresholder;
  LineSet _lines;
  OutputChannel _xChannel;
  OutputChannel _yChannel;
  OutputChannel _zChannel;
  IndexPair _xInputIndex;
  IndexPair _yInputIndex;
  IndexPair _zInputIndex;
  std::vector<OutputChannel> _pointChannels;
  bool _resetChans;
  //SimpleInfoTable<NUM_INFO_ROWS> _debugInfo;
  bool _debugEnabled;
  std::string _error;
  std::string _warning;
  std::string _info;
};

#endif /* defined(__threshold__ThresholderCHOP__) */
