// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "EngineMath.h"
#include "Axis.h"




struct FVector3D;
struct FQuat;


/**
	Implements a container for rotation information.
	All rotation values are stored in degrees.
*/
struct ENGINE_API FRotator
{
public:

	/**
		Default constructor (no initialization).
	*/
	FORCEINLINE FRotator() = default;
	/**
		@param InF - Value to set all components to.
	*/
	FORCEINLINE FRotator(float InF) noexcept : Pitch(InF), Yaw(InF), Roll(InF) { }
	/**
		@param InPitch - Pitch in degrees.
		@param InYaw - Yaw in degrees.
		@param InRoll - Roll in degrees.
	*/
	FORCEINLINE FRotator(float InPitch, float InYaw, float InRoll) noexcept : Pitch(InPitch), Yaw(InYaw), Roll(InRoll) { }
	/**
		@param Quat - Quaternion used to specify rotation.
	*/
	FRotator(const FQuat& Quat);


public:

	/**
		Get the result of adding a rotator to this.
	 
		@param R - The other rotator.
		@return The result of adding a rotator to this.
	*/
	FORCEINLINE FRotator operator+(const FRotator& R) const noexcept { return FRotator(Pitch + R.Pitch, Yaw + R.Yaw, Roll + R.Roll); }
	/**
		Adds another rotator to this.
	 
		@param R - The other rotator.
		@return Copy of rotator after addition.
	*/
	FORCEINLINE FRotator operator+=(const FRotator& R) noexcept
	{
		Pitch += R.Pitch;
		Yaw += R.Yaw;
		Roll += R.Roll;
		return *this;
	}
	/**
		Get the result of subtracting a rotator from this.
	 
		@param R - The other rotator.
		@return The result of subtracting a rotator from this.
	*/
	FORCEINLINE FRotator operator-(const FRotator& R) const noexcept { return FRotator(Pitch - R.Pitch, Yaw - R.Yaw, Roll - R.Roll); }
	/**
		Subtracts another rotator from this.
	 
		@param R - The other rotator.
		@return Copy of rotator after subtraction.
	*/
	FORCEINLINE FRotator operator-=(const FRotator& R) noexcept
	{
		Pitch -= R.Pitch;
		Yaw -= R.Yaw;
		Roll -= R.Roll;
		return *this;
	}
	/**
		Get the result of scaling this rotator.
	 
		@param Scale - The scaling factor.
		@return The result of scaling.
	*/
	FORCEINLINE FRotator operator*(float Scale) const noexcept { return FRotator(Pitch * Scale, Yaw * Scale, Roll * Scale); }
	/**
		Multiply this rotator by a scaling factor.
	 
		@param Scale - The scaling factor.
		@return Copy of the rotator after scaling.
	*/
	FORCEINLINE FRotator operator*=(float Scale) noexcept
	{
		Pitch = Pitch * Scale;
		Yaw = Yaw * Scale;
		Roll = Roll * Scale;
		return *this;
	}

public:

	/**
		Checks whether two rotators are identical. This checks each component for exact equality.
		@see Equals()

		@param R - The other rotator.
		@return true if two rotators are identical, otherwise false.
	*/
	FORCEINLINE bool operator==(const FRotator& R) const noexcept { return Pitch == R.Pitch && Yaw == R.Yaw && Roll == R.Roll; }
	/**
		Checks whether two rotators are different.
	 
		@param V - The other rotator.
		@return true if two rotators are different, otherwise false.
	*/
	FORCEINLINE bool operator!=(const FRotator& V) const noexcept { return Pitch != V.Pitch || Yaw != V.Yaw || Roll != V.Roll; }

public:

