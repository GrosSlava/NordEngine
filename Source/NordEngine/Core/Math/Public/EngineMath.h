// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "MathUtility.h"

#include <math.h>
#include <float.h>




struct ENGINE_API FMath
{
	/**
		Returns value based on comparand. The main purpose of this function is to avoid
		branching based on floating point comparison which can be avoided via compiler intrinsics.

		@note that we don't define what happens in the case of NaNs as there might be platform specific differences.

		@param Comparand - Comparand the results are based on
		@param ValueGEZero - Return value if Comparand >= 0
		@param ValueLTZero - Return value if Comparand < 0
		@return	ValueGEZero if Comparand >= 0, ValueLTZero otherwise.
	*/
	static FORCEINLINE float FloatSelect(float Comparand, float ValueGEZero, float ValueLTZero) noexcept { return Comparand >= 0.0f ? ValueGEZero : ValueLTZero; }
	/**
		Returns value based on comparand. The main purpose of this function is to avoid
		branching based on floating point comparison which can be avoided via compiler intrinsics.

		@note that we don't define what happens in the case of NaNs as there might be platform specific differences.

		@param Comparand - Comparand the results are based on
		@param ValueGEZero - Return value if Comparand >= 0
		@param ValueLTZero - Return value if Comparand < 0
		@return	ValueGEZero if Comparand >= 0, ValueLTZero otherwise.
	*/
	static FORCEINLINE double FloatSelect(double Comparand, double ValueGEZero, double ValueLTZero) noexcept { return Comparand >= 0.0 ? ValueGEZero : ValueLTZero; }

	/**
		Computes absolute value in a generic way.
	*/
	template<class T>
	static FORCEINLINE T Abs(const T A) noexcept
	{
		return (A >= (T)0) ? A : -A;
	}
	/**
		Float specialization.
	*/
	template<>
	static FORCEINLINE float Abs(const float A) noexcept
	{
		return fabsf(A);
	}
	/**
		@return 1, 0, or -1 depending on relation of T to 0.
	*/
	template<class T>
	static FORCEINLINE T Sign(const T A) noexcept
	{
		return (A > (T)0) ? (T)1 : ((A < (T)0) ? (T)-1 : (T)0);
	}

	/**
		@return higher value in a generic way.
	*/
	template<class T>
	static FORCEINLINE T Max(const T A, const T B) noexcept
	{
		return (A >= B) ? A : B;
	}
	/**
		@return lower value in a generic way.
	*/
	template<class T>
	static FORCEINLINE T Min(const T A, const T B) noexcept
	{
		return (A <= B) ? A : B;
	}
	/**
		@return highest of 3 values.
	*/
	template<class T>
	static FORCEINLINE T Max3(const T A, const T B, const T C) noexcept
	{
		return Max(Max(A, B), C);
	}
	/**
		@return lowest of 3 values.
	*/
	template<class T>
	static FORCEINLINE T Min3(const T A, const T B, const T C) noexcept
	{
		return Min(Min(A, B), C);
	}

	/**
		Clamps X to be between Min and Max, inclusive.
	*/
	template<class T>
	static FORCEINLINE T Clamp(const T X, const T Min, const T Max) noexcept
	{
		return X < Min ? Min : X < Max ? X : Max;
	}
	/**
		Wraps X to be between Min and Max, inclusive.
	*/
	template<class T>
	static T Wrap(const T X, const T Min, const T Max) noexcept
	{
		T Size = Max - Min;
		T EndVal = X;
		while( EndVal < Min )
		{
			EndVal += Size;
		}

		while( EndVal > Max )
		{
			EndVal -= Size;
		}
		return EndVal;
	}
	/**
		Snaps a value to the nearest grid multiple.
	*/
	template<class T>
	static FORCEINLINE T GridSnap(T Location, T Grid) noexcept
	{
		return (Grid == T {}) ? Location : (Floor((Location + (Grid / (T)2)) / Grid) * Grid);
	}

