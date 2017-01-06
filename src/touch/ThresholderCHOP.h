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
  explicit ThresholderCHOP(const OP_NodeInfo* info);
  virtual ~ThresholderCHOP() {}

  virtual void		getGeneralInfo(CHOP_GeneralInfo*) override;
  virtual bool		getOutputInfo(CHOP_OutputInfo*) override;
  virtual const char*	getChannelName(int index, void* reserved) override;

  virtual void		execute(const CHOP_Output*,
    OP_Inputs*,
    void* reserved) override;

  virtual int			getNumInfoCHOPChans() override;
  virtual void		getInfoCHOPChan(int index,
    OP_InfoCHOPChan* chan) override;

  virtual bool		getInfoDATSize(OP_InfoDATSize* infoSize) override {
    return false;
  }
  virtual void		getInfoDATEntries(int index,
    int nEntries,
    OP_InfoDATEntries* entries) override {}

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
