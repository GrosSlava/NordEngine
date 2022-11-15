
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
#include "PreprocessorHelpers.h"

//Templates include
#include "TypeHash.h"

//Math include
#include "EngineMath.h"
#include "MathUtility.h"
#include "NumericLimits.h"

//Memory include
#include "EngineMemoryDefs.h"
#include "GarbageCollector.h"
#include <memory>

//Delegate include
#include "Delegate.h"

//Time
#include <chrono>

//Containers include
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Transform.h"
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
//TODO




//Platform native library
#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#endif