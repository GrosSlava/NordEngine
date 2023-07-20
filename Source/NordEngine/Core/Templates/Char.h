// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"

#include "TypeTraits/IsSigned.h"

#include <ctype.h>
#include <wctype.h>




/**
	Templated literal struct to allow selection of wide or ansi string literals
	based on the character type provided, and not on compiler switches.
*/
template<typename T>
struct TLiteral
{
	static const ANSICHAR Select(const ANSICHAR ansi, const WIDECHAR) { return ansi; }
	static const ANSICHAR* Select(const ANSICHAR* ansi, const WIDECHAR*) { return ansi; }
};

template<>
struct TLiteral<WIDECHAR>
{
	static const WIDECHAR Select(const ANSICHAR, const WIDECHAR wide) { return wide; }
	static const WIDECHAR* Select(const ANSICHAR*, const WIDECHAR* wide) { return wide; }
};

#define LITERAL(CharType, StringLiteral) TLiteral<CharType>::Select(StringLiteral, TEXT(StringLiteral))




template<typename CharType, const unsigned int Size>
struct TCharBase
{
	static constexpr CharType LineFeed = 0xa;
	static constexpr CharType VerticalTab = 0xb;
	static constexpr CharType FormFeed = 0xc;
	static constexpr CharType CarriageReturn = 0xd;
	static constexpr CharType NextLine = 0x85;
	static constexpr CharType LineSeparator = 0x2028;
	static constexpr CharType ParagraphSeparator = 0x2029;

	static bool IsLinebreak(CharType Char) { return ((uint32(Char) - LineFeed) <= uint32(CarriageReturn - LineFeed)) | (Char == NextLine) | (Char == LineSeparator) | (Char == ParagraphSeparator); }
};

template<typename CharType>
struct TCharBase<CharType, 1>
{
	static constexpr CharType LineFeed = 0xa;
	static constexpr CharType VerticalTab = 0xb;
	static constexpr CharType FormFeed = 0xc;
	static constexpr CharType CarriageReturn = 0xd;

	static bool IsLinebreak(CharType Char) { return ((uint32(Char) - LineFeed) <= uint32(CarriageReturn - LineFeed)); }
};

/**
	Set of utility functions operating on a single character. 
	The functions are specialized for ANSICHAR and TCHAR character types. 
	You can use the	typedefs FChar and FCharAnsi for convenience.
*/
template<typename CharType>
struct TChar : TCharBase<CharType, sizeof(CharType)>
{
	/**
		Only converts ASCII characters, same as CRT to[w]upper() with standard C locale
	*/
	static FORCEINLINE CharType ToUpper(CharType Char) noexcept { return (CharType)(ToUnsigned(Char) - ((uint32(Char) - 'a' < 26u) << 5)); }
	/**
		Only converts ASCII characters, same as CRT to[w]upper() with standard C locale
	*/
	static FORCEINLINE CharType ToLower(CharType Char) noexcept { return (CharType)(ToUnsigned(Char) + ((uint32(Char) - 'A' < 26u) << 5)); }

	static FORCEINLINE int32 ConvertCharDigitToInt(CharType Char) noexcept { return static_cast<int32>(Char) - static_cast<int32>('0'); }
	static FORCEINLINE bool IsOctDigit(CharType Char) noexcept { return uint32(Char) - '0' < 8u; }
	static FORCEINLINE bool IsIdentifier(CharType Char) noexcept { return IsAlnum(Char) || IsUnderscore(Char); }
	static FORCEINLINE bool IsUnderscore(CharType Char) noexcept { return Char == LITERAL(CharType, '_'); }

	static bool IsUpper(CharType Char) noexcept;
	static bool IsLower(CharType Char) noexcept;
	static bool IsAlpha(CharType Char) noexcept;
	static bool IsGraph(CharType Char) noexcept;
	static bool IsPrint(CharType Char) noexcept;
	static bool IsPunct(CharType Char) noexcept;
	static bool IsAlnum(CharType Char) noexcept;
	static bool IsDigit(CharType Char) noexcept;
	static bool IsHexDigit(CharType Char) noexcept;
	static bool IsWhitespace(CharType Char) noexcept;

