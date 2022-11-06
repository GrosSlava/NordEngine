#pragma once

#if 0

#include "GenericPlatform.h"
#include "GenericPlatformString.h"

#include "StringBuffer.h"
#include "Char.h"


#include "EnableIf.h"
#include "IsArray.h"
#include "IsVariadic.h"

#include "EngineMath.h"
#include "NumericLimits.h"

#include "MoveSemantic.h"




#define MAX_SPRINTF 1024




template<typename T>
class TArray;






/** Determines case sensitivity options for string comparisons. */
enum class ESearchCase : uint8
{
	/** Case sensitive. Upper/lower casing must match for strings to be considered equal. */
	CaseSensitive,

	/** Ignore case. Upper/lower casing does not matter when making a comparison. */
	IgnoreCase
};


/** Determines search direction for string operations. */
enum class ESearchDir : uint8
{
	/** Search from the start, moving forward through the string. */
	FromStart,

	/** Search from the end, moving backward through the string. */
	FromEnd
};







class FString
{

//constructors
public:

	FORCEINLINE FString(uint32 UnitializedSize = 0) noexcept
	{
		SetNumUninitialized(UnitializedSize);
	}

	FORCEINLINE FString(const TCHAR NewString[]) noexcept
	{
		StringBuffer.Resize(FMicrosoftPlatformString::Strlen(NewString));
		
		FMicrosoftPlatformString::Strcpy(StringBuffer.GetBuffer(), NewString);
	}

	FORCEINLINE FString(const ANSICHAR NewString[]) noexcept
	{
		StringBuffer.Resize(FMicrosoftPlatformString::Strlen(NewString));

		int i = 0;
		for (TCHAR& LChar : StringBuffer)
		{
			LChar = (TCHAR)NewString[i];
			++i;
		}
	}

	FORCEINLINE FString(const TCHAR NewString[], uint32 Count) noexcept
	{
		StringBuffer.Resize(Count);

		FMicrosoftPlatformString::Strncpy(StringBuffer.GetBuffer(), NewString, Count + 1);
	}

	FORCEINLINE FString(const FString& OtherString) noexcept
	{
		StringBuffer.Resize(OtherString.Length());
		
		FMicrosoftPlatformString::Strcpy(StringBuffer.GetBuffer(), OtherString.GetStr());
	}

	FORCEINLINE FString(FString&& OtherString) noexcept
	{
		StringBuffer = OtherString.StringBuffer;
		
		OtherString.StringBuffer.Clear();
	}


//operators
public:

	FORCEINLINE FString& operator=(const FString& rhs)
	{
		if (this == &rhs) return *this;

		StringBuffer.Resize(rhs.Length());
		FMicrosoftPlatformString::Strcpy(StringBuffer.GetBuffer(), rhs.GetStr());

		return *this;
	}
	FORCEINLINE FString& operator=(FString&& rhs) noexcept
	{
		StringBuffer = rhs.StringBuffer;

		rhs.StringBuffer.Clear();

		return *this;
	}
	FORCEINLINE FString& operator=(TCHAR rhs)
	{
		StringBuffer.Resize(1);
		FMicrosoftPlatformString::Strcpy(StringBuffer.GetBuffer(), &rhs);

		return *this;
	}
	FORCEINLINE FString& operator=(TCHAR* rhs)
	{
		if (StringBuffer.GetBuffer() == rhs) return *this;

		StringBuffer.Resize(FMicrosoftPlatformString::Strlen(rhs));
		FMicrosoftPlatformString::Strcpy(StringBuffer.GetBuffer(), rhs);

		return *this;
	}


	FORCEINLINE FString& operator+=(const FString& rhs) 
	{ 
		return *this += rhs.GetStr(); 
	}
	FORCEINLINE FString& operator+=(TCHAR rhs)
	{
		StringBuffer.Resize(Length() + 1);
		FMicrosoftPlatformString::Strcat(StringBuffer.GetBuffer(), &rhs);

		return *this;
	}
	FORCEINLINE FString& operator+=(const TCHAR* rhs)
	{
		if (StringBuffer.GetBuffer() == rhs) return *this += FString(rhs);

		StringBuffer.Resize(Length() + FMicrosoftPlatformString::Strlen(rhs));
		FMicrosoftPlatformString::Strcat(StringBuffer.GetBuffer(), rhs);

		return *this;
	}


