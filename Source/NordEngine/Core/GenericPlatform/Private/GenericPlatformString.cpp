// Copyright Nord Engine. All Rights Reserved.
#include "GenericPlatformString.h"
#include "Char.h"





// clang-format off
static constexpr uint8 LowerAscii[128] = 
{
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x40, 'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
	'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z',  0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F
};
// clang-format on


template<typename CharType1, typename CharType2>
FORCEINLINE bool BothAscii(CharType1 C1, CharType2 C2)
{
	return ((static_cast<uint32>(C1) | static_cast<uint32>(C2)) & 0xffffff80) == 0;
}

template<typename CharType1, typename CharType2>
int32 StricmpImpl(const CharType1* String1, const CharType2* String2)
{
	while( true )
	{
		CharType1 C1 = *String1++;
		CharType2 C2 = *String2++;

		// Quickly move on if characters are identical but
		// return equals if we found two null terminators
		if( C1 == C2 )
		{
			if( C1 )
			{
				continue;
			}

			return 0;
		}
		else if( BothAscii(C1, C2) )
		{
			if( int32 Diff = LowerAscii[TChar<CharType1>::ToUnsigned(C1)] - LowerAscii[TChar<CharType2>::ToUnsigned(C2)] )
			{
				return Diff;
			}
		}
		else
		{
			return TChar<CharType1>::ToUnsigned(C1) - TChar<CharType2>::ToUnsigned(C2);
		}
	}
}

template<typename CharType1, typename CharType2>
int32 StrnicmpImpl(const CharType1* String1, const CharType2* String2, SIZE_T Count)
{
	for( ; Count > 0; --Count )
	{
		CharType1 C1 = *String1++;
		CharType2 C2 = *String2++;

		// Quickly move on if characters are identical but
		// return equals if we found two null terminators
		if( C1 == C2 )
		{
			if( C1 )
			{
				continue;
			}

			return 0;
		}
		else if( BothAscii(C1, C2) )
		{
			if( int32 Diff = LowerAscii[TChar<CharType1>::ToUnsigned(C1)] - LowerAscii[TChar<CharType2>::ToUnsigned(C2)] )
			{
				return Diff;
			}
		}
		else
		{
			return TChar<CharType1>::ToUnsigned(C1) - TChar<CharType2>::ToUnsigned(C2);
		}
	}

	return 0;
}

int32 FGenericPlatformString::Stricmp(const ANSICHAR* Str1, const ANSICHAR* Str2)
{
	return StricmpImpl(Str1, Str2);
}
int32 FGenericPlatformString::Stricmp(const WIDECHAR* Str1, const WIDECHAR* Str2)
{
	return StricmpImpl(Str1, Str2);
}
int32 FGenericPlatformString::Stricmp(const ANSICHAR* Str1, const WIDECHAR* Str2)
{
	return StricmpImpl(Str1, Str2);
}
int32 FGenericPlatformString::Stricmp(const WIDECHAR* Str1, const ANSICHAR* Str2)
{
	return StricmpImpl(Str1, Str2);
}
int32 FGenericPlatformString::Strnicmp(const ANSICHAR* Str1, const ANSICHAR* Str2, SIZE_T Count)
{
	return StrnicmpImpl(Str1, Str2, Count);
}
int32 FGenericPlatformString::Strnicmp(const WIDECHAR* Str1, const WIDECHAR* Str2, SIZE_T Count)
{
	return StrnicmpImpl(Str1, Str2, Count);
}
int32 FGenericPlatformString::Strnicmp(const ANSICHAR* Str1, const WIDECHAR* Str2, SIZE_T Count)
{
	return StrnicmpImpl(Str1, Str2, Count);
}
int32 FGenericPlatformString::Strnicmp(const WIDECHAR* Str1, const ANSICHAR* Str2, SIZE_T Count)
{
	return StrnicmpImpl(Str1, Str2, Count);
}




template<typename CharType1, typename CharType2>
int32 StrncmpImpl(const CharType1* String1, const CharType2* String2, SIZE_T Count)
{
	for( ; Count > 0; --Count )
	{
		CharType1 C1 = *String1++;
		CharType2 C2 = *String2++;

		if( C1 != C2 )
		{
			return TChar<CharType1>::ToUnsigned(C1) - TChar<CharType2>::ToUnsigned(C2);
		}
		if( C1 == 0 )
		{
			return 0;
		}
	}

	return 0;
}

int32 FGenericPlatformString::Strncmp(const ANSICHAR* Str1, const ANSICHAR* Str2, SIZE_T Count)
{
	return StrncmpImpl(Str1, Str2, Count);
}
int32 FGenericPlatformString::Strncmp(const WIDECHAR* Str1, const WIDECHAR* Str2, SIZE_T Count)
{
	return StrncmpImpl(Str1, Str2, Count);
}
int32 FGenericPlatformString::Strncmp(const ANSICHAR* Str1, const WIDECHAR* Str2, SIZE_T Count)
{
	return StrncmpImpl(Str1, Str2, Count);
}
int32 FGenericPlatformString::Strncmp(const WIDECHAR* Str1, const ANSICHAR* Str2, SIZE_T Count)
{
	return StrncmpImpl(Str1, Str2, Count);
}