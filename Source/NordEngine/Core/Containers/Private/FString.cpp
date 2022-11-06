#if 0
#include "FString.h"

#include "TArray.h"
#include "EngineMemory.h"

#include "CodeAnalysis.h"
#include "CommonMacros.h"
#include "VarArgs.h"





FString operator+(const FString lhs, const FString rhs)
{
	uint32 LLength = lhs.Length() + rhs.Length();


	TCHAR* LBuff = new TCHAR[LLength + 1];

	FMicrosoftPlatformString::Strcpy(LBuff, lhs.GetStr());
	FMicrosoftPlatformString::Strcat(LBuff, rhs.GetStr());

	
	FString LString(LBuff);
	delete[] LBuff;

	return LString;
}






bool FString::operator==(const FString& rhs) const
{
	if (this == &rhs) return true;
	if (Length() != rhs.Length()) return false;

	for (uint32 i = 0; i < Length(); ++i)
	{
		if (StringBuffer[i] != rhs.StringBuffer[i]) return false;
	}

	return true;
}

bool FString::operator!=(const FString& rhs) const
{
	return !(*this == rhs);
}









int32 FString::ParseIntoArray(TArray<FString>& OutArray, const TCHAR* pchDelim, bool InCullEmpty) const
{
	// Make sure the delimit string is not null or empty
	CA_ASSUME(pchDelim);

	OutArray.Reset();

	const TCHAR* Start = GetStr();
	const int32 DelimLength = FMicrosoftPlatformString::Strlen(pchDelim);
	if (Start && *Start != TEXT('\0') && DelimLength)
	{
		while (const TCHAR* At = FMicrosoftPlatformString::Strstr(Start, pchDelim))
		{
			if (!InCullEmpty || At - Start)
			{
				OutArray.AddAfter(PTRDIFF_TO_INT32(At - Start), Start);
			}

			Start = At + DelimLength;
		}

		if (!InCullEmpty || *Start)
		{
			OutArray.PushBack(Start);
		}

	}

	return OutArray.Num();
}






FString FString::Reverse() const&
{
	FString New(*this);
	New.ReverseString();
	return New;
}

FString FString::Reverse()&&
{
	ReverseString();
	return MoveTemp(*this);
}

void FString::ReverseString()
{
	if (Length() > 0)
	{
		TCHAR* StartChar = StringBuffer.GetAtByPointer(0);
		TCHAR* EndChar = StringBuffer.GetAtByPointer(Length() - 1);
		TCHAR TempChar;
		do
		{
			TempChar = *StartChar;	// store the current value of StartChar
			*StartChar = *EndChar;	// change the value of StartChar to the value of EndChar
			*EndChar = TempChar;	// change the value of EndChar to the character that was previously at StartChar

			++StartChar;
			--EndChar;

		} while (StartChar < EndChar);	// repeat until we've reached the midpoint of the string
	}
}







FString FString::Replace(const TCHAR* From, const TCHAR* To, ESearchCase SearchCase) const&
{
	//TODO

	return FString();
}

FString FString::Replace(const TCHAR* From, const TCHAR* To, ESearchCase SearchCase)&&
{
	ReplaceInline(From, To, SearchCase);
	return MoveTemp(*this);
}

int32 FString::ReplaceInline(const TCHAR* SearchText, const TCHAR* ReplacementText, ESearchCase SearchCase)
{
	//TODO

	return 0;
}


FString FString::ReplaceCharWithEscapedChar(const TArray<TCHAR>* Chars) const&
{
	FString Result(*this);
	return MoveTemp(Result).ReplaceCharWithEscapedChar(Chars);
}



static const TCHAR* CharToEscapeSeqMap[][2] =
{
	// Always replace \\ first to avoid double-escaping characters
	{ TEXT("\\"), TEXT("\\\\") },
	{ TEXT("\n"), TEXT("\\n")  },
	{ TEXT("\r"), TEXT("\\r")  },
	{ TEXT("\t"), TEXT("\\t")  },
	{ TEXT("\'"), TEXT("\\'")  },
	{ TEXT("\""), TEXT("\\\"") }
};

static const uint32 MaxSupportedEscapeChars = 6;

