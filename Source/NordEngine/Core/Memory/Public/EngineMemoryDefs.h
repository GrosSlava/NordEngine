
#pragma once




/* Memory constants in bytes */

#define KB (1024)
#define MB (KB * 1024)
#define GB (MB * 1024)


#define ONE_BYTE_SHIFT 0
#define ONE_BYTE (1 << ONE_BYTE_SHIFT)
#define BYTES(x) ((x) << ONE_BYTE_SHIFT)

#define ONE_KILOBYTE_SHIFT 10
#define ONE_KILOBYTE (1 << ONE_KILOBYTE_SHIFT)
#define KILOBYTES(x) ((x) << ONE_KILOBYTE_SHIFT)

#define ONE_MEGABYTE_SHIFT 20
#define ONE_MEGABYTE (1 << ONE_MEGABYTE_SHIFT)
#define MEGABYTES(x) ((x) << ONE_MEGABYTE_SHIFT)

#define ONE_GIGABYTE_SHIFT 30
#define ONE_GIGABYTE (1 << ONE_GIGABYTE_SHIFT)
#define GIGABYTES(x) ((x) << ONE_GIGABYTE_SHIFT)