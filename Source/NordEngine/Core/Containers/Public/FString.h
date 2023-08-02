// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "GenericPlatformString.h"
#include "StringBuffer.h"
#include "Char.h"

#include "EngineMath.h"
#include "NumericLimits.h"
#include "EngineMemory.h"

#include "MoveSemantic.h"

#include "Array.h"




/** 
	Determines case sensitivity options for string comparisons. 
*/
enum class ESearchCase : uint8
{
	// Case sensitive. Upper/lower casing must match for strings to be considered equal.
	CaseSensitive,

	// Ignore case. Upper/lower casing does not matter when making a comparison.
	IgnoreCase
};

/** 
	Determines search direction for string operations. 
*/
enum class ESearchDir : uint8
{
	// Search from the start, moving forward through the string.
	FromStart,

	// Search from the end, moving backward through the string.
	FromEnd
};


/**
	Engine version of std::string.
*/
struct ENGINE_API FString
{
public:

	FORCEINLINE explicit FString(uint32 UnitializedSize = 0) noexcept { SetNumUninitialized(UnitializedSize); }
	FORCEINLINE FString(const TCHAR NewString[]) noexcept
	{
		StringBuffer.Resize(FPlatformString::Strlen(NewString));
		FPlatformString::Strcpy(StringBuffer.GetBuffer(), StringBuffer.GetBufferSize(), NewString);
	}
	FORCEINLINE FString(const TCHAR NewString[], uint32 Count) noexcept
	{
		StringBuffer.Resize(Count);
		FPlatformString::Strncpy(StringBuffer.GetBuffer(), NewString, Count + 1);
	}
	FORCEINLINE FString(const FString& Other) noexcept
	{
		StringBuffer.Resize(Other.Length());
		FPlatformString::Strcpy(StringBuffer.GetBuffer(), StringBuffer.GetBufferSize(), Other.GetStr());
	}
	FORCEINLINE FString(FString&& Other) noexcept : StringBuffer(MoveTemp(Other.StringBuffer)) { }
	FORCEINLINE FString(FStringBuffer&& Buffer) noexcept : StringBuffer(MoveTemp(Buffer)) { }


public:

	FORCEINLINE FString& operator=(const FString& Other) noexcept
	{
		if( this == &Other ) return *this;

		StringBuffer.Resize(Other.Length());
		FPlatformString::Strcpy(StringBuffer.GetBuffer(), StringBuffer.GetBufferSize(), Other.GetStr());

		return *this;
	}
	FORCEINLINE FString& operator=(FString&& Other) noexcept
	{
		StringBuffer = MoveTemp(Other.StringBuffer);

		return *this;
	}
	FORCEINLINE FString& operator=(TCHAR C) noexcept
	{
		StringBuffer.Resize(1);
		StringBuffer[0] = C;

		return *this;
	}
	FORCEINLINE FString& operator=(const TCHAR* S) noexcept
	{
		if( StringBuffer.GetBuffer() == S ) return *this;

		StringBuffer.Resize(FPlatformString::Strlen(S));
		FPlatformString::Strcpy(StringBuffer.GetBuffer(), StringBuffer.GetBufferSize(), S);

		return *this;
	}

public:

	FORCEINLINE FString& operator+=(const FString& Other) noexcept
	{
		const uint32 OldLength = Length();
		StringBuffer.Resize(Length() + Other.Length());
		FPlatformString::Strcpy(StringBuffer.GetAtByPointer(OldLength), Other.Length(), Other.GetStr());

		return *this;
	}
	FORCEINLINE FString& operator+=(TCHAR C) noexcept
	{
		StringBuffer.Resize(Length() + 1);
		StringBuffer[Length() - 1] = C;

		return *this;
	}
	FORCEINLINE FString& operator+=(const TCHAR* S) noexcept
	{
		const uint32 SLength = FPlatformString::Strlen(S);
		const uint32 OldLength = Length();
		StringBuffer.Resize(Length() + SLength);
		FPlatformString::Strcpy(StringBuffer.GetAtByPointer(OldLength), SLength, S);

		return *this;
	}
	FORCEINLINE friend FString operator+(const FString& lhs, const FString& rhs) noexcept
	{
		FStringBuffer Buff(lhs.Length() + rhs.Length());

		FPlatformString::Strcpy(Buff.GetBuffer(), lhs.Length(), lhs.GetStr());
		FPlatformString::Strcpy(Buff.GetAtByPointer(lhs.Length()), rhs.Length(), rhs.GetStr());

		return FString(MoveTemp(Buff));
	}
	FORCEINLINE friend FString operator+(FString&& lhs, FString&& rhs) noexcept
	{
		uint32 lhsOldLength = lhs.Length();
		lhs.StringBuffer.Resize(lhs.Length() + rhs.Length());

		FPlatformString::Strcpy(lhs.StringBuffer.GetAtByPointer(lhsOldLength), rhs.Length(), rhs.GetStr());

		return FString(MoveTemp(lhs));
	}
	FORCEINLINE friend FString operator+(FString&& lhs, const FString& rhs) noexcept
	{
		uint32 lhsOldLength = lhs.Length();
		lhs.StringBuffer.Resize(lhs.Length() + rhs.Length());

		FPlatformString::Strcpy(lhs.StringBuffer.GetBuffer(), lhsOldLength, lhs.GetStr());
		FPlatformString::Strcpy(lhs.StringBuffer.GetAtByPointer(lhsOldLength), rhs.Length(), rhs.GetStr());

		return FString(MoveTemp(lhs));
	}

public:

	FORCEINLINE TCHAR& operator[](uint32 Index) { return StringBuffer[Index]; }
	FORCEINLINE TCHAR operator[](uint32 Index) const { return StringBuffer[Index]; }

	FORCEINLINE bool operator==(const FString& Other) const noexcept
	{
		if( this == &Other ) return true;
		if( Length() != Other.Length() ) return false;

		const TCHAR* A = StringBuffer.begin();
		const TCHAR* B = Other.StringBuffer.begin();
		while( A != StringBuffer.end() )
		{
			if( *A != *B ) return false;
			++A;
			++B;
		}

		return true;
	}
	FORCEINLINE bool operator!=(const FString& Other) const noexcept { return !(*this == Other); }

	FORCEINLINE const TCHAR* begin() const noexcept { return StringBuffer.begin(); }
	FORCEINLINE TCHAR* begin() noexcept { return StringBuffer.begin(); }
	FORCEINLINE const TCHAR* end() const noexcept { return StringBuffer.end(); }
	FORCEINLINE TCHAR* end() noexcept { return StringBuffer.end(); }

public:

	FORCEINLINE bool IsEmpty() const noexcept { return Length() == 0; }
	FORCEINLINE uint32 Length() const noexcept { return StringBuffer.GetLength(); }
	FORCEINLINE const TCHAR* const GetStr() const noexcept { return StringBuffer.GetBuffer(); }
	FORCEINLINE TCHAR* const GetBuffer() const noexcept { return StringBuffer.GetBuffer(); }

public:

	/**
		Sets string to empty.
	*/
	FORCEINLINE void Clear() noexcept { StringBuffer.Reset(); }
	/**
		Clear old string and create empty buffer with length = Length.
	*/
	FORCEINLINE void SetNumUninitialized(uint32 Length) noexcept
	{
		StringBuffer.Reset();
		StringBuffer.Resize(Length);
	}
	/**
		Clear old string and create buffer with length = Length and filled by C.
	*/
	FORCEINLINE void SetNumInitialized(uint32 Length, TCHAR C) noexcept
	{
		StringBuffer.Reset();
		StringBuffer.Resize(Length);
		FPlatformString::Strset(StringBuffer.GetBuffer(), Length, C);
	}
	/**
		Reserve memory for string.
	*/
	FORCEINLINE void Reserve(uint32 NewBufferSize) noexcept { StringBuffer.Reserve(NewBufferSize); }
	/**
		If NewLength > StringLength then new memory will be unitializes.
	*/
	FORCEINLINE void SetLength(uint32 NewLength) noexcept { StringBuffer.Resize(NewLength); }
	/**
		Append string with given count of chars.
	*/
	FORCEINLINE void AddInitialized(uint32 Count, TCHAR C) noexcept
	{
		StringBuffer.Resize(Length() + Count);
		FPlatformString::Strset(StringBuffer.GetBuffer() + Length() - Count, Count, C);
	}

	/**
		Optimized concatanation of variadic count of strings.
	*/
	template<typename... Args>
	static FORCEINLINE FString Concat(const FString& First, const FString& Second, const Args&... args) noexcept
	{
		const uint32 ArgsSize = (args.Length() + ...);
		FStringBuffer Buff(First.Length() + Second.Length() + ArgsSize);

		FPlatformString::Strcpy(Buff.GetBuffer(), First.Length(), First.GetStr());
		FPlatformString::Strcpy(Buff.GetAtByPointer(First.Length()), Second.Length(), Second.GetStr());

		uint32 ConcatPosition = First.Length() + Second.Length();
		// clang-format off
		(
			[&]
			{
				FPlatformString::Strcat(Buff.GetAtByPointer(ConcatPosition), args.Length(), args.GetStr());
				ConcatPosition += args.Length();
			}(),
		...);
		// clang-format on

		return FString(MoveTemp(Buff));
	}

public:

	/**
		Breaks up a delimited string into elements of a string array.
	 
		@param InArray -The array to fill with the string pieces.
		@param pchDelim - The string to delimit on.
		@param InCullEmpty - If 1, empty strings are not added to the array.
		@return The number of elements in InArray.
	*/
	int32 ParseIntoArray(TArray<FString>& OutArray, const TCHAR* pchDelim, bool InCullEmpty = true) const noexcept;