FString FString::ReplaceCharWithEscapedChar(const TArray<TCHAR>* Chars)&&
{
	if (Length() > 0 && (Chars == nullptr || Chars->Num() > 0))
	{
		FString Result(*this);
		for (int32 ChIdx = 0; ChIdx < MaxSupportedEscapeChars; ++ChIdx)
		{
			if (Chars == nullptr || Chars->Contains(*(CharToEscapeSeqMap[ChIdx][0])))
			{
				// use ReplaceInline as that won't create a copy of the string if the character isn't found
				Result.ReplaceInline(CharToEscapeSeqMap[ChIdx][0], CharToEscapeSeqMap[ChIdx][1]);
			}
		}
		return Result;
	}

	return *this;
}

FString FString::ReplaceEscapedCharWithChar(const TArray<TCHAR>* Chars) const&
{
	FString Result(*this);
	return MoveTemp(Result).ReplaceEscapedCharWithChar(Chars);
}

FString FString::ReplaceEscapedCharWithChar(const TArray<TCHAR>* Chars)&&
{
	if (Length() > 0 && (Chars == nullptr || Chars->Num() > 0))
	{
		// Spin CharToEscapeSeqMap backwards to ensure we're doing the inverse of ReplaceCharWithEscapedChar
		for (int32 ChIdx = MaxSupportedEscapeChars - 1; ChIdx >= 0; --ChIdx)
		{
			if (Chars == nullptr || Chars->Contains(*(CharToEscapeSeqMap[ChIdx][0])))
			{
				// use ReplaceInline as that won't create a copy of the string if the character isn't found
				ReplaceInline(CharToEscapeSeqMap[ChIdx][1], CharToEscapeSeqMap[ChIdx][0]);
			}
		}
	}

	return MoveTemp(*this);
}






void FString::ConvertTabsToSpacesInline(const uint32 InSpacesPerTab)
{
	//must call this with at least 1 space so the modulus operation works
	if (InSpacesPerTab == 0) return;


	int32 TabIndex;
	while ((TabIndex = Find(TEXT("\t"), ESearchCase::CaseSensitive)) != -1)
	{
		FString RightSide = Mid(TabIndex + 1);
		LeftInline(TabIndex);

		//for a tab size of 4, 
		int32 LineBegin = Find(TEXT("\n"), ESearchCase::CaseSensitive, ESearchDir::FromEnd, TabIndex);
		if (LineBegin == -1)
		{
			LineBegin = 0;
		}
		const int32 CharactersOnLine = (Length() - LineBegin);

		int32 NumSpacesForTab = InSpacesPerTab - (CharactersOnLine % InSpacesPerTab);
		for (int32 i = 0; i < NumSpacesForTab; ++i)
		{
			*this += ' ';
		}

		*this += RightSide;
	}
}

FString FString::ConvertTabsToSpaces(const int32 InSpacesPerTab) const&
{
	FString FinalString(*this);
	FinalString.ConvertTabsToSpacesInline(InSpacesPerTab);
	return FinalString;
}

FString FString::ConvertTabsToSpaces(const int32 InSpacesPerTab)&&
{
	ConvertTabsToSpacesInline(InSpacesPerTab);
	return MoveTemp(*this);
}





bool FString::StartsWith(const TCHAR* InPrefix, ESearchCase SearchCase) const
{
	if (SearchCase == ESearchCase::IgnoreCase)
	{
		return InPrefix && *InPrefix && !FMicrosoftPlatformString::Strnicmp(GetStr(), InPrefix, FMicrosoftPlatformString::Strlen(InPrefix));
	}
	else
	{
		return InPrefix && *InPrefix && !FMicrosoftPlatformString::Strncmp(GetStr(), InPrefix, FMicrosoftPlatformString::Strlen(InPrefix));
	}
}

bool FString::StartsWith(const FString& InPrefix, ESearchCase SearchCase) const
{
	if (SearchCase == ESearchCase::IgnoreCase)
	{
		return InPrefix.Length() > 0 && !FMicrosoftPlatformString::Strnicmp(GetStr(), InPrefix.GetStr(), InPrefix.Length());
	}
	else
	{
		return InPrefix.Length() > 0 && !FMicrosoftPlatformString::Strncmp(GetStr(), InPrefix.GetStr(), InPrefix.Length());
	}
}

