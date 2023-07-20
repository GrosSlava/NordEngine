// Copyright Nord Engine. All Rights Reserved.
#pragma once




/**
	Count of bytes in one KB.
*/
#define KB_SZ (1024)
/**
	Count of bytes in one MB.
*/
#define MB_SZ (KB_SZ * 1024)
/**
	Count of bytes in one GB.
*/
#define GB_SZ (MB_SZ * 1024)


#define ONE_BYTE_SHIFT 0
#define BYTES(x) ((x) << ONE_BYTE_SHIFT)

#define ONE_KILOBYTE_SHIFT 10
#define KILOBYTES(x) ((x) << ONE_KILOBYTE_SHIFT)

#define ONE_MEGABYTE_SHIFT 20
#define MEGABYTES(x) ((x) << ONE_MEGABYTE_SHIFT)

#define ONE_GIGABYTE_SHIFT 30
#define GIGABYTES(x) ((x) << ONE_GIGABYTE_SHIFT)