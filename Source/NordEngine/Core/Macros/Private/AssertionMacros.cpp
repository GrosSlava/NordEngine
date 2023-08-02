// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "AssertionMacros.h"
#include "GenericPlatformMisc.h"





void FCheckCodePrivate::ShowStackTrace(const TCHAR* S)
{
	FPlatformMisc::DefaultShowStackTrace(S, 1);
}
