// Copyright Nord Engine. All Rights Reserved.
#include "FString.h"

#include "AssertionMacros.h"





int32 FString::ParseIntoArray(TArray<FString>& OutArray, const TCHAR* pchDelim, bool InCullEmpty) const noexcept
{
	// Make sure the delimit string is not null or empty
	check(pchDelim);

	OutArray.Reset();

	const TCHAR* Start = GetStr();
	const int32 DelimLength = FPlatformString::Strlen(pchDelim);
	if( Start && *Start != TEXT('\0') && DelimLength )
	{
		while( const TCHAR* At = FPlatformString::Strstr(Start, pchDelim) )
		{
			if( !InCullEmpty || At - Start )
			{
				OutArray.Add(FString(Start, static_cast<uint32>(At - Start)));
			}

			Start = At + DelimLength;
		}

		if( !InCullEmpty || *Start )
		{
			OutArray.PushBack(FString(Start));
		}
	}

	return OutArray.Num();
}

void FString::ReverseString() noexcept
{
	if( IsEmpty() ) return;

	TCHAR* StartChar = StringBuffer.GetAtByPointer(0);
	TCHAR* EndChar = StringBuffer.GetAtByPointer(Length() - 1);
	TCHAR TempChar;
	do
	{
		TempChar = *StartChar; // store the current value of StartChar
		*StartChar = *EndChar; // change the value of StartChar to the value of EndChar
		*EndChar = TempChar;   // change the value of EndChar to the character that was previously at StartChar

		++StartChar;
		--EndChar;

	} while( StartChar < EndChar ); // repeat until we've reached the midpoint of the string
}

int32 FString::ReplaceInline(const TCHAR* SearchText, const TCHAR* ReplacementText, ESearchCase SearchCase) noexcept
{
	//TODO

	return 0;
}

static const TCHAR* CharToEscapeSeqMap[][2] = {
	// Always replace \\ first to avoid double-escaping characters
	{TEXT("\\"), TEXT("\\\\")}, {TEXT("\n"), TEXT("\\n")}, {TEXT("\r"), TEXT("\\r")}, {TEXT("\t"), TEXT("\\t")}, {TEXT("\'"), TEXT("\\'")}, {TEXT("\""), TEXT("\\\"")}};

static const uint32 MaxSupportedEscapeChars = 6;

FString FString::ReplaceCharWithEscapedChar(const TArray<TCHAR>* Chars) && noexcept
{
	if( Length() > 0 && (Chars == nullptr || Chars->Num() > 0) )
	{
		FString Result(*this);
		for( int32 ChIdx = 0; ChIdx < MaxSupportedEscapeChars; ++ChIdx )
		{
			if( Chars == nullptr || Chars->Contains(*(CharToEscapeSeqMap[ChIdx][0])) )
			{
				// use ReplaceInline as that won't create a copy of the string if the character isn't found
				Result.ReplaceInline(CharToEscapeSeqMap[ChIdx][0], CharToEscapeSeqMap[ChIdx][1]);
			}
		}
		return Result;
	}

	return *this;
}

FString FString::ReplaceEscapedCharWithChar(const TArray<TCHAR>* Chars) && noexcept
{
	if( Length() > 0 && (Chars == nullptr || Chars->Num() > 0) )
	{
		// Spin CharToEscapeSeqMap backwards to ensure we're doing the inverse of ReplaceCharWithEscapedChar
		for( int32 ChIdx = MaxSupportedEscapeChars - 1; ChIdx >= 0; --ChIdx )
		{
			if( Chars == nullptr || Chars->Contains(*(CharToEscapeSeqMap[ChIdx][0])) )
			{
				// use ReplaceInline as that won't create a copy of the string if the character isn't found
				ReplaceInline(CharToEscapeSeqMap[ChIdx][1], CharToEscapeSeqMap[ChIdx][0]);
			}
		}
	}

	return MoveTemp(*this);
}

void FString::ConvertTabsToSpacesInline(const uint32 SpacesPerTab) noexcept
{
	//must call this with at least 1 space so the modulus operation works
	if( SpacesPerTab == 0 ) return;

	FString Result;
	Result.Reserve(Length());

	TCHAR* p = begin();
	while( p != end() )
	{
		if( *p == TEXT('\t') )
		{
			Result.AddInitialized(SpacesPerTab, TEXT(' '));
		}
		else
		{
			Result += *p;
		}
	}

	*this = MoveTemp(Result);
}