	friend FString operator+(const FString lhs, const FString rhs);


	FORCEINLINE TCHAR& operator[](uint32 Index)
	{
		return StringBuffer[Index];
	}	

	FORCEINLINE TCHAR operator[](uint32 Index) const
	{
		return StringBuffer[Index];
	}



	bool operator==(const FString& rhs) const;
	bool operator!=(const FString& rhs) const;


//Iteration
public:

	FORCEINLINE TCHAR* begin() const noexcept { return StringBuffer.begin(); }
	FORCEINLINE TCHAR* end() const noexcept   { return StringBuffer.end(); }
	



public:

	/* Sets string to empty */
	FORCEINLINE void Clear()
	{
		StringBuffer.Reset();
	}

	/*
		Clear old string and create empty buffer with length = Length.
	*/
	FORCEINLINE void SetNumUninitialized(uint32 Length)
	{
		StringBuffer.Reset();
		StringBuffer.Resize(Length);
	}
	
	/*
		If NewLength > StringLengt then new memory will be unitializes
	*/
	FORCEINLINE void SetLength(uint32 NewLength)
	{
		StringBuffer.Resize(NewLength);
	}






	/**
	 * Breaks up a delimited string into elements of a string array.
	 *
	 * @param	InArray		The array to fill with the string pieces
	 * @param	pchDelim	The string to delimit on
	 * @param	InCullEmpty	If 1, empty strings are not added to the array
	 *
	 * @return	The number of elements in InArray
	 */
	int32 ParseIntoArray(TArray<FString>& OutArray, const TCHAR* pchDelim, bool InCullEmpty = true) const;





	/**
	 * Returns a copy of this string, with the characters in reverse order
	 */
	FString Reverse() const&;

	/**
	 * Returns this string, with the characters in reverse order
	 */
	FString Reverse()&&;

	/**
	 * Reverses the order of characters in this string
	 */
	void ReverseString();






	/**
	 * Replace all occurrences of a substring in this string
	 *
	 * @param From substring to replace
	 * @param To substring to replace From with
	 * @param SearchCase	Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	 * @return a copy of this string with the replacement made
	 */
	FString Replace(const TCHAR* From, const TCHAR* To, ESearchCase SearchCase = ESearchCase::IgnoreCase) const&;

	/**
	 * Replace all occurrences of a substring in this string
	 *
	 * @param From substring to replace
	 * @param To substring to replace From with
	 * @param SearchCase	Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	 * @return a copy of this string with the replacement made
	 */
	FString Replace(const TCHAR* From, const TCHAR* To, ESearchCase SearchCase = ESearchCase::IgnoreCase)&&;

	/**
	 * Replace all occurrences of SearchText with ReplacementText in this string.
	 *
	 * @param	SearchText	the text that should be removed from this string
	 * @param	ReplacementText		the text to insert in its place
	 * @param SearchCase	Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	 *
	 * @return	the number of occurrences of SearchText that were replaced.
	 */
	int32 ReplaceInline(const TCHAR* SearchText, const TCHAR* ReplacementText, ESearchCase SearchCase = ESearchCase::IgnoreCase);
	
	/**
	 * Replaces certain characters with the "escaped" version of that character (i.e. replaces "\n" with "\\n").
	 * The characters supported are: { \n, \r, \t, \', \", \\ }.
	 *
	 * @param	Chars	by default, replaces all supported characters; this parameter allows you to limit the replacement to a subset.
	 *
	 * @return	a string with all control characters replaced by the escaped version.
	 */
	FString ReplaceCharWithEscapedChar(const TArray<TCHAR>* Chars = nullptr) const&;
	
	/**
	 * Replaces certain characters with the "escaped" version of that character (i.e. replaces "\n" with "\\n").
	 * The characters supported are: { \n, \r, \t, \', \", \\ }.
	 *
	 * @param	Chars	by default, replaces all supported characters; this parameter allows you to limit the replacement to a subset.
	 *
	 * @return	a string with all control characters replaced by the escaped version.
	 */
	FString ReplaceCharWithEscapedChar(const TArray<TCHAR>* Chars = nullptr)&&;