bool FString::EndsWith(const TCHAR* InSuffix, ESearchCase SearchCase) const
{
	if (!InSuffix || *InSuffix == TEXT('\0'))
	{
		return false;
	}


	int32 SuffixLen = FMicrosoftPlatformString::Strlen(InSuffix);
	if (SuffixLen > (int32)Length())
	{
		return false;
	}

	if (SearchCase == ESearchCase::IgnoreCase)
	{
		return !FMicrosoftPlatformString::Stricmp(StringBuffer.GetAtByPointer(Length() - SuffixLen), InSuffix);
	}
	else
	{
		return !FMicrosoftPlatformString::Strcmp(StringBuffer.GetAtByPointer(Length() - SuffixLen), InSuffix);
	}
}

bool FString::EndsWith(const FString& InSuffix, ESearchCase SearchCase) const
{
	if (SearchCase == ESearchCase::IgnoreCase)
	{
		return	InSuffix.Length() > 0 && Length() >= InSuffix.Length() &&
				!FMicrosoftPlatformString::Stricmp(StringBuffer.GetAtByPointer(Length() - InSuffix.Length()), InSuffix.GetStr());
	}
	else
	{
		return	InSuffix.Length() > 0 && Length() >= InSuffix.Length() &&
				!FMicrosoftPlatformString::Strcmp(StringBuffer.GetAtByPointer(Length() - InSuffix.Length()), InSuffix.GetStr());
	}
}





void FString::RemoveAt(uint32 Index, uint32 Count)
{
	if (Index < Length())
	{
		if (Index + Count > Length()) Count = Length() - Index;

		TCHAR* Buff = new TCHAR[Length() - Count + 1];

		FMicrosoftPlatformString::Strncpy(Buff, StringBuffer.GetBuffer(), Index - 1);
		FMicrosoftPlatformString::Strcat(Buff, StringBuffer.GetAtByPointer(Index + Count - 1));

		StringBuffer.SetBuffer(Buff);

	}
}

void FString::InsertAt(uint32 Index, TCHAR Character)
{
	if (Character != 0 && Index < Length())
	{
		TCHAR* Buff = new TCHAR[Length() + 1 + 1];

		FMicrosoftPlatformString::Strncpy(Buff, StringBuffer.GetBuffer(), Index - 1);
		Buff[Index] = Character;
		Buff[Index + 1] = '\0';
		FMicrosoftPlatformString::Strcat(Buff, StringBuffer.GetAtByPointer(Index));

		StringBuffer.SetBuffer(Buff);

	}
}

void FString::InsertAt(uint32 Index, const FString& Characters)
{
	if (!Characters.IsEmpty() && Index < Length())
	{

		TCHAR* Buff = new TCHAR[Length() + Characters.Length() + 1];

		FMicrosoftPlatformString::Strncpy(Buff, StringBuffer.GetBuffer(), Index - 1);
		FMicrosoftPlatformString::Strcat(Buff, Characters.GetStr());
		FMicrosoftPlatformString::Strcat(Buff, StringBuffer.GetAtByPointer(Index));

		StringBuffer.SetBuffer(Buff);
	}
}







bool FString::RemoveFromStart(const TCHAR* InPrefix, ESearchCase SearchCase)
{
	if (*InPrefix == 0)
	{
		return false;
	}

	if (StartsWith(InPrefix, SearchCase))
	{
		RemoveAt(0, FMicrosoftPlatformString::Strlen(InPrefix));
		return true;
	}

	return false;
}

bool FString::RemoveFromStart(const FString& InPrefix, ESearchCase SearchCase)
{
	if (InPrefix.IsEmpty())
	{
		return false;
	}

	if (StartsWith(InPrefix, SearchCase))
	{
		RemoveAt(0, InPrefix.Length());
		return true;
	}

	return false;
}

bool FString::RemoveFromEnd(const TCHAR* InSuffix, ESearchCase SearchCase)
{
	if (*InSuffix == 0)
	{
		return false;
	}

	if (EndsWith(InSuffix, SearchCase))
	{
		int32 SuffixLen = FMicrosoftPlatformString::Strlen(InSuffix);
		RemoveAt(Length() - SuffixLen, SuffixLen);
		return true;
	}

	return false;
}

bool FString::RemoveFromEnd(const FString& InSuffix, ESearchCase SearchCase)
{
	if (InSuffix.IsEmpty())
	{
		return false;
	}

	if (EndsWith(InSuffix, SearchCase))
	{
		RemoveAt(Length() - InSuffix.Length(), InSuffix.Length());
		return true;
	}

	return false;
}