	/**
		Checks whether rotator is nearly zero within specified tolerance, when treated as an orientation.
		This means that FRotator(0, 0, 360) is "zero", because it is the same final orientation as the zero rotator.
	 
		@param Tolerance - Error Tolerance.
		@return true if rotator is nearly zero, within specified tolerance, otherwise false.
	*/
	FORCEINLINE bool IsNearlyZero(float Tolerance = KINDA_SMALL_NUMBER) const noexcept
	{
		return FMath::Abs(NormalizeAxis(Pitch)) <= Tolerance && FMath::Abs(NormalizeAxis(Yaw)) <= Tolerance && FMath::Abs(NormalizeAxis(Roll)) <= Tolerance;
	}
	/**
		Checks whether this has exactly zero rotation, when treated as an orientation.
		This means that FRotator(0, 0, 360) is "zero", because it is the same final orientation as the zero rotator.
	 
		@return true if this has exactly zero rotation, otherwise false.
	*/
	FORCEINLINE bool IsZero() const noexcept { return (ClampAxis(Pitch) == 0.f) && (ClampAxis(Yaw) == 0.f) && (ClampAxis(Roll) == 0.f); }

	/**
		Checks whether two rotators are equal within specified tolerance, when treated as an orientation.
		This means that FRotator(0, 0, 360).Equals(FRotator(0,0,0)) is true, because they represent the same final orientation.
	 
		@param R - The other rotator.
		@param Tolerance - Error Tolerance.
		@return true if two rotators are equal, within specified tolerance, otherwise false.
	*/
	FORCEINLINE bool Equals(const FRotator& R, float Tolerance = KINDA_SMALL_NUMBER) const noexcept
	{
		return (FMath::Abs(NormalizeAxis(Pitch - R.Pitch)) <= Tolerance) && (FMath::Abs(NormalizeAxis(Yaw - R.Yaw)) <= Tolerance) && (FMath::Abs(NormalizeAxis(Roll - R.Roll)) <= Tolerance);
	}

	/**
		Get the rotation, snapped to specified degree segments.
	 
		@param RotGrid - A Rotator specifying how to snap each component.
		@return Snapped version of rotation.
	*/
	FORCEINLINE FRotator GridSnap(const FRotator& RotGrid) const noexcept { return FRotator(FMath::GridSnap(Pitch, RotGrid.Pitch), FMath::GridSnap(Yaw, RotGrid.Yaw), FMath::GridSnap(Roll, RotGrid.Roll)); }

	/**
		Gets the rotation values so they fall within the range [0,360]
	 
		@return Clamped version of rotator.
	*/
	FORCEINLINE FRotator Clamp() const noexcept { return FRotator(ClampAxis(Pitch), ClampAxis(Yaw), ClampAxis(Roll)); }

	/**
		Create a copy of this rotator and normalize, removes all winding and creates the "shortest route" rotation.
	 
		@return Normalized copy of this rotator.
	*/
	FORCEINLINE FRotator GetNormalized() const noexcept
	{
		FRotator Rot = *this;
		Rot.Normalize();
		return Rot;
	}
	/**
		Create a copy of this rotator and denormalize, clamping each axis to 0 - 360.
	 
		@return Denormalized copy of this rotator.
	*/
	FORCEINLINE FRotator GetDenormalized() const noexcept
	{
		FRotator Rot = *this;
		Rot.Pitch = ClampAxis(Rot.Pitch);
		Rot.Yaw = ClampAxis(Rot.Yaw);
		Rot.Roll = ClampAxis(Rot.Roll);
		return Rot;
	}

	/**
		Get a specific component of the vector, given a specific axis by enum.
	*/
	FORCEINLINE float GetComponentForAxis(EAxis Axis) const noexcept
	{
		switch( Axis )
		{
		case EAxis::X: return Roll;
		case EAxis::Y: return Pitch;
		case EAxis::Z: return Yaw;
		default: return 0.0f;
		}
	}
	/** 
		Set a specified componet of the vector, given a specific axis by enum.
	*/
	FORCEINLINE void SetComponentForAxis(EAxis Axis, float Component) noexcept
	{
		switch( Axis )
		{
		case EAxis::X: Roll = Component; break;
		case EAxis::Y: Pitch = Component; break;
		case EAxis::Z: Yaw = Component; break;
		}
	}

