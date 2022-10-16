
#pragma once




//Misc include
#include "Build.h"
#include "CoreFWD.h"
#include "EngineVersion.h"

//Platform include
#include "GenericPlatform.h"
#include "GenericPlatformUtils.h"

//Macros include
#include "SpecificationMacros.h"
#include "CommonMacros.h"
#include "AssertionMacros.h"

//Math include
#include "EngineMath.h"
#include "MathUtility.h"
//#include "NumericLimits.h"

//Memory include
#include "EngineMemoryDefs.h"

//Delegate include
#include "Delegate.h"

//Containers include
#include "Vector2D.h"
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>



#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#endif