	/**
		@return a copy of this string, with the characters in reverse order.
	*/
	FORCEINLINE FString Reverse() const& noexcept
	{
		FString New(*this);
		New.ReverseString();
		return New;
	}
	/**
		@return this string, with the characters in reverse order.
	*/
	FORCEINLINE FString Reverse() && noexcept
	{
		ReverseString();
		return MoveTemp(*this);
	}
	/**
		Reverses the order of characters in this string.
	*/
	void ReverseString() noexcept;

	/**
		Replace all occurrences of a substring in this string.
	 
		@param From - Substring to replace.
		@param To - Substring to replace From with.
		@param SearchCase - Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase ).
		@return a copy of this string with the replacement made.
	*/
	FORCEINLINE FString Replace(const TCHAR* From, const TCHAR* To, ESearchCase SearchCase = ESearchCase::IgnoreCase) const& noexcept
	{
		FString New(*this);
		New.ReplaceInline(From, To, SearchCase);
		return New;
	}
	/**
		Replace all occurrences of a substring in this string.
	 
		@param From - Substring to replace.
		@param To - Substring to replace From with.
		@param SearchCase - Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase ).
		@return a copy of this string with the replacement made.
	*/
	FORCEINLINE FString Replace(const TCHAR* From, const TCHAR* To, ESearchCase SearchCase = ESearchCase::IgnoreCase) && noexcept
	{
		ReplaceInline(From, To, SearchCase);
		return MoveTemp(*this);
	}
	/**
		Replace all occurrences of SearchText with ReplacementText in this string.
	 
		@param SearchText - The text that should be removed from this string.
		@param ReplacementText - The text to insert in its place.
		@param SearchCase - Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase ).
		@return	the number of occurrences of SearchText that were replaced.
	*/
	int32 ReplaceInline(const TCHAR* SearchText, const TCHAR* ReplacementText, ESearchCase SearchCase = ESearchCase::IgnoreCase) noexcept;
	/**
		Replaces certain characters with the "escaped" version of that character (i.e. replaces "\n" with "\\n").
		The characters supported are: { \n, \r, \t, \', \", \\ }.
	 
		@param Chars -by default, replaces all supported characters; this parameter allows you to limit the replacement to a subset.
		@return	a string with all control characters replaced by the escaped version.
	*/
	FORCEINLINE FString ReplaceCharWithEscapedChar(const TArray<TCHAR>* Chars = nullptr) const& noexcept
	{
		FString Result(*this);
		return MoveTemp(Result).ReplaceCharWithEscapedChar(Chars);
	}
	/**
		Replaces certain characters with the "escaped" version of that character (i.e. replaces "\n" with "\\n").
		The characters supported are: { \n, \r, \t, \', \", \\ }.
	 
		@param Chars - by default, replaces all supported characters; this parameter allows you to limit the replacement to a subset.
		@return	a string with all control characters replaced by the escaped version.
	*/
	FString ReplaceCharWithEscapedChar(const TArray<TCHAR>* Chars = nullptr) && noexcept;
	/**
		Removes the escape backslash for all supported characters, replacing the escape and character with the non-escaped version.  
		i.e. replaces "\\n" with "\n".
		Counterpart to ReplaceCharWithEscapedChar().
	 
		@return copy of this string with replacement made.
	*/
	FORCEINLINE FString ReplaceEscapedCharWithChar(const TArray<TCHAR>* Chars = nullptr) const& noexcept
	{
		FString Result(*this);
		return MoveTemp(Result).ReplaceEscapedCharWithChar(Chars);
	}
	/**
		Removes the escape backslash for all supported characters, replacing the escape and character with the non-escaped version.  
		i.e. replaces "\\n" with "\n".
		Counterpart to ReplaceCharWithEscapedChar().

		@return copy of this string with replacement made.
	*/
	FString ReplaceEscapedCharWithChar(const TArray<TCHAR>* Chars = nullptr) && noexcept;

	/**
		Replaces all instances of '\t' with TabWidth number of spaces.

		@param SpacesPerTab - Number of spaces that a tab represents.
		@return copy of this string with replacement made.
	*/
	FORCEINLINE FString ConvertTabsToSpaces(const int32 SpacesPerTab) const& noexcept
	{
		FString FinalString(*this);
		FinalString.ConvertTabsToSpacesInline(SpacesPerTab);
		return FinalString;
	}
	/**
		Replaces all instances of '\t' with TabWidth number of spaces.

		@param SpacesPerTab - Number of spaces that a tab represents.
		@return copy of this string with replacement made.
	*/
	FORCEINLINE FString ConvertTabsToSpaces(const int32 SpacesPerTab) && noexcept
	{
		ConvertTabsToSpacesInline(SpacesPerTab);
		return MoveTemp(*this);
	}
	/**
		Replaces all instances of '\t' with TabWidth number of spaces.

		@param SpacesPerTab - Number of spaces that a tab represents.
	*/
	void ConvertTabsToSpacesInline(const uint32 SpacesPerTab) noexcept;

