//
//  ThresholderCHOP.h
//  threshold
//
//  Created by tekt on 1/9/15.
//
//

#ifndef __threshold__ThresholderCHOP__
#define __threshold__ThresholderCHOP__

#include "ChopCPP_wrapper.h"

#include "Thresholder.h"
#include "LineSet.h"

class ThresholderCHOP : public CHOP_CPlusPlusBase {
public:
  ThresholderCHOP(const CHOP_NodeInfo* info);
  virtual ~ThresholderCHOP();
  
  void		getGeneralInfo(CHOP_GeneralInfo* info) override;
  bool		getOutputInfo(CHOP_OutputInfo* info) override;
  const char*	getChannelName(int index, void* reserved) override;
  
  void		execute(const CHOP_Output* outputs,
                  const CHOP_InputArrays* inputs,
                  void* reserved) override;
  
  int			getNumInfoCHOPChans() override;
  void		getInfoCHOPChan(int index,
                          CHOP_InfoCHOPChan *chan) override;
private:
  Thresholder _thresholder;
  LineSet _lines;
};

#endif /* defined(__threshold__ThresholderCHOP__) */