	/**
	 * Removes the escape backslash for all supported characters, replacing the escape and character with the non-escaped version.  (i.e.
	 * replaces "\\n" with "\n".  Counterpart to ReplaceCharWithEscapedChar().
	 * @return copy of this string with replacement made
	 */
	FString ReplaceEscapedCharWithChar(const TArray<TCHAR>* Chars = nullptr) const&;
	
	/**
	 * Removes the escape backslash for all supported characters, replacing the escape and character with the non-escaped version.  (i.e.
	 * replaces "\\n" with "\n".  Counterpart to ReplaceCharWithEscapedChar().
	 * @return copy of this string with replacement made
	 */
	FString ReplaceEscapedCharWithChar(const TArray<TCHAR>* Chars = nullptr)&&;




	/**
	 * Replaces all instances of '\t' with TabWidth number of spaces
	 * @param InSpacesPerTab - Number of spaces that a tab represents
	 */
	void ConvertTabsToSpacesInline(const uint32 InSpacesPerTab);

	/**
	 * Replaces all instances of '\t' with TabWidth number of spaces
	 * @param InSpacesPerTab - Number of spaces that a tab represents
	 * @return copy of this string with replacement made
	 */
	FString ConvertTabsToSpaces(const int32 InSpacesPerTab) const&;
	
	/**
	 * Replaces all instances of '\t' with TabWidth number of spaces
	 * @param InSpacesPerTab - Number of spaces that a tab represents
	 * @return copy of this string with replacement made
	 */
	FString ConvertTabsToSpaces(const int32 InSpacesPerTab)&&;
	



	/**
	 * Test whether this string starts with given string.
	 *
	 * @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	 * @return true if this string begins with specified text, false otherwise
	*/
	bool StartsWith(const TCHAR* InPrefix, ESearchCase SearchCase = ESearchCase::IgnoreCase) const;

	/**
	 * Test whether this string starts with given string.
	 *
	 * @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	 * @return true if this string begins with specified text, false otherwise
	 */
	bool StartsWith(const FString& InPrefix, ESearchCase SearchCase = ESearchCase::IgnoreCase) const;

	/**
	 * Test whether this string ends with given string.
	 *
	 * @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	 * @return true if this string ends with specified text, false otherwise
	 */
	bool EndsWith(const TCHAR* InSuffix, ESearchCase SearchCase = ESearchCase::IgnoreCase) const;

	/**
	 * Test whether this string ends with given string.
	 *
	 * @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	 * @return true if this string ends with specified text, false otherwise
	 */
	bool EndsWith(const FString& InSuffix, ESearchCase SearchCase = ESearchCase::IgnoreCase) const;




	/**
	* Removes characters within the string.
	*
	* @param Index           The index of the first character to remove.
	* @param Count           The number of characters to remove.
	* @param AllowShrinking  Whether or not to reallocate to shrink the storage after removal.
	*/
	void RemoveAt(uint32 Index, uint32 Count = 1);
	
	void InsertAt(uint32 Index, TCHAR Character);
	
	void InsertAt(uint32 Index, const FString& Characters);
	

	/**
	 * Removes the text from the start of the string if it exists.
	 *
	 * @param InPrefix the prefix to search for at the start of the string to remove.
	 * @return true if the prefix was removed, otherwise false.
	 */
	bool RemoveFromStart(const TCHAR* InPrefix, ESearchCase SearchCase = ESearchCase::IgnoreCase);

	/**
	 * Removes the text from the start of the string if it exists.
	 *
	 * @param InPrefix the prefix to search for at the start of the string to remove.
	 * @return true if the prefix was removed, otherwise false.
	 */
	bool RemoveFromStart(const FString& InPrefix, ESearchCase SearchCase = ESearchCase::IgnoreCase);

	/**
	 * Removes the text from the end of the string if it exists.
	 *
	 * @param InSuffix the suffix to search for at the end of the string to remove.
	 * @return true if the suffix was removed, otherwise false.
	 */
	bool RemoveFromEnd(const TCHAR* InSuffix, ESearchCase SearchCase = ESearchCase::IgnoreCase);

	/**
	 * Removes the text from the end of the string if it exists.
	 *
	 * @param InSuffix the suffix to search for at the end of the string to remove.
	 * @return true if the suffix was removed, otherwise false.
	 */
	bool RemoveFromEnd(const FString& InSuffix, ESearchCase SearchCase = ESearchCase::IgnoreCase);






