// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <memory>

template <class T>
using uptr = std::unique_ptr<T>;

template <class T>
using sptr = std::shared_ptr<T>;

template <class T>
using wptr = std::weak_ptr<T>;

// TODO: reference additional headers your program requires here