	/**
		Divides two integers and rounds up.
	*/
	template<class T>
	static FORCEINLINE T DivideAndRoundUp(T Dividend, T Divisor) noexcept
	{
		return (Dividend + Divisor - 1) / Divisor;
	}
	/**
		Divides two integers and rounds down.
	*/
	template<class T>
	static FORCEINLINE T DivideAndRoundDown(T Dividend, T Divisor) noexcept
	{
		return Dividend / Divisor;
	}
	/**
		Divides two integers and rounds to nearest.
	*/
	template<class T>
	static FORCEINLINE T DivideAndRoundNearest(T Dividend, T Divisor) noexcept
	{
		return (Dividend >= 0) ? (Dividend + Divisor / 2) / Divisor : (Dividend - Divisor / 2 + 1) / Divisor;
	}

	/**
		Checks if value is within a range, exclusive on MaxValue).
	*/
	template<class U>
	static FORCEINLINE bool IsWithin(const U& TestValue, const U& MinValue, const U& MaxValue) noexcept
	{
		return ((TestValue >= MinValue) && (TestValue < MaxValue));
	}
	/**
		Checks if value is within a range, inclusive on MaxValue).
	*/
	template<class U>
	static FORCEINLINE bool IsWithinInclusive(const U& TestValue, const U& MinValue, const U& MaxValue) noexcept
	{
		return ((TestValue >= MinValue) && (TestValue <= MaxValue));
	}

	/**
		Checks if two floating point numbers are nearly equal.

		@param A - First number to compare.
		@param B - Second number to compare.
		@param ErrorTolerance - Maximum allowed difference for considering them as 'nearly equal'.
		@return true if A and B are nearly equal.
	*/
	static FORCEINLINE bool IsNearlyEqual(float A, float B, float ErrorTolerance = SMALL_NUMBER) noexcept { return Abs<float>(A - B) <= ErrorTolerance; }
	/**
		Checks if a floating point number is nearly zero.

		@param Value - Number to compare.
		@param ErrorTolerance - Maximum allowed difference for considering Value as 'nearly zero'.
		@return true if Value is nearly zero.
	*/
	static FORCEINLINE bool IsNearlyZero(double Value, double ErrorTolerance = SMALL_NUMBER) noexcept { return Abs<double>(Value) <= ErrorTolerance; }

	/**
		Checks whether a number is a power of two.

		@param Value - Number to check.
		@return true if Value is a power of two.
	*/
	template<typename T>
	static FORCEINLINE bool IsPowerOfTwo(T Value) noexcept
	{
		return ((Value & (Value - 1)) == (T)0);
	}

	/**
		Multiples value by itself.
	*/
	template<class T>
	static FORCEINLINE T Square(const T A) noexcept
	{
		return A * A;
	}

	/**
		Converts a float to an integer with truncation towards zero.

		@param F - Floating point value to convert
		@return Truncated integer.
	*/
	static constexpr int32 TruncToInt(float F) noexcept { return static_cast<int32>(F); }
	static constexpr int64 TruncToInt(double F) noexcept { return static_cast<int64>(F); }
	/**
		Converts a float to an integer value with truncation towards zero.

		@param F - Floating point value to convert.
		@return Truncated integer value.
	*/
	static FORCEINLINE float TruncToFloat(float F) noexcept { return truncf(F); }
	/**
		Converts a double to an integer value with truncation towards zero.

		@param F - Floating point value to convert.
		@return Truncated integer value.
	*/
	static FORCEINLINE double TruncToDouble(double F) noexcept { return trunc(F); }

	/**
		Converts a float to a nearest less or equal integer.
	*/
	static FORCEINLINE float Floor(float F) noexcept { return FloorToFloat(F); }
	/**
		Converts a float to a nearest less or equal integer.

		@param F - Floating point value to convert.
		@return An integer less or equal to 'F'.
	*/
	static FORCEINLINE int FloorToInt(float F) noexcept { return TruncToInt(floorf(F)); }
	/**
		Converts a float to the nearest less or equal integer.

		@param F - Floating point value to convert.
		@return An integer less or equal to 'F'.
	*/
	static FORCEINLINE float FloorToFloat(float F) noexcept { return floorf(F); }
	/**
		Converts a double to a less or equal integer.

		@param F - Floating point value to convert.
		@return The nearest integer value to 'F'.
	*/
	static FORCEINLINE double FloorToDouble(double F) noexcept { return floor(F); }