	/**
		Adds to each component of the rotator.
	 
		@param DeltaPitch - Change in pitch. (+/-)
		@param DeltaYaw - Change in yaw. (+/-)
		@param DeltaRoll - Change in roll. (+/-)
		@return Copy of rotator after addition.
	*/
	FORCEINLINE FRotator Add(float DeltaPitch, float DeltaYaw, float DeltaRoll) noexcept
	{
		Yaw += DeltaYaw;
		Pitch += DeltaPitch;
		Roll += DeltaRoll;
		return *this;
	}

	/**
		In-place normalize, removes all winding and creates the "shortest route" rotation.
	*/
	FORCEINLINE void Normalize() noexcept
	{
		Pitch = NormalizeAxis(Pitch);
		Yaw = NormalizeAxis(Yaw);
		Roll = NormalizeAxis(Roll);
	}

	/**
		Decompose this Rotator into a Winding part (multiples of 360) and a Remainder part.
		Remainder will always be in [-180, 180] range.
	 
		@param Winding[Out] - the Winding part of this Rotator.
		@param Remainder[Out] - the Remainder.
	*/
	FORCEINLINE void GetWindingAndRemainder(FRotator& Winding, FRotator& Remainder) const noexcept
	{
		//// YAW
		Remainder.Yaw = NormalizeAxis(Yaw);
		Winding.Yaw = Yaw - Remainder.Yaw;

		//// PITCH
		Remainder.Pitch = NormalizeAxis(Pitch);
		Winding.Pitch = Pitch - Remainder.Pitch;

		//// ROLL
		Remainder.Roll = NormalizeAxis(Roll);
		Winding.Roll = Roll - Remainder.Roll;
	}

	/**
		Return the manhattan distance in degrees between this Rotator and the passed in one.

		@param Rotator - the Rotator we are comparing with.
		@return Distance(Manhattan) between the two rotators.
	*/
	FORCEINLINE float GetManhattanDistance(const FRotator& Rotator) const noexcept { return FMath::Abs<float>(Yaw - Rotator.Yaw) + FMath::Abs<float>(Pitch - Rotator.Pitch) + FMath::Abs<float>(Roll - Rotator.Roll); }

	/**
		Return a Rotator that has the same rotation but has different degree values for Yaw, Pitch, and Roll.
		This rotator should be within -180,180 range.

		@return A Rotator with the same rotation but different degrees.
	*/
	FORCEINLINE FRotator GetEquivalentRotator() const noexcept { return FRotator(180.0f - Pitch, Yaw + 180.0f, Roll + 180.0f); }

	/**
		Modify if necessary the passed in rotator to be the closest rotator to it based upon it's equivalent.
		This Rotator should be within (-180, 180], usually just constructed from a Matrix or a Quaternion.
	
		@param MakeClosest[In/Out] - The Rotator we want to make closest to us. Should be between (-180, 180]. 
		This Rotator may change if we need to use different degree values to make it closer.
	*/
	FORCEINLINE void SetClosestToMe(FRotator& MakeClosest) const noexcept
	{
		FRotator OtherChoice = MakeClosest.GetEquivalentRotator();
		float FirstDiff = GetManhattanDistance(MakeClosest);
		float SecondDiff = GetManhattanDistance(OtherChoice);

		if( SecondDiff < FirstDiff ) MakeClosest = OtherChoice;
	}

	/**
		Utility to check if there are any non-finite values (NaN or Inf) in this Rotator.
	 
		@return true if there are any non-finite values in this Rotator, otherwise false.
	*/
	FORCEINLINE bool ContainsNaN() const noexcept { return (!FMath::IsFinite(Pitch) || !FMath::IsFinite(Yaw) || !FMath::IsFinite(Roll)); }

public:

	/**
		@return The inverse of the rotator.
	*/
	FRotator GetInverse() const noexcept;

	/**
		Convert a rotation into a unit vector facing in its direction.
	 
		@return Rotation as a unit direction vector.
	*/
	FVector3D Vector() const noexcept;

	/**
		Get Rotation as a quaternion.
	 
		@return Rotation as a quaternion.
	*/
	FQuat Quaternion() const noexcept;

