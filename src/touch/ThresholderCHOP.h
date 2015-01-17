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

#include "ChopCPP_wrapper.h"

#include "Thresholder.h"
#include "PointSet.h"
#include "LineSet.h"

struct OutputChannel {
  std::string outName;
  std::size_t outIndex;
  bool isStart;
  std::size_t sourceIndex;
};

class ThresholderCHOP : public CHOP_CPlusPlusBase {
public:
  ThresholderCHOP(const CHOP_NodeInfo* info) : _xInputIndex(-1), _yInputIndex(-1), _zInputIndex(-1) {}
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
  int _xInputIndex;
  int _yInputIndex;
  int _zInputIndex;
  std::vector<OutputChannel> _pointChannels;
};

#endif /* defined(__threshold__ThresholderCHOP__) */