void FString::AppendInt(int32 Num)
{
	const TCHAR* DigitToChar = TEXT("9876543210123456789");
	constexpr int32 ZeroDigitIndex = 9;
	bool bIsNumberNegative = Num < 0;
	const int32 TempBufferSize = 16; // 16 is big enough
	TCHAR TempNum[TempBufferSize];
	int32 TempAt = TempBufferSize; // fill the temp string from the top down.

	// Convert to string assuming base ten.
	do
	{
		TempNum[--TempAt] = DigitToChar[ZeroDigitIndex + (Num % 10)];
		Num /= 10;
	} while (Num);

	if (bIsNumberNegative)
	{
		TempNum[--TempAt] = TEXT('-');
	}

	const TCHAR* CharPtr = TempNum + TempAt;
	const int32 NumChars = TempBufferSize - TempAt;

	*this += FString(CharPtr);
}






// This starting size catches 99.97% of printf calls - there are about 700k printf calls per level
#define STARTING_BUFFER_SIZE		512

FString FString::PrintfImpl(const TCHAR* Fmt, ...)
{
	int32		BufferSize = STARTING_BUFFER_SIZE;
	TCHAR		StartingBuffer[STARTING_BUFFER_SIZE];
	TCHAR*		Buffer = StartingBuffer;
	int32		Result = -1;

	// First try to print to a stack allocated location 
	GET_VARARGS_RESULT(Buffer, BufferSize, BufferSize - 1, Fmt, Fmt, Result);

	// If that fails, start allocating regular memory
	if (Result == -1)
	{
		Buffer = nullptr;
		while (Result == -1)
		{
			BufferSize *= 2;
			Buffer = (TCHAR*)FMemory::Realloc(Buffer, BufferSize * sizeof(TCHAR));
			GET_VARARGS_RESULT(Buffer, BufferSize, BufferSize - 1, Fmt, Fmt, Result);
		};
	}

	Buffer[Result] = 0;

	FString ResultString(Buffer);

	if (BufferSize != STARTING_BUFFER_SIZE)
	{
		FMemory::Free(Buffer);
	}

	return ResultString;
}

void FString::AppendfImpl(FString& AppendToMe, const TCHAR* Fmt, ...)
{
	int32		BufferSize = STARTING_BUFFER_SIZE;
	TCHAR		StartingBuffer[STARTING_BUFFER_SIZE];
	TCHAR*		Buffer = StartingBuffer;
	int32		Result = -1;

	// First try to print to a stack allocated location 
	GET_VARARGS_RESULT(Buffer, BufferSize, BufferSize - 1, Fmt, Fmt, Result);

	// If that fails, start allocating regular memory
	if (Result == -1)
	{
		Buffer = nullptr;
		while (Result == -1)
		{
			BufferSize *= 2;
			Buffer = (TCHAR*)FMemory::Realloc(Buffer, BufferSize * sizeof(TCHAR));
			GET_VARARGS_RESULT(Buffer, BufferSize, BufferSize - 1, Fmt, Fmt, Result);
		};
	}

	Buffer[Result] = 0;

	AppendToMe += Buffer;

	if (BufferSize != STARTING_BUFFER_SIZE)
	{
		FMemory::Free(Buffer);
	}
}

int32 VARARGS FString::SprintfImpl(TCHAR* Dest, const TCHAR* Fmt, ...)
{
	int32 Result = -1;
	GET_VARARGS_RESULT(Dest, MAX_SPRINTF, MAX_SPRINTF - 1, Fmt, Fmt, Result);
	return Result;
}

int32 VARARGS FString::SnprintfImpl(TCHAR* Dest, int32 DestSize, const TCHAR* Fmt, ...)
{
	int32 Result = -1;
	GET_VARARGS_RESULT(Dest, DestSize, DestSize - 1, Fmt, Fmt, Result);
	return Result;
}






