
#pragma once

#include <stdarg.h>
#include "MicrosoftPlatformString.h"




#define GET_VARARGS(msg, msgsize, len, lastarg, fmt)                 \
	{                                                                \
		va_list ap;                                                  \
		va_start(ap, lastarg);                                       \
		FMicrosoftPlatformString::GetVarArgs(msg, msgsize, fmt, ap); \
		va_end(ap);                                                  \
	}

#define GET_VARARGS_RESULT(msg, msgsize, len, lastarg, fmt, result)           \
	{                                                                         \
		va_list ap;                                                           \
		va_start(ap, lastarg);                                                \
		result = FMicrosoftPlatformString::GetVarArgs(msg, msgsize, fmt, ap); \
		if( result >= msgsize )                                               \
		{                                                                     \
			result = -1;                                                      \
		}                                                                     \
		va_end(ap);                                                           \
	}
