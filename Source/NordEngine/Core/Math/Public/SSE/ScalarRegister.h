#pragma once

#include "SSE/MathSSE.h"




/*
	The ScalarRegister class wraps the concept of a 'float-in-vector', allowing common scalar operations like bone
	weight calculations to be done in vector registers.  This will avoid some LHS hazards that arise when mixing float
	and vector math on some platforms.  However, doing the math for four elements is slower if the vector operations are
	being emulated on a scalar FPU, so ScalarRegister is defined to float when ENABLE_VECTORIZED_FBONEATOM == 0.
*/
class ScalarRegister
{

public:

	/* default constructor */
	FORCEINLINE ScalarRegister() noexcept{ }

	/* Copy Constructor */
	FORCEINLINE ScalarRegister(const ScalarRegister& VectorValue) noexcept
	{
		Value = VectorValue.Value;
	}

	/* Constructor using float value */
	explicit FORCEINLINE ScalarRegister(const float& ScalarValue) noexcept
	{
		Value = VectorLoadFloat1(&ScalarValue);
	}

	/*
		Constructor
		@param VectorRegister - float4 vector register type
	*/
	explicit FORCEINLINE ScalarRegister(const VectorRegister& VectorValue) noexcept
	{
		Value = VectorValue;
	}



public:

	/*
		Gets the result of multiplying a scalar register to this.
	 
		@param RHS - The scalar register to multiply this by.
		@return The result of multiplication.
	*/
	FORCEINLINE ScalarRegister operator*(const ScalarRegister& RHS) const noexcept
	{
		return ScalarRegister(VectorMultiply(Value, RHS.Value));
	}

	/*
		Gets the result of adding a scalar register to this.
	 
		@param RHS - The scalar register to add.
		@return The result of addition.
	*/
	FORCEINLINE ScalarRegister operator+(const ScalarRegister& RHS) const noexcept
	{
		return ScalarRegister(VectorAdd(Value, RHS.Value));
	}

	/*
		Adds to this scalar register.
	 
		@param RHS - The scalar register to add to this.
		@return Reference to this after addition.
	*/
	FORCEINLINE ScalarRegister& operator+=(const ScalarRegister& RHS) noexcept
	{
		Value = VectorAdd(Value, RHS.Value);
		return *this;
	}

	/*
		Subtracts another scalar register from this.
	 
		@param RHS - The other scalar register.
		@return reference to this after subtraction.
	*/
	FORCEINLINE ScalarRegister& operator-=(const ScalarRegister& RHS) noexcept
	{
		Value = VectorSubtract(Value, RHS.Value);
		return *this;
	}

	/*
		Gets the result of subtracting a scalar register to this.
	 
		@param RHS - The scalar register to subtract.
		@return The result of subtraction.
	*/
	FORCEINLINE ScalarRegister operator-(const ScalarRegister& RHS) const noexcept
	{
		return ScalarRegister(VectorSubtract(Value, RHS.Value));
	}


	/*
		Assignment operator.
	 
		@param RHS - A ScalarRegister.
	*/
	FORCEINLINE ScalarRegister& operator=(const ScalarRegister& RHS) noexcept
	{
		Value = RHS.Value;
		return *this;
	}

	/*
		Assignment operator.

		@param RHS - A VectorRegister.
	*/
	FORCEINLINE ScalarRegister& operator=(const VectorRegister& RHS) noexcept
	{
		Value = RHS;
		return *this;
	}

	/*
		ScalarRegister to VectorRegister conversion operator.
	*/
	FORCEINLINE operator VectorRegister() const noexcept
	{
		return Value;
	}




public:

	VectorRegister Value;
};


#define ScalarOne (ScalarRegister)ScalarRegister(VectorOne())
#define ScalarZero (ScalarRegister)ScalarRegister(VectorZero())





/* 
	@return The smaller of the two values.
*/
FORCEINLINE ScalarRegister ScalarMin(const ScalarRegister& A, const ScalarRegister& B)
{
	return ScalarRegister(VectorMin(A.Value, B.Value));
}

/*
	@return the larger of the two values.
*/
FORCEINLINE ScalarRegister ScalarMax(const ScalarRegister& A, const ScalarRegister& B)
{
	return ScalarRegister(VectorMax(A.Value, B.Value));
}

/*
	Computes the reciprocal of the scalar register (component-wise) and returns the result.
 
	@param A - 1st scalar.
	@return	ScalarRegister( 1.0f / A.x, 1.0f / A.y, 1.0f / A.z, 1.0f / A.w ).
*/
FORCEINLINE ScalarRegister ScalarReciprocal(const ScalarRegister& A)
{
	return ScalarRegister(VectorReciprocalAccurate(A.Value));
}


/*
	Returns zero if any element in A is greater than the corresponding element in the global AnimWeightThreshold.
 
	@param A - 1st source vector.
	@return Zero integer if (A.x > AnimWeightThreshold.x) || (A.y > AnimWeightThreshold.y) || (A.z > AnimWeightThreshold.z) || (A.w > AnimWeightThreshold.w), non-zero Otherwise.
*/
#define NonZeroAnimWeight(A) VectorAnyGreaterThan(A.Value, GlobalVectorConstants::AnimWeightThreshold)

/*
	Returns non-zero if any element in A is greater than the corresponding element in the global AnimWeightThreshold.
  
	@param A - 1st source vector.
	@return Non-zero integer if (A.x > AnimWeightThreshold.x) || (A.y > AnimWeightThreshold.y) || (A.z > AnimWeightThreshold.z) || (A.w > AnimWeightThreshold.w).
*/
#define NonOneAnimWeight(A) !VectorAnyGreaterThan(A.Value, VectorSubtract(VectorOne(), GlobalVectorConstants::AnimWeightThreshold))