	/**
		Remove leading whitespaces.

		@return copy of this string with removement made.
	*/
	FORCEINLINE FString TrimLeft() const& noexcept
	{
		FString FinalString(*this);
		FinalString.TrimLeftInline();
		return FinalString;
	}
	/**
		Remove leading whitespaces.

		@return copy of this string with removement made.
	*/
	FORCEINLINE FString TrimLeft() && noexcept
	{
		TrimLeftInline();
		return MoveTemp(*this);
	}
	/**
		Remove leading whitespaces.
	*/
	FORCEINLINE void TrimLeftInline() noexcept
	{
		TCHAR* p = begin();

		while( p != end() && FChar::IsWhitespace(*p) )
		{
			++p;
		}

		RemoveAt(0, static_cast<uint32>(p - begin()));
	}
	/**
		Remove trailing whitespaces.

		@return copy of this string with removement made.
	*/
	FORCEINLINE FString TrimRight() const& noexcept
	{
		FString FinalString(*this);
		FinalString.TrimRightInline();
		return FinalString;
	}
	/**
		Remove trailing whitespaces.

		@return copy of this string with removement made.
	*/
	FORCEINLINE FString TrimRight() && noexcept
	{
		TrimRightInline();
		return MoveTemp(*this);
	}
	/**
		Remove trailing whitespaces.
	*/
	FORCEINLINE void TrimRightInline() noexcept
	{
		TCHAR* p = end() - 1;
		while( p >= begin() && FChar::IsWhitespace(*p) )
		{
			--p;
		}

		RemoveAt(static_cast<uint32>(p - begin()), static_cast<uint32>(end() - 1 - p));
	}
	/**
		Remove leading and trailing whitespaces.

		@return copy of this string with removement made.
	*/
	FORCEINLINE FString Trim() const& noexcept
	{
		FString FinalString(*this);
		FinalString.TrimInline();
		return FinalString;
	}
	/**
		Remove leading and trailing whitespaces.

		@return copy of this string with removement made.
	*/
	FORCEINLINE FString Trim() && noexcept
	{
		TrimInline();
		return MoveTemp(*this);
	}
	/**
		Remove leading and trailing whitespaces.
	*/
	FORCEINLINE void TrimInline() noexcept
	{
		TrimRightInline();
		TrimLeftInline();
	}

	/**
		Test whether this string starts with given string.
	
		@param SearchCase - Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase ).
		@return true if this string begins with specified text, false otherwise.
	*/
	FORCEINLINE bool StartsWith(const TCHAR* InPrefix, ESearchCase SearchCase = ESearchCase::IgnoreCase) const noexcept
	{
		if( SearchCase == ESearchCase::IgnoreCase )
		{
			return InPrefix && *InPrefix && !FPlatformString::Strnicmp(GetStr(), InPrefix, FPlatformString::Strlen(InPrefix));
		}
		else
		{
			return InPrefix && *InPrefix && !FPlatformString::Strncmp(GetStr(), InPrefix, FPlatformString::Strlen(InPrefix));
		}
	}
	/**
		Test whether this string starts with given string.
	 
		@param SearchCase - Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase ).
		@return true if this string begins with specified text, false otherwise.
	*/
	FORCEINLINE bool StartsWith(const FString& InPrefix, ESearchCase SearchCase = ESearchCase::IgnoreCase) const noexcept
	{
		if( SearchCase == ESearchCase::IgnoreCase )
		{
			return InPrefix.Length() > 0 && !FPlatformString::Strnicmp(GetStr(), InPrefix.GetStr(), InPrefix.Length());
		}
		else
		{
			return InPrefix.Length() > 0 && !FPlatformString::Strncmp(GetStr(), InPrefix.GetStr(), InPrefix.Length());
		}
	}

	/**
		Test whether this string ends with given string.
	 
		@param SearchCase - Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase ).
		@return true if this string ends with specified text, false otherwise.
	*/
	FORCEINLINE bool EndsWith(const TCHAR* InSuffix, ESearchCase SearchCase = ESearchCase::IgnoreCase) const noexcept
	{
		if( !InSuffix || *InSuffix == TEXT('\0') ) return false;

		const int32 SuffixLen = FPlatformString::Strlen(InSuffix);
		if( SuffixLen > (int32)Length() ) return false;

		if( SearchCase == ESearchCase::IgnoreCase )
		{
			return !FPlatformString::Stricmp(StringBuffer.GetAtByPointer(Length() - SuffixLen), InSuffix);
		}
		else
		{
			return !FPlatformString::Strcmp(StringBuffer.GetAtByPointer(Length() - SuffixLen), InSuffix);
		}
	}
	/**
		Test whether this string ends with given string.
	 
		@param SearchCase - Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase ).
		@return true if this string ends with specified text, false otherwise.
	*/
	FORCEINLINE bool EndsWith(const FString& InSuffix, ESearchCase SearchCase = ESearchCase::IgnoreCase) const noexcept
	{
		if( SearchCase == ESearchCase::IgnoreCase )
		{
			return InSuffix.Length() > 0 && Length() >= InSuffix.Length() && !FPlatformString::Stricmp(StringBuffer.GetAtByPointer(Length() - InSuffix.Length()), InSuffix.GetStr());
		}
		else
		{
			return InSuffix.Length() > 0 && Length() >= InSuffix.Length() && !FPlatformString::Strcmp(StringBuffer.GetAtByPointer(Length() - InSuffix.Length()), InSuffix.GetStr());
		}
	}

