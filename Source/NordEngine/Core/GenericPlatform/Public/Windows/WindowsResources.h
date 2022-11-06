#pragma once

#include "GenericPlatform.h"

#if !PLATFORM_WINDOWS
	#error PLATFORM_WINDOWS not defined!
#endif




#define IDD_YESNO2ALL                   101
#define IDD_YESNO2ALLCANCEL             102
#define IDD_YESNOYESTOALL               103
#define IDICON			                123
#define IDC_YESTOALL                    1000
#define IDC_NOTOALL                     1001
#define IDC_YES                         1002
#define IDC_NO_B                        1003
#define IDC_MESSAGE                     1004
#define IDC_CANCEL                      1009
#define ID_HASHFILE                     1010