int32 FString::Find(const TCHAR* SubStr, ESearchCase SearchCase, ESearchDir SearchDir, int32 StartPosition) const noexcept
{
	if( SubStr == nullptr ) return -1;

	if( SearchDir == ESearchDir::FromStart )
	{
		const TCHAR* Start = (*this).GetStr();
		if( StartPosition != -1 )
		{
			Start += FMath::Clamp(StartPosition, 0, (int32)Length() - 1);
		}

		const TCHAR* Tmp = SearchCase == ESearchCase::IgnoreCase ? Stristr(Start, SubStr) : FPlatformString::Strstr(Start, SubStr);

		return Tmp ? static_cast<int32>((Tmp - (*this).GetStr())) : -1;
	}
	else
	{
		// if ignoring, do a onetime ToUpper on both strings, to avoid ToUppering multiple
		// times in the loop below
		if( SearchCase == ESearchCase::IgnoreCase )
		{
			return ToUpper().Find(FString(SubStr).ToUpper(), ESearchCase::CaseSensitive, SearchDir, StartPosition);
		}
		else
		{
			const int32 SearchStringLength = FMath::Max(1, FPlatformString::Strlen(SubStr));

			if( StartPosition == -1 || StartPosition >= (int32)Length() )
			{
				StartPosition = Length();
			}

			for( int32 i = StartPosition - SearchStringLength; i >= 0; --i )
			{
				int32 j;
				for( j = 0; SubStr[j]; ++j )
				{
					if( (*this)[i + j] != SubStr[j] )
					{
						break;
					}
				}

				if( !SubStr[j] )
				{
					return i;
				}
			}

			return -1;
		}
	}
}

void FString::AppendInt(int32 Num) noexcept
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
	} while( Num );

	if( bIsNumberNegative )
	{
		TempNum[--TempAt] = TEXT('-');
	}

	const TCHAR* CharPtr = TempNum + TempAt;
	const int32 NumChars = TempBufferSize - TempAt;

	*this += FString(CharPtr);
}

bool FString::IsNumeric() const noexcept
{
	if( IsEmpty() )
	{
		return false;
	}

	TCHAR* LStrPointer = GetBuffer();
	if( *LStrPointer == '-' || *LStrPointer == '+' )
	{
		++LStrPointer;
	}

	bool bHasDot = false;
	while( *LStrPointer != '\0' )
	{
		if( *LStrPointer == '.' )
		{
			if( bHasDot )
			{
				return false;
			}
			bHasDot = true;
		}
		else if( !FChar::IsDigit(*LStrPointer) )
		{
			return false;
		}

		++LStrPointer;
	}

	return true;
}





FString FString::FormatAsNumber(int32 InNumber) noexcept
{
	FString Number = FString::FromInt(InNumber);
	FString Result;

	int32 dec = 0;
	for( int32 x = Number.Length() - 1; x > -1; --x )
	{
		Result += Number.Mid(x, 1);

		++dec;
		if( dec == 3 && x > 0 )
		{
			Result += TEXT(",");
			dec = 0;
		}
	}

	return Result.Reverse();
}

FString FString::SanitizeFloat(double InFloat, const int32 InMinFractionalDigits) noexcept
{
	// Avoids negative zero
	if( InFloat == 0 )
	{
		InFloat = 0;
	}

	// First create the string
	TCHAR Buff[256];
	FPlatformString::Printf(Buff, 256, TEXT("%f"), InFloat);

	FString TempString(Buff);
	if( !TempString.IsNumeric() )
	{
		// String did not format as a valid decimal number so avoid messing with it
		return TempString;
	}


	// Trim all trailing zeros (up-to and including the decimal separator) from the fractional part of the number
	int32 TrimIndex = -1;
	int32 DecimalSeparatorIndex = -1;
	for( int32 CharIndex = TempString.Length() - 1; CharIndex >= 0; --CharIndex )
	{
		const TCHAR Char = TempString[CharIndex];
		if( Char == TEXT('.') )
		{
			DecimalSeparatorIndex = CharIndex;
			TrimIndex = FMath::Max(TrimIndex, DecimalSeparatorIndex);
			break;
		}
		if( TrimIndex == -1 && Char != TEXT('0') )
		{
			TrimIndex = CharIndex + 1;
		}
	}

	TempString.RemoveAt(TrimIndex, TempString.Length() - TrimIndex);

	// Pad the number back to the minimum number of fractional digits
	if( InMinFractionalDigits > 0 )
	{
		if( TrimIndex == DecimalSeparatorIndex )
		{
			// Re-add the decimal separator
			TempString += TEXT('.');
		}

		const int32 NumFractionalDigits = (TempString.Length() - DecimalSeparatorIndex) - 1;
		const int32 FractionalDigitsToPad = InMinFractionalDigits - NumFractionalDigits;
		if( FractionalDigitsToPad > 0 )
		{
			for( int32 Cx = 0; Cx < FractionalDigitsToPad; ++Cx )
			{
				TempString += TEXT('0');
			}
		}
	}

	return TempString;
}
