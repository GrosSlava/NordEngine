
#pragma once




/* 
	These defines are used to mark a difference between two pointers as expected to fit into the specified range
	while still leaving something searchable if the surrounding code is updated to work with a 64 bit count/range
	in the future.
*/
#define PTRDIFF_TO_INT32(argument) static_cast<int32>(argument)
#define PTRDIFF_TO_UINT32(argument) static_cast<uint32>(argument)


#define INDEX_NONE -1
#define NAME_NONE "None"