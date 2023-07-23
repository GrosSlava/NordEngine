// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "EngineMath.h"

#include <random>




struct FVector3D;
struct FVector2D;
struct FBox3D;
struct FBox2D;


struct ENGINE_API FRandom
{
	/** 
		Seeds global random number functions Rand() and FRand() 
	*/
	static FORCEINLINE void RandInit(int32 Seed) noexcept { Generator.seed(Seed); }
	/** 
		@return Random integer between 0 and 2^32, inclusive.
	*/
	static FORCEINLINE int32 Rand() noexcept { return Generator(); }
	/** 
		@return Random integer between 0 and 2^64, inclusive.
	*/
	static FORCEINLINE int64 Rand2() noexcept { return (static_cast<int64>(Rand()) << 32) | static_cast<int64>(Rand()); }
	/** 
		@return Random float between 0 and 1, inclusive.
	*/
	static FORCEINLINE float FRand() noexcept
	{
		// FP32 mantissa can only represent 24 bits before losing precision
		constexpr int32 RandMax = 0x00ffffff < Generator.max() ? 0x00ffffff : Generator.max();
		return (Rand() & RandMax) / static_cast<float>(RandMax);
	}
	/** 
		@return Random double between 0 and 1, inclusive.
	*/
	static FORCEINLINE double FRandDbl() noexcept
	{
		// FP64 mantissa can only represent 52 bits before losing precision
		constexpr uint64 GeneratorMax = (static_cast<uint64>(Generator.max()) << 32) | static_cast<uint64>(Generator.max());
		constexpr int64 RandMax = 0xfffffffffffff < GeneratorMax ? 0xfffffffffffff : GeneratorMax;
		return (Rand2() & RandMax) / static_cast<double>(RandMax);
	}

	/** 
		Helper function for rand implementations.
	
		@return Random number in [0..A)
	*/
	static FORCEINLINE int32 RandHelper(int32 A) noexcept
	{
		// Note that on some platforms RAND_MAX is a large number so we cannot do ((rand()/(RAND_MAX+1)) * A)
		// or else we may include the upper bound results, which should be excluded.
		return A > 0 ? FMath::Min(FMath::TruncToInt(FRand() * static_cast<float>(A)), A - 1) : 0;
	}
	static FORCEINLINE int64 RandHelper64(int64 A) noexcept
	{
		// Note that on some platforms RAND_MAX is a large number so we cannot do ((rand()/(RAND_MAX+1)) * A)
		// or else we may include the upper bound results, which should be excluded.
		return A > 0 ? FMath::Min<int64>(FMath::TruncToInt(FRandDbl() * static_cast<double>(A)), A - 1) : 0;
	}

	/** 
		Helper function for rand implementations.

		@return Random number >= Min and <= Max 
	*/
	static FORCEINLINE int32 RandRange(int32 Min, int32 Max) noexcept
	{
		const int32 Range = (Max - Min) + 1;
		return Min + RandHelper(Range);
	}
	static FORCEINLINE int64 RandRange(int64 Min, int64 Max) noexcept
	{
		const int64 Range = (Max - Min) + 1;
		return Min + RandHelper64(Range);
	}
	/** 
		Util to generate a random number in a range. 
		Overloaded to distinguish from int32 version, where passing a float is typically a mistake.
	*/
	static FORCEINLINE float RandRange(float InMin, float InMax) noexcept { return FRandRange(InMin, InMax); }
	static FORCEINLINE double RandRange(double InMin, double InMax) noexcept { return FRandRange(InMin, InMax); }
	/** 
		Util to generate a random number in a range.
	*/
	static FORCEINLINE float FRandRange(float InMin, float InMax) noexcept { return InMin + (InMax - InMin) * FRand(); }
	/** 
		Util to generate a random number in a range.
	*/
	static FORCEINLINE double FRandRange(double InMin, double InMax) noexcept { return InMin + (InMax - InMin) * FRandDbl(); }
	/** 
		Util to generate a random boolean.
	*/
	static FORCEINLINE bool RandBool() noexcept { return (RandRange(0, 1) == 1) ? true : false; }

	/** 
		@return Uniformly distributed random unit length vector = point on the unit sphere surface.
	*/
	static FVector2D VRand2D() noexcept;
	/** 
		@return Uniformly distributed random unit length vector = point on the unit sphere surface.
	*/
	static FVector3D VRand3D() noexcept;
	/**
		Returns a random unit vector, uniformly distributed, within the specified cone
		ConeHalfAngleRad is the half-angle of cone, in radians.  
		
		@return Normalized vector. 
	*/
	static FVector3D VRandCone(FVector3D const& Dir, float ConeHalfAngleRad) noexcept;
	/** 
		This is a version of VRandCone that handles "squished" cones, i.e. with different angle limits in the Y and Z axes.
		Assumes world Y and Z, although this could be extended to handle arbitrary rotations.
	*/
	static FVector3D VRandCone(FVector3D const& Dir, float HorizontalConeHalfAngleRad, float VerticalConeHalfAngleRad) noexcept;
	/** 
		@return Random point, uniformly distributed, within the specified radius. 
	*/
	static FVector2D RandPointInCircle(float CircleRadius) noexcept;
	/** 
		@return Random point, uniformly distributed, within the specified radius. 
	*/
	static FVector3D RandPointInSphere(float SphereRadius) noexcept;
	/** 
		@return Random point within the passed in bounding box.
	*/
	static FVector2D RandPointInBox2D(const FBox2D& Box) noexcept;
	/** 
		@return Random point within the passed in bounding box.
	*/
	static FVector3D RandPointInBox3D(const FBox3D& Box) noexcept;



private:

	static std::mt19937 Generator;
};