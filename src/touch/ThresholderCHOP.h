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
#include "PointSet.h"
#include "LineSet.h"

typedef std::pair<int, int> IndexPair;

struct OutputChannel {
  std::string outName;
  int outIndex;
  bool isStart;
  IndexPair sourceIndex;
};

class ThresholderCHOP : public CHOP_CPlusPlusBase {
public:
  explicit ThresholderCHOP(const CHOP_NodeInfo* info);
  virtual ~ThresholderCHOP() {}
  
  void getGeneralInfo(CHOP_GeneralInfo* info) override;
  bool getOutputInfo(CHOP_OutputInfo* info) override;
  const char*	getChannelName(int index, void* reserved) override;
  
  void execute(const CHOP_Output* outputs,
               const CHOP_InputArrays* inputs,
               void* reserved) override;
  
  int getNumInfoCHOPChans() override;
  void getInfoCHOPChan(int index, CHOP_InfoCHOPChan *chan) override;
private:
  void loadParameters(const CHOP_FloatInput* inputs);
  void loadChannels(const CHOP_InputArrays* inputs);
  void loadChannelsSeparate(const CHOP_InputArrays* inputs);
  void outputLineSingle(const ThreshLine& line,
                        std::size_t i,
                        float** channels,
                        const CHOP_InputArrays *inputs) const;
  void outputLineSeparate(const ThreshLine& line,
                          std::size_t i,
                          float** channels,
                          const CHOP_InputArrays *inputs) const;
  
  Thresholder _thresholder;
  LineSet _lines;
  OutputChannel _xChannel;
  OutputChannel _yChannel;
  OutputChannel _zChannel;
  IndexPair _xInputIndex;
  IndexPair _yInputIndex;
  IndexPair _zInputIndex;
  std::vector<OutputChannel> _pointChannels;
};

#endif /* defined(__threshold__ThresholderCHOP__) */
