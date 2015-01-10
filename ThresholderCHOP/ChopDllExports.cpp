#include "ThresholderCHOP.h"

// These functions are basic C function, which the DLL loader can find
// much easier than finding a C++ Class.
// The DLLEXPORT prefix is needed so the compile exports these functions from the .dll
// you are creating
extern "C"
{

  DLLEXPORT int GetCHOPAPIVersion(void)
  {
    // Always return CHOP_CPLUSPLUS_API_VERSION in this function.
    return CHOP_CPLUSPLUS_API_VERSION;
  }

  DLLEXPORT CHOP_CPlusPlusBase* CreateCHOPInstance(const CHOP_NodeInfo *info)
  {
    // Return a new instance of your class every time this is called.
    // It will be called once per CHOP that is using the .dll
    return new ThresholderCHOP(info);
  }

  DLLEXPORT void DestroyCHOPInstance(CHOP_CPlusPlusBase *instance)
  {
    // Delete the instance here, this will be called when
    // Touch is shutting down, when the CHOP using that instance is deleted, or
    // if the CHOP loads a different DLL
    delete (ThresholderCHOP*)instance;
  }

};
