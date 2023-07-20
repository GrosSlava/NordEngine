// Copyright Nord Engine. All Rights Reserved.
#pragma once




/**
	Generic axis enum.
*/
enum class EAxis : unsigned short int
{
	None,
	X,
	Y,
	Z
};

/**
	Extended axis enum for more specialized usage.
*/
enum class EAxisList : unsigned short int
{
	None = 0,
	X = 1,
	Y = 2,
	Z = 4,

	XY = X | Y,
	XZ = X | Z,
	YZ = Y | Z,
	XYZ = X | Y | Z
};
