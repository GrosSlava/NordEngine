
#pragma once

#include "GenericPlatform.h"




/*
	TIsCharType.
*/
template<typename T> struct TIsCharType				{ enum { Value = false }; };
template<>           struct TIsCharType<ANSICHAR>	{ enum { Value = true }; };
template<>           struct TIsCharType<UCS2CHAR>	{ enum { Value = true }; };
template<>           struct TIsCharType<WIDECHAR>	{ enum { Value = true }; };
