// Copyright Nord Engine. All Rights Reserved.
#include "Array.h"
#include "TestHelpers.h"





int Core_ArrayTest(int argc, char* argv[])
{
	{
		TArray<TestSimpleType> arr(4);
		TestEqual(arr.GetSize(), arr.Num());
		TestEqual(arr.GetSize(), 4);
		TestEqual(arr.IsEmpty(), false);
		Test(arr.GetCapacity() >= arr.Num());

		arr.ShrinkToFit();
		TestEqual(arr.GetSize(), 4);
		TestEqual(arr.IsEmpty(), false);
		TestEqual(arr.GetCapacity(), arr.Num());

		arr.Clear();
		TestEqual(arr.GetSize(), 0);
		TestEqual(arr.IsEmpty(), true);
		TestEqual(arr.GetCapacity(), 4);

		arr.Reset();
		TestEqual(arr.GetSize(), 0);
		TestEqual(arr.IsEmpty(), true);
		TestEqual(arr.GetCapacity(), 0);
	}

	{
		TArray<TestSimpleType> arr;
		TestEqual(arr.GetSize(), arr.Num());
		TestEqual(arr.GetSize(), 0);
		TestEqual(arr.IsEmpty(), true);
		TestEqual(arr.GetCapacity(), 0);

		arr.ShrinkToFit();
		TestEqual(arr.GetSize(), 0);
		TestEqual(arr.IsEmpty(), true);
		TestEqual(arr.GetCapacity(), arr.Num());

		arr.Clear();
		TestEqual(arr.GetSize(), 0);
		TestEqual(arr.IsEmpty(), true);
		TestEqual(arr.GetCapacity(), 0);

		arr.Reset();
		TestEqual(arr.GetSize(), 0);
		TestEqual(arr.IsEmpty(), true);
		TestEqual(arr.GetCapacity(), 0);
	}

	return PROGRAM_EXIT_SUCCESS;
}