	/**
		Converts a float to the nearest integer. Rounds up when the fraction is 0.5

		@param F - Floating point value to convert
		@return The nearest integer to 'F'.
	*/
	static FORCEINLINE int RoundToInt(float F) noexcept { return FloorToInt(F + 0.5f); }
	/**
		Converts a float to the nearest integer. Rounds up when the fraction is 0.5

		@param F - Floating point value to convert.
		@return The nearest integer to 'F'.
	*/
	static FORCEINLINE float RoundToFloat(float F) noexcept { return FloorToFloat(F + 0.5f); }
	/**
		Converts a double to the nearest integer. Rounds up when the fraction is 0.5

		@param F - Floating point value to convert.
		@return The nearest integer to 'F'.
	*/
	static FORCEINLINE double RoundToDouble(double F) noexcept { return FloorToDouble(F + 0.5); }

	/**
		Converts a float to the nearest greater or equal integer.

		@param F - Floating point value to convert.
		@return An integer greater or equal to 'F'.
	*/
	static FORCEINLINE int CeilToInt(float F) noexcept { return TruncToInt(ceilf(F)); }
	/**
		Converts a float to the nearest greater or equal integer.

		@param F - Floating point value to convert.
		@return An integer greater or equal to 'F'.
	*/
	static FORCEINLINE float CeilToFloat(float F) noexcept { return ceilf(F); }
	/**
		Converts a double to the nearest greater or equal integer.

		@param F - Floating point value to convert.
		@return An integer greater or equal to 'F'.
	*/
	static FORCEINLINE double CeilToDouble(double F) noexcept { return ceil(F); }

	/**
		Returns signed fractional part of a float.

		@param Value - Floating point value to convert.
		@return A float between >=0 and < 1 for nonnegative input. A float between >= -1 and < 0 for negative input.
	*/
	static FORCEINLINE float Fractional(float Value) noexcept { return Value - TruncToFloat(Value); }
	/**
		Returns the fractional part of a float.

		@param Value - Floating point value to convert.
		@return A float between >=0 and < 1.
	*/
	static FORCEINLINE float Frac(float Value) noexcept { return Value - FloorToFloat(Value); }

	/**
		Breaks the given value into an integral and a fractional part.

		@param InValue - Floating point value to convert.
		@param OutIntPart - Floating point value that receives the integral part of the number.
		@return The fractional part of the number.
	*/
	static FORCEINLINE float Modf(const float InValue, float* OutIntPart) noexcept { return modff(InValue, OutIntPart); }
	/**
		Breaks the given value into an integral and a fractional part.

		@param InValue - Floating point value to convert.
		@param OutIntPart - Floating point value that receives the integral part of the number.
		@return The fractional part of the number.
	*/
	static FORCEINLINE double Modf(const double InValue, double* OutIntPart) noexcept { return modf(InValue, OutIntPart); }

	/**
		Computes the base 2 logarithm of the specified value.

		@param Value - The value to perform the log on.
		@return the base 2 log of the value.
	*/
	static FORCEINLINE float Log2(float Value) noexcept
	{
		// Cached value for fast conversions
		// 1.0 / Loge(2) = 1.4426950f
		static const float LogToLog2 = 1.f / Loge(2.f);
		// Do the platform specific log and convert using the cached value
		return Loge(Value) * LogToLog2;
	}