	/**
		Removes characters within the string.
	
		@param Index - The index of the first character to remove.
		@param Count - The number of characters to remove.
		@param AllowShrinking - Whether or not to reallocate to shrink the storage after removal.
	*/
	FORCEINLINE void RemoveAt(uint32 Index, uint32 Count = 1) noexcept
	{
		if( Index >= Length() ) return;
		if( Index + Count > Length() ) Count = Length() - Index;
		if( Count == 0 ) return;

		FMemory::MemCpy(StringBuffer.GetAtByPointer(Index), StringBuffer.GetAtByPointer(Index + Count), sizeof(TCHAR) * (Length() - Index - Count));
		StringBuffer.Resize(Length() - Count);
	}
	/**
		Removes the text from the start of the string if it exists.
	 
		@param InPrefix - The prefix to search for at the start of the string to remove.
		@return true if the prefix was removed, otherwise false.
	*/
	FORCEINLINE bool RemoveFromStart(const TCHAR* InPrefix, ESearchCase SearchCase = ESearchCase::IgnoreCase) noexcept
	{
		if( *InPrefix == 0 ) return false;

		if( StartsWith(InPrefix, SearchCase) )
		{
			RemoveAt(0, FPlatformString::Strlen(InPrefix));
			return true;
		}

		return false;
	}
	/**
		Removes the text from the start of the string if it exists.
	 
		@param InPrefix - The prefix to search for at the start of the string to remove.
		@return true if the prefix was removed, otherwise false.
	*/
	FORCEINLINE bool RemoveFromStart(const FString& InPrefix, ESearchCase SearchCase = ESearchCase::IgnoreCase) noexcept
	{
		if( InPrefix.IsEmpty() ) return false;

		if( StartsWith(InPrefix, SearchCase) )
		{
			RemoveAt(0, InPrefix.Length());
			return true;
		}

		return false;
	}
	/**
		Removes the text from the end of the string if it exists.
	 
		@param InSuffix - The suffix to search for at the end of the string to remove.
		@return true if the suffix was removed, otherwise false.
	*/
	FORCEINLINE bool RemoveFromEnd(const TCHAR* InSuffix, ESearchCase SearchCase = ESearchCase::IgnoreCase) noexcept
	{
		if( *InSuffix == 0 ) return false;

		if( EndsWith(InSuffix, SearchCase) )
		{
			int32 SuffixLen = FPlatformString::Strlen(InSuffix);
			RemoveAt(Length() - SuffixLen, SuffixLen);
			return true;
		}

		return false;
	}
	/**
		Removes the text from the end of the string if it exists.
	 
		@param InSuffix - The suffix to search for at the end of the string to remove.
		@return true if the suffix was removed, otherwise false.
	*/
	FORCEINLINE bool RemoveFromEnd(const FString& InSuffix, ESearchCase SearchCase = ESearchCase::IgnoreCase) noexcept
	{
		if( InSuffix.IsEmpty() ) return false;

		if( EndsWith(InSuffix, SearchCase) )
		{
			RemoveAt(Length() - InSuffix.Length(), InSuffix.Length());
			return true;
		}

		return false;
	}

	/**
		Insert character within the string.
	
		@param Index - The index of the first character to insert.
		@param C - Character to insert.
	*/
	FORCEINLINE void InsertAt(uint32 Index, TCHAR C) noexcept
	{
		if( C != 0 && Index < Length() )
		{
			StringBuffer.Resize(Length() + 1);
			FMemory::MemMove(StringBuffer.GetAtByPointer(Index + 1), StringBuffer.GetAtByPointer(Index), sizeof(TCHAR) * (Length() - Index - 1));
			StringBuffer[Index] = C;
		}
	}
	/**
		Insert characters within the string.
	
		@param Index - The index of the first character to insert.
		@param S - String to insert.
	*/
	FORCEINLINE void InsertAt(uint32 Index, const FString& S) noexcept
	{
		if( !S.IsEmpty() && Index < Length() )
		{
			StringBuffer.Resize(Length() + S.Length());
			FMemory::MemMove(StringBuffer.GetAtByPointer(Index + S.Length()), StringBuffer.GetAtByPointer(Index), sizeof(TCHAR) * (Length() - Index - S.Length()));
			FPlatformString::Strncpy(StringBuffer.GetAtByPointer(Index), S.GetBuffer(), S.Length());
		}
	}

	/**
		@return new string with the characters of this converted to uppercase.
	*/
	FORCEINLINE FString ToUpper() const& noexcept
	{
		FString New = *this;
		New.ToUpperInline();
		return New;
	}
	/**
		Converts all characters in this rvalue string to uppercase and moves it into the returned string.

		@return a new string with the characters of this converted to uppercase.
	*/
	FORCEINLINE FString ToUpper() && noexcept
	{
		this->ToUpperInline();
		return MoveTemp(*this);
	}
	/**
		Converts all characters in this string to uppercase.
	*/
	FORCEINLINE void ToUpperInline() noexcept
	{
		for( TCHAR& LChar : StringBuffer )
		{
			LChar = FChar::ToUpper(LChar);
		}
	}

