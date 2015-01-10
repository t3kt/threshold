//
//  ChopCPP_wrapper.h
//  threshold
//
//  Created by tekt on 1/9/15.
//
//

#ifndef threshold_ChopCPP_wrapper_h
#define threshold_ChopCPP_wrapper_h

#if defined(WIN32) || defined(_WIN64)

#include <Windows.h>

#else

#define _WIN64

typedef void* HWND;

#endif

#include "CHOP_CPlusPlusBase.h"

#endif
