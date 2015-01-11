//
//  Common.h
//  threshold
//
//  Created by tekt on 1/7/15.
//
//

#ifndef __threshold__Common__
#define __threshold__Common__

#ifdef OF_VERSION_MAJOR

#define USING_OPEN_FRAMEWORKS

#endif

#if (_MSC_VER)
#include <memory>
#else
#include <tr1/memory>
// import smart pointers utils into std
namespace std {
#if __cplusplus<201103L
  using std::tr1::shared_ptr;
#endif
}
#endif


#endif /* defined(__threshold__Common__) */