	/**
		@return a new string with the characters of this converted to lowercase.
	*/
	FORCEINLINE FString ToLower() const& noexcept
	{
		FString New = *this;
		New.ToLowerInline();
		return New;
	}
	/**
		Converts all characters in this rvalue string to lowercase and moves it into the returned string.

		@return a new string with the characters of this converted to lowercase.
	*/
	FORCEINLINE FString ToLower() && noexcept
	{
		this->ToLowerInline();
		return MoveTemp(*this);
	}
	/** 
		Converts all characters in this string to lowercase.
	*/
	FORCEINLINE void ToLowerInline() noexcept
	{
		for( TCHAR& LChar : StringBuffer )
		{
			LChar = FChar::ToLower(LChar);
		}
	}

	/** 
		@return the left most given number of characters.
	*/
	FORCEINLINE FString Left(uint32 Count) const& noexcept
	{
		if( Count > Length() ) Count = Length();
		return FString(GetStr(), Count);
	}
	FORCEINLINE FString Left(uint32 Count) && noexcept
	{
		LeftInline(Count);
		return MoveTemp(*this);
	}
	/** 
		Modifies the string such that it is now the left most given number of characters.
	*/
	FORCEINLINE void LeftInline(uint32 Count) noexcept
	{
		if( Count > Length() ) Count = Length();
		RemoveAt(Count, Length() - Count);
	}
	/** 
		@return the left most characters from the string chopping the given number of characters from the end.
	*/
	FORCEINLINE FString LeftChop(uint32 Count) const& noexcept
	{
		if( Count > Length() ) Count = Length();
		return FString(GetStr(), Length() - Count);
	}
	FORCEINLINE FString LeftChop(uint32 Count) && noexcept
	{
		LeftChopInline(Count);
		return MoveTemp(*this);
	}
	/** 
		Modifies the string such that it is now the left most characters chopping the given number of characters from the end.
	*/
	FORCEINLINE void LeftChopInline(uint32 Count) noexcept
	{
		if( Count > Length() ) Count = Length();
		RemoveAt(Length() - Count, Count);
	}

	/**
		@return the string to the right of the specified location, counting back from the right (end of the word). 
	*/
	FORCEINLINE FString Right(uint32 Count) const& noexcept
	{
		if( Count > Length() ) Count = Length();
		return FString(StringBuffer[Length() - Count]);
	}
	FORCEINLINE FString Right(uint32 Count) && noexcept
	{
		RightInline(Count);
		return MoveTemp(*this);
	}
	/** 
		Modifies the string such that it is now the right most given number of characters.
	*/
	FORCEINLINE void RightInline(uint32 Count) noexcept
	{
		if( Count > Length() ) Count = Length();
		RemoveAt(0, Length() - Count);
	}
	/** 
		@return the string to the right of the specified location, counting forward from the left (from the beginning of the word). 
	*/
	FORCEINLINE FString RightChop(uint32 Count) const& noexcept
	{
		if( Count > Length() ) Count = Length();
		return FString(StringBuffer[Count]);
	}
	FORCEINLINE FString RightChop(uint32 Count) && noexcept
	{
		RightChopInline(Count);
		return MoveTemp(*this);
	}
	/** 
		Modifies the string such that it is now the string to the right of the specified location, counting forward from the left (from the beginning of the word).
	*/
	FORCEINLINE void RightChopInline(uint32 Count) noexcept
	{
		if( Count > Length() ) Count = Length();
		RemoveAt(0, Count);
	}

	/** 
		@return the substring from Start position for Count characters.
	*/
	FORCEINLINE FString Mid(uint32 Start, int32 Count = MAX_int32) const& noexcept
	{
		FString Result;
		if( Count >= 0 )
		{
			const int32 RequestedStart = Start;
			if( Start > Length() ) Start = Length();
			const int32 End = (int32)FMath::Clamp(static_cast<int64>(Count) + static_cast<int64>(RequestedStart), static_cast<int64>(Start), static_cast<int64>(Length()));
			Result = FString(StringBuffer.GetAtByPointer(Start), End - Start);
		}

		return Result;
	}
	FORCEINLINE FString Mid(uint32 Start, int32 Count = MAX_int32) && noexcept
	{
		MidInline(Start, Count);
		return MoveTemp(*this);
	}
	/** 
		Modifies the string such that it is now the substring from Start position for Count characters.
	*/
	FORCEINLINE void MidInline(uint32 Start, int32 Count = MAX_int32) noexcept
	{
		LeftInline((int32)FMath::Min(static_cast<int64>(Count) + static_cast<int64>(Start), static_cast<int64>(MAX_int32)));
		RightChopInline(Start);
	}
	/** 
		Alias for Mid.
		@return the substring from Start position for Count characters.
	*/
	FORCEINLINE FString SubStr(uint32 Offset, uint32 Count = MAX_int32) const& noexcept { return Mid(Offset, Count); }
	FORCEINLINE FString SubStr(uint32 Offset, uint32 Count = MAX_int32) && noexcept { return Mid(Offset, Count); }
	FORCEINLINE void SubStrInline(uint32 Offset, uint32 Count = MAX_int32) noexcept { MidInline(Offset, Count); }