	/**
		Convert a Rotator into floating-point Euler angles (in degrees). Rotator now stored in degrees.
	 
		@return Rotation as a Euler angle vector.
	*/
	FVector3D Euler() const noexcept;

	/**
		Rotate a vector rotated by this rotator.
	 
		@param V - The vector to rotate.
		@return The rotated vector.
	*/
	FVector3D RotateVector(const FVector3D& V) const noexcept;

	/**
		Returns the vector rotated by the inverse of this rotator.
	 
		@param V - The vector to rotate.
		@return The rotated vector.
	*/
	FVector3D UnrotateVector(const FVector3D& V) const noexcept;

public:

	/**
		Clamps an angle to the range of [0, 360).
	 
		@param Angle - The angle to clamp.
		@return The clamped angle.
	*/
	static FORCEINLINE float ClampAxis(float Angle) noexcept
	{
		// returns Angle in the range (-360,360)
		Angle = FMath::Fmod(Angle, 360.0f);

		if( Angle < 0.0f )
		{
			// shift to [0,360) range
			Angle += 360.0f;
		}

		return Angle;
	}

	/**
		Clamps an angle to the range of (-180, 180].
	 
		@param Angle - The Angle to clamp.
		@return The clamped angle.
	*/
	static FORCEINLINE float NormalizeAxis(float Angle) noexcept
	{
		// returns Angle in the range [0,360)
		Angle = ClampAxis(Angle);

		if( Angle > 180.0f )
		{
			// shift to (-180,180]
			Angle -= 360.0f;
		}

		return Angle;
	}

	/**
		Compresses a floating point angle into a byte.
	 
		@param Angle - The angle to compress.
		@return The angle as a byte.
	*/
	static FORCEINLINE uint8 CompressAxisToByte(float Angle) noexcept
	{
		// map [0->360) to [0->256) and mask off any winding
		return FMath::RoundToInt(Angle * 256.f / 360.f) & 0xFF;
	}
	/**
		Decompress a word into a floating point angle.
	 
		@param Angle - The word angle.
		@return The decompressed angle.
	*/
	static FORCEINLINE float DecompressAxisFromByte(uint8 Angle) noexcept
	{
		// map [0->256) to [0->360)
		return (Angle * 360.f / 256.f);
	}
	/**
		Compress a floating point angle into a word.
	 
		@param Angle - The angle to compress.
		@return The decompressed angle.
	*/
	static FORCEINLINE uint16 CompressAxisToShort(float Angle) noexcept
	{
		// map [0->360) to [0->65536) and mask off any winding
		return FMath::RoundToInt(Angle * 65536.f / 360.f) & 0xFFFF;
	}
	/**
		Decompress a short into a floating point angle.
	 
		@param Angle - The word angle.
		@return The decompressed angle.
	*/
	static FORCEINLINE float DecompressAxisFromShort(uint16 Angle) noexcept
	{
		// map [0->65536) to [0->360)
		return (Angle * 360.f / 65536.f);
	}

	/**
		Convert a vector of floating-point Euler angles (in degrees) into a Rotator. 
		Rotator now stored in degrees.
	 
		@param Euler - Euler angle vector.
		@return A rotator from a Euler angle.
	*/
	static FRotator MakeFromEuler(const FVector3D& Euler) noexcept;



public:

	/** 
		A rotator of zero degrees on each axis.
	*/
	static const FRotator ZeroRotator;

public:

	/** 
		Rotation around the right axis (around Y axis), Looking up and down (0=Straight Ahead, +Up, -Down) 
	*/
	float Pitch = 0.0f;
	/** 
		Rotation around the up axis (around Z axis), Running in circles 0=East, +North, -South.
	*/
	float Yaw = 0.0f;
	/** 
		Rotation around the forward axis (around X axis), Tilting your head, 0=Straight, +Clockwise, -CCW. 
	*/
	float Roll = 0.0f;
};


/**
	Scale a rotator and return.
 
	@param Scale - scale to apply to R.
	@param R - rotator to be scaled.
	@return Scaled rotator.
*/
FORCEINLINE FRotator operator*(float Scale, const FRotator& R)
{
	return R.operator*(Scale);
}