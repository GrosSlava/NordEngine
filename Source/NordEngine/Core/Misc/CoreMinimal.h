// Copyright Nord Engine. All Rights Reserved.
#pragma once




// Misc include
#include "Build.h"
#include "EngineVersion.h"

// Platform include
#include "GenericPlatform.h"
#include "GenericPlatformUtils.h"

// Macros include
#include "SpecificationMacros.h"
#include "CommonMacros.h"
#include "AssertionMacros.h"
#include "PreprocessorHelpers.h"

// Templates include
#include "TypeHash.h"

// Math include
#include "EngineMath.h"
#include "MathUtility.h"
#include "NumericLimits.h"

// Memory include
#include "EngineMemoryDefs.h"
#include "EngineMemory.h"

// Time
#include <chrono>
//TODO

// Containers include
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Transform.h"
#include "Delegate.h"
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
//TODO

// Files include
#include "Path.h"



// Platform native library
// clang-format off
#if PLATFORM_WINDOWS
	#include "Windows/WindowsHWrapper.h"
#else
	#error "Undefined platform!"
#endif
// clang-format on