	/**
		Searches the string for a substring, and returns index into this string
		of the first found instance. Can search from beginning or end, and ignore case or not.
	 
		@param SubStr - The string array of TCHAR to search for.
		@param StartPosition - The start character position to search from.
		@param SearchCase - Indicates whether the search is case sensitive or not.
		@param SearchDir - Indicates whether the search starts at the beginning or at the end.
	*/
	int32 Find(const TCHAR* SubStr, ESearchCase SearchCase = ESearchCase::IgnoreCase, ESearchDir SearchDir = ESearchDir::FromStart, int32 StartPosition = -1) const noexcept;
	/**
		Searches the string for a substring, and returns index into this string
		of the first found instance. Can search from beginning or end, and ignore case or not.
	 
		@param SubStr - The string to search for.
		@param StartPosition - The start character position to search from.
		@param SearchCase - Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase ).
		@param SearchDir - Indicates whether the search starts at the beginning or at the end ( defaults to ESearchDir::FromStart ).
	*/
	FORCEINLINE int32 Find(const FString& SubStr, ESearchCase SearchCase = ESearchCase::IgnoreCase, ESearchDir SearchDir = ESearchDir::FromStart, int32 StartPosition = -1) const noexcept
	{
		return Find(SubStr.GetStr(), SearchCase, SearchDir, StartPosition);
	}

	/**
		Returns whether this string contains the specified substring.
	 
		@param SubStr - Find to search for.
		@param SearchCase - Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase ).
		@param SearchDir - Indicates whether the search starts at the beginning or at the end ( defaults to ESearchDir::FromStart ).
		@return whether the string contains the substring.
	*/
	FORCEINLINE bool Contains(const TCHAR* SubStr, ESearchCase SearchCase = ESearchCase::IgnoreCase, ESearchDir SearchDir = ESearchDir::FromStart) const noexcept { return Find(SubStr, SearchCase, SearchDir) != -1; }
	/**
	  Returns whether this string contains the specified substring.
	 
	  @param SubStr - Find to search for.
	  @param SearchCase - Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase ).
	  @param SearchDir - Indicates whether the search starts at the beginning or at the end ( defaults to ESearchDir::FromStart ).
	  @return - whether the string contains the substring.
	*/
	FORCEINLINE bool Contains(const FString& SubStr, ESearchCase SearchCase = ESearchCase::IgnoreCase, ESearchDir SearchDir = ESearchDir::FromStart) const noexcept
	{
		return Find(SubStr.GetStr(), SearchCase, SearchDir) != -1;
	}

	/** 
		Appends the integer Num to this string.
	*/
	void AppendInt(int64 Num) noexcept;
	void AppendHexInt(uint64 Num) noexcept;

	/** 
		@return true if the string only contains numeric characters.
	*/
	bool IsNumeric() const noexcept;

public:

	/** 
		Converts an ANSI string to a string.
	*/
	static FORCEINLINE FString FromAnsi(const ANSICHAR* S) noexcept
	{
		FString Ret(FPlatformString::Strlen(S));

		TCHAR* p = Ret.begin();
		TCHAR* pEnd = Ret.end();

		while( p != pEnd )
		{
			*p = *S;
			++p;
			++S;
		}

		return Ret;
	}
	/** 
		Converts an WIDE string to a string.
	*/
	static FORCEINLINE FString FromWide(const WIDECHAR* S) noexcept
	{
		FString Ret(FPlatformString::Strlen(S));

		TCHAR* p = Ret.begin();
		TCHAR* pEnd = Ret.end();

		while( p != pEnd )
		{
			*p = *S;
			++p;
			++S;
		}

		return Ret;
	}

	/**
		Takes the number passed in and formats the string in comma format(12345 becomes "12,345").
	*/
	static FString FormatAsNumber(int32 InNumber) noexcept;
	/** 
		Converts an integer to a string.
	*/
	static FORCEINLINE FString FromInt(int32 Num) noexcept
	{
		FString Ret;
		Ret.AppendInt(Num);
		return Ret;
	}

	/**
		Converts a float string with the trailing zeros stripped.
		For example - 1.234 will be "1.234" rather than "1.234000".
	 
		@param InFloat - The float to sanitize.
		@param InMinFractionalDigits - The minimum number of fractional digits the number should have (will be padded with zero).
	 
		@return sanitized string version of float.
	*/
	static FString SanitizeFloat(double InFloat, const int32 InMinFractionalDigits = 1) noexcept;