	/** appends the integer Num to this string */
	void AppendInt(int32 Num);







	/** Returns a new string with the characters of this converted to uppercase */
	FORCEINLINE FString ToUpper() const&
	{
		FString New = *this;
		New.ToUpperInline();
		return New;
	}

	/**
	 * Converts all characters in this rvalue string to uppercase and moves it into the returned string.
	 * @return a new string with the characters of this converted to uppercase
	 */
	FORCEINLINE FString ToUpper()&&
	{
		this->ToUpperInline();
		return MoveTemp(*this);
	}

	/** Converts all characters in this string to uppercase */
	FORCEINLINE void ToUpperInline()
	{
		for (TCHAR& LChar : StringBuffer)
		{
			LChar = FChar::ToUpper(LChar);
		}
	}



	/** Returns a new string with the characters of this converted to lowercase */
	FORCEINLINE FString ToLower() const&
	{
		FString New = *this;
		New.ToLowerInline();
		return New;
	}

	/**
	 * Converts all characters in this rvalue string to lowercase and moves it into the returned string.
	 * @return a new string with the characters of this converted to lowercase
	 */
	FORCEINLINE FString ToLower()&&
	{
		this->ToLowerInline();
		return MoveTemp(*this);
	}

	/** Converts all characters in this string to lowercase */
	FORCEINLINE void ToLowerInline()
	{
		for (TCHAR& LChar : StringBuffer)
		{
			LChar = FChar::ToLower(LChar);
		}
	}



	/** Returns the left most given number of characters */
	FORCEINLINE FString Left(uint32 Count) const&
	{
		if (Count > Length()) Count = Length();
		return FString(GetStr(), Count);
	}

	FORCEINLINE FString Left(uint32 Count)&&
	{
		LeftInline(Count);
		return MoveTemp(*this);
	}

	/** Modifies the string such that it is now the left most given number of characters */
	FORCEINLINE void LeftInline(uint32 Count)
	{
		if (Count > Length()) Count = Length();
		RemoveAt(Count, Length() - Count);
	}

	/** Returns the left most characters from the string chopping the given number of characters from the end */
	FORCEINLINE FString LeftChop(uint32 Count) const&
	{
		if (Count > Length()) Count = Length();
		return FString(GetStr(), Length() - Count);
	}

	FORCEINLINE FString LeftChop(uint32 Count)&&
	{
		LeftChopInline(Count);
		return MoveTemp(*this);
	}

	/** Modifies the string such that it is now the left most characters chopping the given number of characters from the end */
	FORCEINLINE void LeftChopInline(uint32 Count)
	{
		if (Count > Length()) Count = Length();
		RemoveAt(Length() - Count, Count);
	}




	/** Returns the string to the right of the specified location, counting back from the right (end of the word). */
	FORCEINLINE FString Right(uint32 Count) const&
	{
		if (Count > Length()) Count = Length();
		return FString(StringBuffer[Length() - Count]);
	}

	FORCEINLINE FString Right(uint32 Count)&&
	{
		RightInline(Count);
		return MoveTemp(*this);
	}

	/** Modifies the string such that it is now the right most given number of characters */
	FORCEINLINE void RightInline(uint32 Count)
	{
		if (Count > Length()) Count = Length();
		RemoveAt(0, Length() - Count);
	}

	/** Returns the string to the right of the specified location, counting forward from the left (from the beginning of the word). */
	FORCEINLINE FString RightChop(uint32 Count) const&
	{
		if (Count > Length()) Count = Length();
		return FString(StringBuffer[Count]);
	}

	FORCEINLINE FString RightChop(uint32 Count)&&
	{
		RightChopInline(Count);
		return MoveTemp(*this);
	}

	/** Modifies the string such that it is now the string to the right of the specified location, counting forward from the left (from the beginning of the word). */
	FORCEINLINE void RightChopInline(uint32 Count)
	{
		if (Count > Length()) Count = Length();
		RemoveAt(0, Count);
	}