	/**
		Computes the sine and cosine of a scalar value.

		@param ScalarSin - Pointer to where the Sin result should be stored.
		@param ScalarCos - Pointer to where the Cos result should be stored.
		@param Value - Input angles.
	*/
	static void SinCos(float* ScalarSin, float* ScalarCos, float Value) noexcept
	{
		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		float quotient = (INV_PI * 0.5f) * Value;
		if( Value >= 0.0f )
		{
			quotient = (float)((int)(quotient + 0.5f));
		}
		else
		{
			quotient = (float)((int)(quotient - 0.5f));
		}
		float y = Value - (2.0f * PI) * quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		float sign;
		if( y > HALF_PI )
		{
			y = PI - y;
			sign = -1.0f;
		}
		else if( y < -HALF_PI )
		{
			y = -PI - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		float y2 = y * y;

		// 11-degree minimax approximation
		*ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		// 10-degree minimax approximation
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		*ScalarCos = sign * p;
	}

	/**
		Converts radians to degrees.

		@param RadVal - Value in radians.
		@return Value in degrees.
	*/
	template<class T>
	static FORCEINLINE auto RadiansToDegrees(T const& RadVal) noexcept -> decltype(RadVal * (180.f / PI))
	{
		return RadVal * (180.f / PI);
	}
	/**
		Converts degrees to radians.

		@param DegVal - Value in degrees.
		@return Value in radians.
	*/
	template<class T>
	static FORCEINLINE auto DegreesToRadians(T const& DegVal) noexcept -> decltype(DegVal * (PI / 180.f))
	{
		return DegVal * (PI / 180.f);
	}

	/**
		Find the smallest angle between two headings (in degrees).
	*/
	static FORCEINLINE float FindDeltaAngleDegrees(float A1, float A2) noexcept
	{
		// Find the difference
		float Delta = A2 - A1;

		// If change is larger than 180
		if( Delta > 180.0f )
		{
			// Flip to negative equivalent
			Delta = Delta - 360.0f;
		}
		else if( Delta < -180.0f )
		{
			// Otherwise, if change is smaller than -180
			// Flip to positive equivalent
			Delta = Delta + 360.0f;
		}

		// Return delta in [-180,180] range
		return Delta;
	}
	/**
		Find the smallest angle between two headings (in radians).
	*/
	static FORCEINLINE float FindDeltaAngleRadians(float A1, float A2) noexcept
	{
		// Find the difference
		float Delta = A2 - A1;

		// If change is larger than PI
		if( Delta > PI )
		{
			// Flip to negative equivalent
			Delta = Delta - (PI * 2.0f);
		}
		else if( Delta < -PI )
		{
			// Otherwise, if change is smaller than -PI
			// Flip to positive equivalent
			Delta = Delta + (PI * 2.0f);
		}

		// Return delta in [-PI,PI] range
		return Delta;
	}

	/**
		Given a heading which may be outside the +/- PI range, 'unwind' it back into that range.
	*/
	static FORCEINLINE float UnwindRadians(float A) noexcept
	{
		while( A > PI )
		{
			A -= ((float)PI * 2.0f);
		}

		while( A < -PI )
		{
			A += ((float)PI * 2.0f);
		}

		return A;
	}
	/**
		Utility to ensure angle is between +/- 180 degrees by unwinding.
	*/
	static FORCEINLINE float UnwindDegrees(float A) noexcept
	{
		while( A > 180.f )
		{
			A -= 360.f;
		}

		while( A < -180.f )
		{
			A += 360.f;
		}

		return A;
	}

	/**
		Performs a linear interpolation between two values, Alpha ranges from 0-1.
	*/
	template<class T, class U>
	static FORCEINLINE T Lerp(const T& A, const T& B, const U& Alpha) noexcept
	{
		return (T)(A + Alpha * (B - A));
	}
	/**
		Performs a linear interpolation between two values, Alpha ranges from 0-1. Handles full numeric range of T.
	*/
	template<class T>
	static FORCEINLINE T LerpStable(const T& A, const T& B, float Alpha) noexcept
	{
		return (T)((A * (1.0f - Alpha)) + (B * Alpha));
	}
	/**
		Performs a 2D linear interpolation between four values values, FracX, FracY ranges from 0-1.
	*/
	template<class T, class U>
	static FORCEINLINE T BiLerp(const T& P00, const T& P10, const T& P01, const T& P11, const U& FracX, const U& FracY) noexcept
	{
		return Lerp(Lerp(P00, P10, FracX), Lerp(P01, P11, FracX), FracY);
	}
	/**
		Performs a cubic interpolation.

		@param P - End points.
		@param T - Tangent directions at end points.
		@param Alpha - Distance along spline.
		@return Interpolated value.
	*/
	template<class T, class U>
	static FORCEINLINE T CubicInterp(const T& P0, const T& T0, const T& P1, const T& T1, const U& A) noexcept
	{
		const float A2 = A * A;
		const float A3 = A2 * A;

		return (T)(((2 * A3) - (3 * A2) + 1) * P0) + ((A3 - (2 * A2) + A) * T0) + ((A3 - A2) * T1) + (((-2 * A3) + (3 * A2)) * P1);
	}
	/**
		Performs a first derivative cubic interpolation.

		@param P - End points.
		@param T - Tangent directions at end points.
		@param Alpha - Distance along spline.

		@return Interpolated value.
	*/
	template<class T, class U>
	static FORCEINLINE T CubicInterpDerivative(const T& P0, const T& T0, const T& P1, const T& T1, const U& A) noexcept
	{
		T a = 6.f * P0 + 3.f * T0 + 3.f * T1 - 6.f * P1;
		T b = -6.f * P0 - 4.f * T0 - 2.f * T1 + 6.f * P1;
		T c = T0;

		const float A2 = A * A;

		return (a * A2) + (b * A) + c;
	}
	/**
		Performs a second derivative cubic interpolation.

		@param P - End points.
		@param T - Tangent directions at end points.
		@param Alpha - Distance along spline.

		@return Interpolated value.
	*/
	template<class T, class U>
	static FORCEINLINE T CubicInterpSecondDerivative(const T& P0, const T& T0, const T& P1, const T& T1, const U& A) noexcept
	{
		T a = 12.f * P0 + 6.f * T0 + 6.f * T1 - 12.f * P1;
		T b = -6.f * P0 - 4.f * T0 - 2.f * T1 + 6.f * P1;

		return (a * A) + b;
	}
	/**
		Interpolate between A and B, applying an ease in function. Exp controls the degree of the curve.
	*/
	template<class T>
	static FORCEINLINE T InterpEaseIn(const T& A, const T& B, float Alpha, float Exp) noexcept
	{
		const float ModifiedAlpha = Pow(Alpha, Exp);
		return Lerp<T>(A, B, ModifiedAlpha);
	}
	/**
		Interpolate between A and B, applying an ease out function. Exp controls the degree of the curve.
	*/
	template<class T>
	static FORCEINLINE T InterpEaseOut(const T& A, const T& B, float Alpha, float Exp) noexcept
	{
		const float ModifiedAlpha = 1.f - Pow(1.f - Alpha, Exp);
		return Lerp<T>(A, B, ModifiedAlpha);
	}
	/**
		Interpolate between A and B, applying an ease in/out function. Exp controls the degree of the curve.
	*/
	template<class T>
	static FORCEINLINE T InterpEaseInOut(const T& A, const T& B, float Alpha, float Exp) noexcept
	{
		return Lerp<T>(A, B, (Alpha < 0.5f) ? InterpEaseIn(0.f, 1.0f, Alpha * 2.0f, Exp) * 0.5f : InterpEaseOut(0.0f, 1.0f, Alpha * 2.0f - 1.0f, Exp) * 0.5f + 0.5f);
	}
	/**
		Interpolation between A and B, applying a step function.
	*/
	template<class T>
	static FORCEINLINE T InterpStep(const T& A, const T& B, float Alpha, int32 Steps)
	{
		if( Steps <= 1 || Alpha <= 0 )
		{
			return A;
		}
		else if( Alpha >= 1 )
		{
			return B;
		}

		const float StepsAsFloat = static_cast<float>(Steps);
		const float NumIntervals = StepsAsFloat - 1.0f;
		const float ModifiedAlpha = FloorToFloat(Alpha * StepsAsFloat) / NumIntervals;
		return Lerp<T>(A, B, ModifiedAlpha);
	}
	/**
		Interpolation between A and B, applying a sinusoidal in function.
	*/
	template<class T>
	static FORCEINLINE T InterpSinIn(const T& A, const T& B, float Alpha) noexcept
	{
		const float ModifiedAlpha = -1.0f * Cos(Alpha * HALF_PI) + 1.0f;
		return Lerp<T>(A, B, ModifiedAlpha);
	}
	/**
		Interpolation between A and B, applying a sinusoidal out function.
	*/
	template<class T>
	static FORCEINLINE T InterpSinOut(const T& A, const T& B, float Alpha) noexcept
	{
		const float ModifiedAlpha = Sin(Alpha * HALF_PI);
		return Lerp<T>(A, B, ModifiedAlpha);
	}
	/**
		Interpolation between A and B, applying a sinusoidal in/out function.
	*/
	template<class T>
	static FORCEINLINE T InterpSinInOut(const T& A, const T& B, float Alpha) noexcept
	{
		return Lerp<T>(A, B, (Alpha < 0.5f) ? InterpSinIn(0.0f, 1.0f, Alpha * 2.0f) * 0.5f : InterpSinOut(0.0f, 1.0f, Alpha * 2.0f - 1.0f) * 0.5f + 0.5f);
	}
	/**
		Interpolation between A and B, applying an exponential in function.
	*/
	template<class T>
	static FORCEINLINE T InterpExpoIn(const T& A, const T& B, float Alpha) noexcept
	{
		const float ModifiedAlpha = (Alpha == 0.0f) ? 0.0f : Pow(2.f, 10.0f * (Alpha - 1.0f));
		return Lerp<T>(A, B, ModifiedAlpha);
	}
	/**
		Interpolation between A and B, applying an exponential out function.
	*/
	template<class T>
	static FORCEINLINE T InterpExpoOut(const T& A, const T& B, float Alpha) noexcept
	{
		const float ModifiedAlpha = (Alpha == 1.0f) ? 1.0f : -Pow(2.0f, -10.0f * Alpha) + 1.0f;
		return Lerp<T>(A, B, ModifiedAlpha);
	}
	/**
		Interpolation between A and B, applying an exponential in/out function.
	*/
	template<class T>
	static FORCEINLINE T InterpExpoInOut(const T& A, const T& B, float Alpha) noexcept
	{
		return Lerp<T>(A, B, (Alpha < 0.5f) ? InterpExpoIn(0.0f, 1.0f, Alpha * 2.0f) * 0.5f : InterpExpoOut(0.0f, 1.0f, Alpha * 2.0f - 1.0f) * 0.5f + 0.5f);
	}
	/**
		Interpolation between A and B, applying a circular in function.
	*/
	template<class T>
	static FORCEINLINE T InterpCircularIn(const T& A, const T& B, float Alpha) noexcept
	{
		const float ModifiedAlpha = -1.0f * (Sqrt(1.0f - Alpha * Alpha) - 1.0f);
		return Lerp<T>(A, B, ModifiedAlpha);
	}
	/**
		Interpolation between A and B, applying a circular out function.
	*/
	template<class T>
	static FORCEINLINE T InterpCircularOut(const T& A, const T& B, float Alpha) noexcept
	{
		Alpha -= 1.0f;
		const float ModifiedAlpha = Sqrt(1.f - Alpha * Alpha);
		return Lerp<T>(A, B, ModifiedAlpha);
	}
	/**
		Interpolation between A and B, applying a circular in/out function.
	*/
	template<class T>
	static FORCEINLINE T InterpCircularInOut(const T& A, const T& B, float Alpha) noexcept
	{
		return Lerp<T>(A, B, (Alpha < 0.5f) ? InterpCircularIn(0.0f, 1.0f, Alpha * 2.0f) * 0.5f : InterpCircularOut(0.0f, 1.0f, Alpha * 2.0f - 1.0f) * 0.5f + 0.5f);
	}
	/**
		Returns a smooth Hermite interpolation between 0 and 1 for the value X (where X ranges between A and B)
		Clamped to 0 for X <= A and 1 for X >= B.

		@param A - Minimum value of X.
		@param B - Maximum value of X.
		@param X - Parameter.
		@return Smoothed value between 0 and 1.
	*/
	static float SmoothStep(float A, float B, float X) noexcept
	{
		if( X < A )
		{
			return 0.0f;
		}
		else if( X >= B )
		{
			return 1.0f;
		}
		const float InterpFraction = (X - A) / (B - A);
		return InterpFraction * InterpFraction * (3.0f - 2.0f * InterpFraction);
	}


	static FORCEINLINE float Exp(float Value) noexcept { return expf(Value); }
	static FORCEINLINE float Exp2(float Value) noexcept { return powf(2.f, Value); /*exp2f(Value);*/ }
	static FORCEINLINE float Loge(float Value) noexcept { return logf(Value); }
	static FORCEINLINE float LogX(float Base, float Value) noexcept { return Loge(Value) / Loge(Base); }
	static FORCEINLINE float Sin(float Value) noexcept { return sinf(Value); }
	static FORCEINLINE float Asin(float Value) noexcept { return asinf((Value < -1.f) ? -1.f : ((Value < 1.f) ? Value : 1.f)); }
	static FORCEINLINE float Sinh(float Value) noexcept { return sinhf(Value); }
	static FORCEINLINE float Cos(float Value) noexcept { return cosf(Value); }
	static FORCEINLINE float Acos(float Value) noexcept { return acosf((Value < -1.f) ? -1.f : ((Value < 1.f) ? Value : 1.f)); }
	static FORCEINLINE float Tan(float Value) noexcept { return tanf(Value); }
	static FORCEINLINE float Atan(float Value) noexcept { return atanf(Value); }
	static FORCEINLINE float Sqrt(float Value) noexcept { return sqrtf(Value); }
	static FORCEINLINE float Pow(float A, float B) noexcept { return powf(A, B); }

	static FORCEINLINE bool IsNaN(float A) noexcept { return _isnan(A) != 0; }
	static FORCEINLINE bool IsNaN(double A) noexcept { return _isnan(A) != 0; }
	static FORCEINLINE bool IsFinite(float A) noexcept { return _finite(A) != 0; }
	static FORCEINLINE bool IsFinite(double A) noexcept { return _finite(A) != 0; }

	/** 
		Computes a fully accurate inverse square root.
	*/
	static FORCEINLINE float InvSqrt(float F) noexcept { return 1.0f / sqrtf(F); }

	/** 
		Computes a faster but less accurate inverse square root.
	*/
	static FORCEINLINE float InvSqrtEst(float F) noexcept { return InvSqrt(F); }

	/**
		atan2f occasionally returns NaN with perfectly valid input (possibly due to a compiler or library bug).
		We are replacing it with a minimax approximation with a max relative error of 7.15255737e-007 compared to the C library function.
		On PC this has been measured to be 2x faster than the std C version.
	*/
	static float Atan2(float Y, float X) noexcept
	{
		//return atan2f(Y,X);

		const float absX = FMath::Abs(X);
		const float absY = FMath::Abs(Y);
		const bool yAbsBigger = (absY > absX);
		float t0 = yAbsBigger ? absY : absX; // Max(absY, absX)
		float t1 = yAbsBigger ? absX : absY; // Min(absX, absY)

		if( t0 == 0.0f ) return 0.0f;

		float t3 = t1 / t0;
		float t4 = t3 * t3;

		static const float c[7] = {+7.2128853633444123e-03f, -3.5059680836411644e-02f, +8.1675882859940430e-02f, -1.3374657325451267e-01f, +1.9856563505717162e-01f, -3.3324998579202170e-01f, +1.0f};

		t0 = c[0];
		t0 = t0 * t4 + c[1];
		t0 = t0 * t4 + c[2];
		t0 = t0 * t4 + c[3];
		t0 = t0 * t4 + c[4];
		t0 = t0 * t4 + c[5];
		t0 = t0 * t4 + c[6];
		t3 = t0 * t3;

		t3 = yAbsBigger ? (0.5f * PI) - t3 : t3;
		t3 = (X < 0.0f) ? PI - t3 : t3;
		t3 = (Y < 0.0f) ? -t3 : t3;

		return t3;
	}

	static float Fmod(float X, float Y) noexcept
	{
		const float AbsY = fabsf(Y);
		if( AbsY <= 1.e-8f )
		{
			return 0.0f;
		}
		const float Div = (X / Y);
		// All floats where abs(f) >= 2^23 (8388608) are whole numbers so do not need truncation, and avoid overflow in TruncToFloat as they get even larger.
		const float Quotient = fabsf(Div) < FLOAT_NON_FRACTIONAL ? TruncToFloat(Div) : Div;
		float IntPortion = Y * Quotient;

		// Rounding and imprecision could cause IntPortion to exceed X and cause the result to be outside the expected range.
		// For example Fmod(55.8, 9.3) would result in a very small negative value!
		if( fabsf(IntPortion) > fabsf(X) )
		{
			IntPortion = X;
		}

		const float Result = X - IntPortion;
		// Clamp to [-AbsY, AbsY] because of possible failures for very large numbers (>1e10) due to precision loss.
		// We could instead fall back to stock fmodf() for large values, however this would diverge from the SIMD VectorMod() which has no similar fallback with reasonable performance.
		return FMath::Clamp(Result, -AbsY, AbsY);
	}


	// Note:  We use FASTASIN_HALF_PI instead of HALF_PI inside of FastASin(), since it was the value that accompanied the minimax coefficients below.
	// It is important to use exactly the same value in all places inside this function to ensure that FastASin(0.0f) == 0.0f.
	// For comparison:
	//		HALF_PI				== 1.57079632679f == 0x3fC90FDB
	//		FASTASIN_HALF_PI	== 1.5707963050f  == 0x3fC90FDA
#define FASTASIN_HALF_PI (1.5707963050f)
	/**
		Computes the ASin of a scalar value.

		@param Value - input angle.
		@return ASin of Value.
	*/
	static FORCEINLINE float FastAsin(float Value) noexcept
	{
		// Clamp input to [-1,1].
		bool nonnegative = (Value >= 0.0f);
		float x = FMath::Abs(Value);
		float omx = 1.0f - x;
		if( omx < 0.0f )
		{
			omx = 0.0f;
		}
		const float root = FMath::Sqrt(omx);
		// 7-degree minimax approximation
		float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + FASTASIN_HALF_PI;
		result *= root; // acos(|x|)
		// acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
		return (nonnegative ? FASTASIN_HALF_PI - result : result - FASTASIN_HALF_PI);
	}
#undef FASTASIN_HALF_PI

/**
	Counts the number of trailing zeros in the bit representation of the value,	counting from least-significant bit to most.
 
	@param Value - the value to determine the number of leading zeros for.
	@return the number of zeros before the first "on" bit.
*/
#if defined(_MSC_VER)
#pragma intrinsic(_BitScanForward)
	FORCEINLINE uint32 CountTrailingZeros(uint32 Value)
	{
		if( Value == 0 )
		{
			return 32;
		}
		unsigned long BitIndex;			   // 0-based, where the LSB is 0 and MSB is 31
		_BitScanForward(&BitIndex, Value); // Scans from LSB to MSB
		return BitIndex;
	}
#else  // !defined(_MSC_VER)
	FORCEINLINE uint32 CountTrailingZeros(uint32 Value)
	{
		if( Value == 0 )
		{
			return 32;
		}
		return __builtin_ffs(Value) - 1;
	}
#endif // _MSC_VER
};