	/**
		Convert an array of bytes to a TCHAR.

		@param In - Byte array values to convert.
		@param Count - Number of bytes to convert.
		@return Valid string representing bytes.
	*/
	static FORCEINLINE FString BytesToString(const uint8* In, int32 Count) noexcept
	{
		FString Result;

		while( Count )
		{
			// Put the byte into an int16 and add 1 to it, this keeps anything from being put into the string as a null terminator
			int16 Value = *In;
			Value += 1;

			Result += TCHAR(Value);

			++In;
			--Count;
		}

		return Result;
	}
	/**
		Convert FString of bytes into the byte array.

		@param String - The FString of byte values.
		@param OutBytes - Ptr to memory must be preallocated large enough.
		@param MaxBufferSize - Max buffer size of the OutBytes array, to prevent overflow.
		@return	The number of bytes copied.
	*/
	static FORCEINLINE int32 StringToBytes(const FString& String, uint8* OutBytes, int32 MaxBufferSize) noexcept
	{
		int32 NumBytes = 0;
		const TCHAR* CharPos = String.StringBuffer.GetBuffer();

		while( *CharPos && NumBytes < MaxBufferSize )
		{
			OutBytes[NumBytes] = (int8)(*CharPos - 1);
			++CharPos;
			++NumBytes;
		}

		return NumBytes;
	}

	/** 
		@return Char value of Nibble.
	*/
	static FORCEINLINE TCHAR NibbleToTChar(uint8 Num) noexcept
	{
		if( Num > 9 )
		{
			return TEXT('A') + TCHAR(Num - 10);
		}

		return TEXT('0') + TCHAR(Num);
	}

	static const TCHAR* Stristr(const TCHAR* Str, const TCHAR* Find) noexcept
	{
		if( Find == nullptr || Str == nullptr ) return nullptr;

		// get upper-case first letter of the find string (to reduce the number of full strnicmps)
		const TCHAR FindInitial = TChar<TCHAR>::ToUpper(*Find);
		// get length of find string, and increment past first letter
		const int32 Length = FPlatformString::Strlen(++Find) - 1;
		// get the first letter of the search string, and increment past it
		TCHAR StrChar = *Str++;
		while( StrChar )
		{
			// make sure it's upper-case
			StrChar = TChar<TCHAR>::ToUpper(StrChar);
			// if it matches the first letter of the find string, do a case-insensitive string compare for the length of the find string
			if( StrChar == FindInitial && !FPlatformString::Strnicmp(Str, Find, Length) )
			{
				// if we found the string, then return a pointer to the beginning of it in the search string
				return Str - 1;
			}
			StrChar = *Str++;
		}

		return nullptr;
	}

public:

	static FORCEINLINE void LexFromString(int8& OutValue, const TCHAR* Buffer) noexcept { OutValue = (int8)FPlatformString::Atoi(Buffer); }
	static FORCEINLINE void LexFromString(int16& OutValue, const TCHAR* Buffer) noexcept { OutValue = (int16)FPlatformString::Atoi(Buffer); }
	static FORCEINLINE void LexFromString(int32& OutValue, const TCHAR* Buffer) noexcept { OutValue = (int32)FPlatformString::Atoi(Buffer); }
	static FORCEINLINE void LexFromString(int64& OutValue, const TCHAR* Buffer) noexcept { OutValue = FPlatformString::Atoi64(Buffer); }
	static FORCEINLINE void LexFromString(uint8& OutValue, const TCHAR* Buffer) noexcept { OutValue = (uint8)FPlatformString::Atoi(Buffer); }
	static FORCEINLINE void LexFromString(uint16& OutValue, const TCHAR* Buffer) noexcept { OutValue = (uint16)FPlatformString::Atoi(Buffer); }
	static FORCEINLINE void LexFromString(uint32& OutValue, const TCHAR* Buffer) noexcept { OutValue = (uint32)FPlatformString::Atoi64(Buffer); } //64 because this unsigned and so Atoi might overflow
	static FORCEINLINE void LexFromString(uint64& OutValue, const TCHAR* Buffer) noexcept { OutValue = FPlatformString::Strtoui64(Buffer, nullptr, 0); }
	static FORCEINLINE void LexFromString(float& OutValue, const TCHAR* Buffer) noexcept { OutValue = FPlatformString::Atof(Buffer); }
	static FORCEINLINE void LexFromString(double& OutValue, const TCHAR* Buffer) noexcept { OutValue = FPlatformString::Atod(Buffer); }
	static FORCEINLINE void LexFromString(FString& OutValue, const TCHAR* Buffer) noexcept { OutValue = Buffer; }



private:

	/**
		The string buffer.
	*/
	FStringBuffer StringBuffer;
};



namespace FString_Private
{
/**
	Table of 65537^x where x in [0, 1023]
*/
extern const uint64 StringP[1024];
} // namespace FString_Private

FORCEINLINE uint64 GetTypeHash(const FString& S) noexcept
{
	check(S.Length() <= 1024);

	uint64 Result = 0;
	for( uint32 i = 0; i < S.Length(); ++i )
	{
		Result += static_cast<uint64>(S[i]) * FString_Private::StringP[i];
	}

	return Result;
}