	/** Returns the substring from Start position for Count characters. */
	FORCEINLINE FString Mid(uint32 Start, int32 Count = MAX_int32) const&
	{
		FString Result;
		if (Count >= 0)
		{
			const int32 RequestedStart = Start;
			if (Start > Length()) Start = Length();
			const int32 End = (int32)FMath::Clamp((int64)Count + RequestedStart, (int64)Start, (int64)Length());
			Result = FString(StringBuffer.GetAtByPointer(Start), End - Start);
		}

		return Result;
	}

	FORCEINLINE FString Mid(uint32 Start, int32 Count = MAX_int32)&&
	{
		MidInline(Start, Count);
		return MoveTemp(*this);
	}

	/** Modifies the string such that it is now the substring from Start position for Count characters. */
	FORCEINLINE void MidInline(uint32 Start, int32 Count = MAX_int32)
	{
		LeftInline((int32)FMath::Min((int64)Count + Start, (int64)MAX_int32));
		RightChopInline(Start);
	}





	/**
	 * Searches the string for a substring, and returns index into this string
	 * of the first found instance. Can search from beginning or end, and ignore case or not.
	 *
	 * @param SubStr			The string array of TCHAR to search for
	 * @param StartPosition		The start character position to search from
	 * @param SearchCase		Indicates whether the search is case sensitive or not
	 * @param SearchDir			Indicates whether the search starts at the beginning or at the end.
	 */
	int32 Find(const TCHAR* SubStr, ESearchCase SearchCase = ESearchCase::IgnoreCase,
		ESearchDir SearchDir = ESearchDir::FromStart, int32 StartPosition = -1) const;

	/**
	 * Searches the string for a substring, and returns index into this string
	 * of the first found instance. Can search from beginning or end, and ignore case or not.
	 *
	 * @param SubStr			The string to search for
	 * @param StartPosition		The start character position to search from
	 * @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	 * @param SearchDir			Indicates whether the search starts at the beginning or at the end ( defaults to ESearchDir::FromStart )
	 */
	FORCEINLINE int32 Find(const FString& SubStr, ESearchCase SearchCase = ESearchCase::IgnoreCase,
		ESearchDir SearchDir = ESearchDir::FromStart, int32 StartPosition = -1) const
	{
		return Find(SubStr.GetStr(), SearchCase, SearchDir, StartPosition);
	}

	/**
	 * Returns whether this string contains the specified substring.
	 *
	 * @param SubStr			Find to search for
	 * @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	 * @param SearchDir			Indicates whether the search starts at the beginning or at the end ( defaults to ESearchDir::FromStart )
	 * @return					Returns whether the string contains the substring
	 **/
	FORCEINLINE bool Contains(const TCHAR* SubStr, ESearchCase SearchCase = ESearchCase::IgnoreCase,
		ESearchDir SearchDir = ESearchDir::FromStart) const
	{
		return Find(SubStr, SearchCase, SearchDir) != -1;
	}

	/**
	 * Returns whether this string contains the specified substring.
	 *
	 * @param SubStr			Find to search for
	 * @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	 * @param SearchDir			Indicates whether the search starts at the beginning or at the end ( defaults to ESearchDir::FromStart )
	 * @return					Returns whether the string contains the substring
	 **/
	FORCEINLINE bool Contains(const FString& SubStr, ESearchCase SearchCase = ESearchCase::IgnoreCase,
		ESearchDir SearchDir = ESearchDir::FromStart) const
	{
		return Find(SubStr.GetStr(), SearchCase, SearchDir) != -1;
	}






	FORCEINLINE bool IsEmpty() const noexcept {	return Length() == 0; }
	FORCEINLINE uint32 Length() const noexcept { return StringBuffer.GetLength(); }

	/** Returns true if the string only contains numeric characters */
	bool IsNumeric() const;

	FORCEINLINE const TCHAR* const GetStr() const noexcept { return StringBuffer.GetBuffer(); }
	FORCEINLINE TCHAR* const GetBuffer() const noexcept { return StringBuffer.GetBuffer(); }



public:

	/**
	 * Constructs FString object similarly to how classic sprintf works.
	 *
	 * @param Format	Format string that specifies how FString should be built optionally using additional args. Refer to standard printf format.
	 * @param ...		Depending on format function may require additional arguments to build output object.
	 *
	 * @returns FString object that was constructed using format and additional parameters.
	 */
	template <typename FmtType, typename... Types>
	static typename TEnableIf<TIsArrayOrRefOfType<FmtType, TCHAR>::Value, FString>::Type Printf(const FmtType& Fmt, Types... Args)
	{
		static_assert(TIsArrayOrRefOfType<FmtType, TCHAR>::Value, "Formatting string must be a TCHAR array.");
		static_assert(TAnd<TIsValidVariadicFunctionArg<Types>...>::Value, "Invalid argument(s) passed to FString::Printf");

		return PrintfImpl(Fmt, Args...);
	}

	/**
	 * Just like Printf, but appends the formatted text to the existing FString instead.
	 * @return a reference to the modified string, so that it can be chained
	 */
	template <typename FmtType, typename... Types>
	typename TEnableIf<TIsArrayOrRefOfType<FmtType, TCHAR>::Value, FString&>::Type Appendf(const FmtType& Fmt, Types... Args)
	{
		static_assert(TIsArrayOrRefOfType<FmtType, TCHAR>::Value, "Formatting string must be a TCHAR array.");
		static_assert(TAnd<TIsValidVariadicFunctionArg<Types>...>::Value, "Invalid argument(s) passed to FString::Appendf");

		AppendfImpl(*this, Fmt, Args...);
		return *this;
	}

	/**
	* Standard string formatted print.
	* @warning: make sure code using FCString::Sprintf allocates enough (>= MAX_SPRINTF) memory for the destination buffer
	*/
	template <typename FmtType, typename... Types>
	static int32 Sprintf(TCHAR* Dest, const FmtType& Fmt, Types... Args)
	{
		static_assert(TIsArrayOrRefOfType<FmtType, TCHAR>::Value, "Formatting string must be a literal string of the same character type as template.");
		static_assert(TAnd<TIsValidVariadicFunctionArg<Types>...>::Value, "Invalid argument(s) passed to TCString::Sprintf");

		return SprintfImpl(Dest, Fmt, Args...);
	}

	/**
	 * Safe string formatted print.
	 */
	template <typename FmtType, typename... Types>
	static int32 Snprintf(TCHAR* Dest, int32 DestSize, const FmtType& Fmt, Types... Args)
	{
		static_assert(TIsArrayOrRefOfType<FmtType, TCHAR>::Value, "Formatting string must be a literal string of the same character type as template.");
		static_assert(TAnd<TIsValidVariadicFunctionArg<Types>...>::Value, "Invalid argument(s) passed to TCString::Snprintf");

		return SnprintfImpl(Dest, DestSize, Fmt, Args...);
	}

private:

	static FString VARARGS PrintfImpl(const TCHAR* Fmt, ...);
	static void VARARGS AppendfImpl(FString& AppendToMe, const TCHAR* Fmt, ...);

	static int32 VARARGS SprintfImpl(TCHAR* Dest, const TCHAR* Fmt, ...);
	static int32 VARARGS SnprintfImpl(TCHAR* Dest, int32 DestSize, const TCHAR* Fmt, ...);




public:

	// Takes the number passed in and formats the string in comma format ( 12345 becomes "12,345")
	static FString FormatAsNumber(int32 InNumber);

	/** Converts an integer to a string. */
	static FORCEINLINE FString FromInt(int32 Num)
	{
		FString Ret;
		Ret.AppendInt(Num);
		return Ret;
	}

	/**
	 * Converts a float string with the trailing zeros stripped
	 * For example - 1.234 will be "1.234" rather than "1.234000"
	 *
	 * @param	InFloat					The float to sanitize
	 * @param	InMinFractionalDigits	The minimum number of fractional digits the number should have (will be padded with zero)
	 *
	 * @return sanitized string version of float
	 */
	static FString SanitizeFloat(double InFloat, const int32 InMinFractionalDigits = 1);
	/**
	 * Convert an array of bytes to a TCHAR
	 * @param In byte array values to convert
	 * @param Count number of bytes to convert
	 * @return Valid string representing bytes.
	 */