	/**
		Avoid sign extension problems with signed characters smaller than int
	
		E.g. 'Ö' - 'A' is negative since the char 'Ö' (0xD6) is negative and gets
		sign-extended to the 32-bit int 0xFFFFFFD6 before subtraction happens.
	
		Mainly needed for subtraction and addition.
	*/
	static constexpr FORCEINLINE uint32 ToUnsigned(CharType Char) noexcept { return (typename TUnsignedIntType<sizeof(CharType)>::Type)Char; }
};

using FChar = TChar<TCHAR>;
using FCharWide = TChar<WIDECHAR>;
using FCharAnsi = TChar<ANSICHAR>;




// WIDECHAR specialized functions
template<>
inline bool TChar<WIDECHAR>::IsUpper(WIDECHAR Char) noexcept
{
	return iswupper(Char) != 0;
}
template<>
inline bool TChar<WIDECHAR>::IsLower(WIDECHAR Char) noexcept
{
	return iswlower(Char) != 0;
}
template<>
inline bool TChar<WIDECHAR>::IsAlpha(WIDECHAR Char) noexcept
{
	return iswalpha(Char) != 0;
}
template<>
inline bool TChar<WIDECHAR>::IsGraph(WIDECHAR Char) noexcept
{
	return iswgraph(Char) != 0;
}
template<>
inline bool TChar<WIDECHAR>::IsPrint(WIDECHAR Char) noexcept
{
	return iswprint(Char) != 0;
}
template<>
inline bool TChar<WIDECHAR>::IsPunct(WIDECHAR Char) noexcept
{
	return iswpunct(Char) != 0;
}
template<>
inline bool TChar<WIDECHAR>::IsAlnum(WIDECHAR Char) noexcept
{
	return iswalnum(Char) != 0;
}
template<>
inline bool TChar<WIDECHAR>::IsDigit(WIDECHAR Char) noexcept
{
	return iswdigit(Char) != 0;
}
template<>
inline bool TChar<WIDECHAR>::IsHexDigit(WIDECHAR Char) noexcept
{
	return iswxdigit(Char) != 0;
}
template<>
inline bool TChar<WIDECHAR>::IsWhitespace(WIDECHAR Char) noexcept
{
	return iswspace(Char) != 0;
}


// ANSICHAR specialized functions
template<>
inline bool TChar<ANSICHAR>::IsUpper(ANSICHAR Char) noexcept
{
	return isupper(static_cast<int>(Char)) != 0;
}
template<>
inline bool TChar<ANSICHAR>::IsLower(ANSICHAR Char) noexcept
{
	return islower(static_cast<int>(Char)) != 0;
}
template<>
inline bool TChar<ANSICHAR>::IsAlpha(ANSICHAR Char) noexcept
{
	return isalpha(static_cast<int>(Char)) != 0;
}
template<>
inline bool TChar<ANSICHAR>::IsGraph(ANSICHAR Char) noexcept
{
	return isgraph(static_cast<int>(Char)) != 0;
}
template<>
inline bool TChar<ANSICHAR>::IsPrint(ANSICHAR Char) noexcept
{
	return isprint(static_cast<int>(Char)) != 0;
}
template<>
inline bool TChar<ANSICHAR>::IsPunct(ANSICHAR Char) noexcept
{
	return ispunct(static_cast<int>(Char)) != 0;
}
template<>
inline bool TChar<ANSICHAR>::IsAlnum(ANSICHAR Char) noexcept
{
	return isalnum(static_cast<int>(Char)) != 0;
}
template<>
inline bool TChar<ANSICHAR>::IsDigit(ANSICHAR Char) noexcept
{
	return isdigit(static_cast<int>(Char)) != 0;
}
template<>
inline bool TChar<ANSICHAR>::IsHexDigit(ANSICHAR Char) noexcept
{
	return isxdigit(static_cast<int>(Char)) != 0;
}
template<>
inline bool TChar<ANSICHAR>::IsWhitespace(ANSICHAR Char) noexcept
{
	return isspace(static_cast<int>(Char)) != 0;
}
