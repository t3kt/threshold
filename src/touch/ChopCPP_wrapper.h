//
//  ChopCPP_wrapper.h
//  threshold
//
//  Created by tekt on 1/9/15.
//
//

#ifndef threshold_ChopCPP_wrapper_h
#define threshold_ChopCPP_wrapper_h

#ifdef WIN32

#include <Windows.h>

#else

#define _WIN64

typedef void* HWND;

#endif

#include "CHOP_CPlusPlusBase.h"

#endif