	static FORCEINLINE FString BytesToString(const uint8* In, int32 Count)
	{
		FString Result;

		while (Count)
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
	 * Convert FString of bytes into the byte array.
	 * @param String		The FString of byte values
	 * @param OutBytes		Ptr to memory must be preallocated large enough
	 * @param MaxBufferSize	Max buffer size of the OutBytes array, to prevent overflow
	 * @return	The number of bytes copied
	 */
	static FORCEINLINE int32 StringToBytes(const FString& String, uint8* OutBytes, int32 MaxBufferSize)
	{
		int32 NumBytes = 0;
		const TCHAR* CharPos = String.StringBuffer.GetBuffer();

		while (*CharPos && NumBytes < MaxBufferSize)
		{
			OutBytes[NumBytes] = (int8)(*CharPos - 1);
			++CharPos;
			++NumBytes;
		}

		return NumBytes;
	}

	/** Returns Char value of Nibble */
	static FORCEINLINE TCHAR NibbleToTChar(uint8 Num)
	{
		if (Num > 9)
		{
			return TEXT('A') + TCHAR(Num - 10);
		}

		return TEXT('0') + TCHAR(Num);
	}





	static const TCHAR* Stristr(const TCHAR* Str, const TCHAR* Find)
	{
		// both strings must be valid
		if (Find == NULL || Str == NULL)
		{
			return NULL;
		}


		// get upper-case first letter of the find string (to reduce the number of full strnicmps)
		TCHAR FindInitial = TChar<TCHAR>::ToUpper(*Find);
		// get length of find string, and increment past first letter
		int32   Length = FMicrosoftPlatformString::Strlen(++Find) - 1;
		// get the first letter of the search string, and increment past it
		TCHAR StrChar = *Str++;
		// while we aren't at end of string...
		while (StrChar)
		{
			// make sure it's upper-case
			StrChar = TChar<TCHAR>::ToUpper(StrChar);
			// if it matches the first letter of the find string, do a case-insensitive string compare for the length of the find string
			if (StrChar == FindInitial && !FMicrosoftPlatformString::Strnicmp(Str, Find, Length))
			{
				// if we found the string, then return a pointer to the beginning of it in the search string
				return Str - 1;
			}
			// go to next letter
			StrChar = *Str++;
		}

		// if nothing was found, return NULL
		return NULL;
	}


/** Covert a string buffer to intrinsic types */
public:

	static FORCEINLINE void LexFromString(int8& OutValue, const TCHAR* Buffer) noexcept		{ OutValue = (int8)FMicrosoftPlatformString::Atoi(Buffer); }
	static FORCEINLINE void LexFromString(int16& OutValue, const TCHAR* Buffer) noexcept	{ OutValue = (int16)FMicrosoftPlatformString::Atoi(Buffer); }
	static FORCEINLINE void LexFromString(int32& OutValue, const TCHAR* Buffer) noexcept	{ OutValue = (int32)FMicrosoftPlatformString::Atoi(Buffer); }
	static FORCEINLINE void LexFromString(int64& OutValue, const TCHAR* Buffer) noexcept	{ OutValue = FMicrosoftPlatformString::Atoi64(Buffer); }
	static FORCEINLINE void LexFromString(uint8& OutValue, const TCHAR* Buffer) noexcept	{ OutValue = (uint8)FMicrosoftPlatformString::Atoi(Buffer); }
	static FORCEINLINE void LexFromString(uint16& OutValue, const TCHAR* Buffer) noexcept	{ OutValue = (uint16)FMicrosoftPlatformString::Atoi(Buffer); }
	static FORCEINLINE void LexFromString(uint32& OutValue, const TCHAR* Buffer) noexcept	{ OutValue = (uint32)FMicrosoftPlatformString::Atoi64(Buffer); }	//64 because this unsigned and so Atoi might overflow
	static FORCEINLINE void LexFromString(uint64& OutValue, const TCHAR* Buffer) noexcept	{ OutValue = FMicrosoftPlatformString::Strtoui64(Buffer, nullptr, 0); }
	static FORCEINLINE void LexFromString(float& OutValue, const TCHAR* Buffer)	noexcept	{ OutValue = FMicrosoftPlatformString::Atof(Buffer); }
	static FORCEINLINE void LexFromString(double& OutValue, const TCHAR* Buffer) noexcept	{ OutValue = FMicrosoftPlatformString::Atod(Buffer); }
	static FORCEINLINE void LexFromString(FString& OutValue, const TCHAR* Buffer) noexcept	{ OutValue = Buffer; }




private:

	FStringBuffer StringBuffer;

};

#endif