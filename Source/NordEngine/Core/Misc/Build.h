// Copyright Nord Engine. All Rights Reserved.
#pragma once




// clang-format off
#ifdef _DEBUG
	#define BUILD_DEBUG 1
	#define BUILD_RELEASE 0
#endif
#ifdef NDEBUG
	#define BUILD_DEBUG 0
	#define BUILD_RELEASE 1
#endif
// clang-format on

// clang-format off
#ifndef BUILD_DEBUG
	#define BUILD_DEBUG 0
#endif
#ifndef BUILD_RELEASE
	#define BUILD_RELEASE 0
#endif
// clang-format on

// Ensure that we have one, and only one build config coming from UBT
#if BUILD_DEBUG + BUILD_RELEASE != 1
#error Exactly one of [BUILD_DEBUG BUILD_RELEASE] should be defined to be 1
#endif





/*
	DO_CHECK			If true, then checkCode, checkf, verify, check, checkNoEntry, checkNoReentry, checkNoRecursion, verifyf, checkf are compiled into the executables
	NO_LOGGING			If true, then no logs or text output will be produced
*/
// clang-format off
#if BUILD_DEBUG
	#ifndef DO_CHECK
		#define DO_CHECK 1
	#endif
	#ifndef NO_LOGGING
		#define NO_LOGGING 0
	#endif
#elif BUILD_RELEASE
	#ifndef NDEBUG
		#define NDEBUG 1
	#endif
	#ifndef DO_CHECK
		#define DO_CHECK 0
	#endif
	#ifndef NO_LOGGING
		#define NO_LOGGING 1
	#endif
#else
	#error Exactly one of [BUILD_DEBUG BUILD_DEVELOPMENT BUILD_SHIPPING] should be defined to be 1
#endif
// clang-format on