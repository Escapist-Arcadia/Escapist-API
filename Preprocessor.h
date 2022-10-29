#pragma once

#include<cassert>

#if defined(WIN32) || defined (_WIN32) || defined (__WIN32) || defined(WIN64) || defined (_WIN64) || defined (__WIN64)

#define ESCAPIST_OS_WINDOWS
#include<Windows.h>

//#define ESCAPIST_HAS_NAMESPACE

#if defined(WIN64) || defined (_WIN64) || defined (__WIN64)
#define ESCAPIST_64BITS
#elif defined(WIN32) || defined (_WIN32) || defined (__WIN32)
#define ESCAPIST_32BITS
#else
#define ESCAPIST_BITS_UNDEFINED
#endif

#elif __linux__

#elif __unix__

#endif

#ifdef ESCAPIST_OS_WINDOWS
#endif

using Handle = typename void*;

#if defined(ESCAPIST_64BITS)
using SizeType = typename unsigned long long;
using Char = typename wchar_t;
#elif defined(ESCAPIST_32BITS)
using SizeType = typename unsigned int;
using Char = typename char;
#else
using SizeType = decltype(sizeof(void*));
using Char = typename char;
#endif // ESCAPIST_64BITS

#ifdef ESCAPIST_OS_WINDOWS
#endif

#ifdef ESCAPIST_HAS_NAMESPACE
namespace Escapist
{
#endif
#ifdef ESCAPIST_HAS_NAMESPACE
}
#endif