int32 FString::Find(const TCHAR* SubStr, ESearchCase SearchCase, ESearchDir SearchDir, int32 StartPosition) const
{
	if (SubStr == nullptr)
	{
		return -1;
	}


	if (SearchDir == ESearchDir::FromStart)
	{
		const TCHAR* Start = (*this).GetStr();
		if (StartPosition != -1)
		{
			Start += FMath::Clamp(StartPosition, 0, (int32)Length() - 1);
		}

		const TCHAR* Tmp = SearchCase == ESearchCase::IgnoreCase ? Stristr(Start, SubStr) : FMicrosoftPlatformString::Strstr(Start, SubStr);

		return Tmp ? PTRDIFF_TO_INT32(Tmp - (*this).GetStr()) : -1;
	}
	else
	{
		// if ignoring, do a onetime ToUpper on both strings, to avoid ToUppering multiple
		// times in the loop below
		if (SearchCase == ESearchCase::IgnoreCase)
		{
			return ToUpper().Find(FString(SubStr).ToUpper(), ESearchCase::CaseSensitive, SearchDir, StartPosition);
		}
		else
		{
			const int32 SearchStringLength = FMath::Max(1, FMicrosoftPlatformString::Strlen(SubStr));

			if (StartPosition == -1 || StartPosition >= (int32)Length())
			{
				StartPosition = Length();
			}

			for (int32 i = StartPosition - SearchStringLength; i >= 0; --i)
			{
				int32 j;
				for (j = 0; SubStr[j]; ++j)
				{
					if ((*this)[i + j] != SubStr[j])
					{
						break;
					}
				}

				if (!SubStr[j])
				{
					return i;
				}
			}

			return -1;
		}
	}
}









bool FString::IsNumeric() const
{
	if (IsEmpty())
	{
		return false;
	}

	TCHAR* LStrPointer = GetBuffer();


	if (*LStrPointer == '-' || *LStrPointer == '+')
	{
		++LStrPointer;
	}

	bool bHasDot = false;
	while (*LStrPointer != '\0')
	{
		if (*LStrPointer == '.')
		{
			if (bHasDot)
			{
				return false;
			}
			bHasDot = true;
		}
		else if (!FChar::IsDigit(*LStrPointer))
		{
			return false;
		}

		++LStrPointer;
	}


	return true;
}








FString FString::FormatAsNumber(int32 InNumber)
{
	FString Number = FString::FromInt(InNumber);
	FString Result;

	int32 dec = 0;
	for (int32 x = Number.Length() - 1; x > -1; --x)
	{
		Result += Number.Mid(x, 1);

		++dec;
		if (dec == 3 && x > 0)
		{
			Result += TEXT(",");
			dec = 0;
		}
	}

	return Result.Reverse();
}


FString FString::SanitizeFloat(double InFloat, const int32 InMinFractionalDigits)
{
	// Avoids negative zero
	if (InFloat == 0)
	{
		InFloat = 0;
	}

	// First create the string
	FString TempString = FString::Printf(TEXT("%f"), InFloat);
	if (!TempString.IsNumeric())
	{
		// String did not format as a valid decimal number so avoid messing with it
		return TempString;
	}


	// Trim all trailing zeros (up-to and including the decimal separator) from the fractional part of the number
	int32 TrimIndex = -1;
	int32 DecimalSeparatorIndex = -1;
	for (int32 CharIndex = TempString.Length() - 1; CharIndex >= 0; --CharIndex)
	{
		const TCHAR Char = TempString[CharIndex];
		if (Char == TEXT('.'))
		{
			DecimalSeparatorIndex = CharIndex;
			TrimIndex = FMath::Max(TrimIndex, DecimalSeparatorIndex);
			break;
		}
		if (TrimIndex == -1 && Char != TEXT('0'))
		{
			TrimIndex = CharIndex + 1;
		}
	}

	TempString.RemoveAt(TrimIndex, TempString.Length() - TrimIndex);

	// Pad the number back to the minimum number of fractional digits
	if (InMinFractionalDigits > 0)
	{
		if (TrimIndex == DecimalSeparatorIndex)
		{
			// Re-add the decimal separator
			TempString += TEXT('.');
		}

		const int32 NumFractionalDigits = (TempString.Length() - DecimalSeparatorIndex) - 1;
		const int32 FractionalDigitsToPad = InMinFractionalDigits - NumFractionalDigits;
		if (FractionalDigitsToPad > 0)
		{
			for (int32 Cx = 0; Cx < FractionalDigitsToPad; ++Cx)
			{
				TempString += TEXT('0');
			}
		}
	}


	return TempString;
}
#endif
