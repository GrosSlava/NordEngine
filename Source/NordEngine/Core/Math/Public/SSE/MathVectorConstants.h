// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "MathUtility.h"




namespace GlobalVectorConstants
{
static const VectorRegister Pi = MakeVectorRegister(PI, PI, PI, PI);
static const VectorRegister TwoPi = MakeVectorRegister(2.0f * PI, 2.0f * PI, 2.0f * PI, 2.0f * PI);
static const VectorRegister PiByTwo = MakeVectorRegister(0.5f * PI, 0.5f * PI, 0.5f * PI, 0.5f * PI);
static const VectorRegister PiByFour = MakeVectorRegister(0.25f * PI, 0.25f * PI, 0.25f * PI, 0.25f * PI);
static const VectorRegister OneOverPi = MakeVectorRegister(1.0f / PI, 1.0f / PI, 1.0f / PI, 1.0f / PI);
static const VectorRegister OneOverTwoPi = MakeVectorRegister(1.0f / (2.0f * PI), 1.0f / (2.0f * PI), 1.0f / (2.0f * PI), 1.0f / (2.0f * PI));

static const VectorRegister SmallLengthThreshold = MakeVectorRegister(1.e-8f, 1.e-8f, 1.e-8f, 1.e-8f);
static const VectorRegister KindaSmallNumber = MakeVectorRegister(KINDA_SMALL_NUMBER, KINDA_SMALL_NUMBER, KINDA_SMALL_NUMBER, KINDA_SMALL_NUMBER);
static const VectorRegister SmallNumber = MakeVectorRegister(SMALL_NUMBER, SMALL_NUMBER, SMALL_NUMBER, SMALL_NUMBER);
static const VectorRegister ThreshQuatNormalized = MakeVectorRegister(THRESH_QUAT_NORMALIZED, THRESH_QUAT_NORMALIZED, THRESH_QUAT_NORMALIZED, THRESH_QUAT_NORMALIZED);
static const VectorRegister BigNumber = MakeVectorRegister(BIG_NUMBER, BIG_NUMBER, BIG_NUMBER, BIG_NUMBER);


static const VectorRegister FloatZero = MakeVectorRegister(0.0f, 0.0f, 0.0f, 0.0f);
static const VectorRegister FloatOne = MakeVectorRegister(1.0f, 1.0f, 1.0f, 1.0f);
static const VectorRegister FloatMinusOne = MakeVectorRegister(-1.0f, -1.0f, -1.0f, -1.0f);
static const VectorRegister FloatTwo = MakeVectorRegister(2.0f, 2.0f, 2.0f, 2.0f);
static const VectorRegister Float0001 = MakeVectorRegister(0.0f, 0.0f, 0.0f, 1.0f);
static const VectorRegister Float1000 = MakeVectorRegister(1.0f, 0.0f, 0.0f, 0.0f);
static const VectorRegister FloatOneHundredth = MakeVectorRegister(0.01f, 0.01f, 0.01f, 0.01f);
static const VectorRegister Float111_Minus1 = MakeVectorRegister(1.0f, 1.0f, 1.0f, -1.0f);
static const VectorRegister FloatMinus1_111 = MakeVectorRegister(-1.0f, 1.0f, 1.0f, 1.0f);
static const VectorRegister FloatOneHalf = MakeVectorRegister(0.5f, 0.5f, 0.5f, 0.5f);
static const VectorRegister FloatMinusOneHalf = MakeVectorRegister(-0.5f, -0.5f, -0.5f, -0.5f);
static const VectorRegister FloatInfinity = MakeVectorRegister((uint32)0x7F800000, (uint32)0x7F800000, (uint32)0x7F800000, (uint32)0x7F800000);
static const VectorRegister Float255 = MakeVectorRegister(255.0f, 255.0f, 255.0f, 255.0f);
static const VectorRegister Float127 = MakeVectorRegister(127.0f, 127.0f, 127.0f, 127.0f);
static const VectorRegister FloatNeg127 = MakeVectorRegister(-127.0f, -127.0f, -127.0f, -127.0f);
static const VectorRegister Float360 = MakeVectorRegister(360.0f, 360.0f, 360.0f, 360.0f);
static const VectorRegister Float180 = MakeVectorRegister(180.0f, 180.0f, 180.0f, 180.0f);

static const VectorRegisterInt IntZero = MakeVectorRegisterInt(0, 0, 0, 0);
static const VectorRegisterInt IntOne = MakeVectorRegisterInt(1, 1, 1, 1);
static const VectorRegisterInt IntMinusOne = MakeVectorRegisterInt(-1, -1, -1, -1);
static const VectorRegisterInt IntTwo = MakeVectorRegisterInt(2, 2, 2, 2);
static const VectorRegisterInt Int0001 = MakeVectorRegisterInt(0, 0, 0, 1);
static const VectorRegisterInt Int1000 = MakeVectorRegisterInt(1, 0, 0, 0);



/** This is to speed up Quaternion Inverse. Static variable to keep sign of inverse **/
static const VectorRegister QINV_SIGN_MASK = MakeVectorRegister(-1.0f, -1.0f, -1.0f, 1.0f);

static const VectorRegister QMULTI_SIGN_MASK0 = MakeVectorRegister(1.0f, -1.0f, 1.0f, -1.0f);
static const VectorRegister QMULTI_SIGN_MASK1 = MakeVectorRegister(1.0f, 1.0f, -1.0f, -1.0f);
static const VectorRegister QMULTI_SIGN_MASK2 = MakeVectorRegister(-1.0f, 1.0f, 1.0f, -1.0f);

static const VectorRegister DEG_TO_RAD = MakeVectorRegister(PI / (180.0f), PI / (180.0f), PI / (180.0f), PI / (180.0f));
static const VectorRegister DEG_TO_RAD_HALF = MakeVectorRegister((PI / 180.0f) * 0.5f, (PI / 180.0f) * 0.5f, (PI / 180.0f) * 0.5f, (PI / 180.0f) * 0.5f);
static const VectorRegister RAD_TO_DEG = MakeVectorRegister((180.f) / PI, (180.0f) / PI, (180.0f) / PI, (180.0f) / PI);

/** Bitmask to AND out the XYZ components in a vector */
static const VectorRegister XYZMask = MakeVectorRegister((uint32)0xffffffff, (uint32)0xffffffff, (uint32)0xffffffff, (uint32)0x00000000);

/** Bitmask to AND out the sign bit of each components in a vector */
#define SIGN_BIT ((1 << 31))
static const VectorRegister SignBit = MakeVectorRegister((uint32)SIGN_BIT, (uint32)SIGN_BIT, (uint32)SIGN_BIT, (uint32)SIGN_BIT);
static const VectorRegister SignMask = MakeVectorRegister((uint32)(~SIGN_BIT), (uint32)(~SIGN_BIT), (uint32)(~SIGN_BIT), (uint32)(~SIGN_BIT));
static const VectorRegisterInt IntSignBit = MakeVectorRegisterInt(SIGN_BIT, SIGN_BIT, SIGN_BIT, SIGN_BIT);
static const VectorRegisterInt IntSignMask = MakeVectorRegisterInt((~SIGN_BIT), (~SIGN_BIT), (~SIGN_BIT), (~SIGN_BIT));
#undef SIGN_BIT
static const VectorRegister AllMask = MakeVectorRegister(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
static const VectorRegisterInt IntAllMask = MakeVectorRegisterInt(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);


// All float numbers greater than or equal to this have no fractional value.
static const VectorRegister FloatNonFractional = MakeVectorRegister(FLOAT_NON_FRACTIONAL, FLOAT_NON_FRACTIONAL, FLOAT_NON_FRACTIONAL, FLOAT_NON_FRACTIONAL);

static const uint32 AlmostTwoBits = 0x3fffffff;
static const VectorRegister FloatAlmostTwo = MakeVectorRegister(*(float*)&AlmostTwoBits, *(float*)&AlmostTwoBits, *(float*)&AlmostTwoBits, *(float*)&AlmostTwoBits);

static const VectorRegister AnimWeightThreshold = MakeVectorRegister(ZERO_ANIMWEIGHT_THRESH, ZERO_ANIMWEIGHT_THRESH, ZERO_ANIMWEIGHT_THRESH, ZERO_ANIMWEIGHT_THRESH);
static const VectorRegister RotationSignificantThreshold = MakeVectorRegister(1.0f - DELTA * DELTA, 1.0f - DELTA * DELTA, 1.0f - DELTA * DELTA, 1.0f - DELTA * DELTA);
} // namespace GlobalVectorConstants
