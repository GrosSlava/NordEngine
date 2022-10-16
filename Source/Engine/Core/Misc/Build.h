
#pragma once




#ifdef _DEBUG
	#define BUILD_DEBUG 1
	#define BUILD_RELEASE 0
#endif
#ifdef NDEBUG
	#define BUILD_DEBUG 0
	#define BUILD_RELEASE 1
#endif


#ifndef BUILD_DEBUG
	#define BUILD_DEBUG 0
#endif
#ifndef BUILD_RELEASE
	#define BUILD_RELEASE 0
#endif


// Ensure that we have one, and only one build config coming from UBT
#if BUILD_DEBUG + BUILD_RELEASE != 1
	#error Exactly one of [BUILD_DEBUG BUILD_RELEASE] should be defined to be 1
#endif




/*--------------------------------------------------------------------------------
	Basic options that by default depend on the build configuration and platform

	DO_CHECK							If true, then checkCode, checkf, verify, check, checkNoEntry, checkNoReentry, checkNoRecursion, verifyf, checkf are compiled into the executables
	DO_ENSURE							If true, then ensure, ensureAlways, ensureMsgf and ensureAlwaysMsgf are compiled into the executables
	NO_LOGGING							If true, then no logs or text output will be produced

--------------------------------------------------------------------------------*/

#if BUILD_DEBUG
	#ifndef DO_CHECK
		#define DO_CHECK 1
	#endif //DO_CHECK
	#ifndef DO_ENSURE
		#define DO_ENSURE 1
	#endif //DO_ENSURE
	#ifndef NO_LOGGING
		#define NO_LOGGING 0
	#endif //NO_LOGGING
#elif BUILD_RELEASE
#ifndef NDEBUG
	#define NDEBUG 1
#endif
	#ifndef DO_CHECK
		#define DO_CHECK 0
	#endif //DO_CHECK
	#ifndef DO_ENSURE
		#define DO_ENSURE 0
	#endif //DO_ENSURE
	#ifndef NO_LOGGING
		#define NO_LOGGING 1
	#endif //NO_LOGGING
#else
	#error Exactly one of [BUILD_DEBUG BUILD_DEVELOPMENT BUILD_SHIPPING] should be defined to be 1
#endif