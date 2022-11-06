#pragma once

#include "GenericPlatform.h"
#include "GenericPlatformString.h"


class FStringBuffer
{

public:

	FORCEINLINE ~FStringBuffer() noexcept
	{
		Reset();
	}



public:

	FORCEINLINE TCHAR& operator[](uint32 Index)
	{
		return str[Index];
	}

	FORCEINLINE TCHAR operator[](uint32 Index) const
	{
		return str[Index];
	}


public:

	FORCEINLINE TCHAR* begin() const noexcept { return str; }
	FORCEINLINE TCHAR* end() const noexcept { return &str[StringLength]; }




public:

	FORCEINLINE TCHAR* GetAtByPointer(uint32 Index) const noexcept
	{
		return &str[Index];
	}


	void Resize(uint32 NewStringLength)
	{
		if (StringLength == NewStringLength) return;


		StringLength = NewStringLength;
		if (StringLength > BufferSize)
		{
			BufferSize = StringLength * 2;

			TCHAR* LBuff = new TCHAR[StringLength + 1];

			if (str != nullptr)
			{
				FMicrosoftPlatformString::Strcpy(LBuff, str);

				delete[] str;
			}

			str = LBuff;
		}

		str[StringLength] = '\0';
	}

	FORCEINLINE void SetBuffer(TCHAR* NewBuffer)
	{
		StringLength = BufferSize = FMicrosoftPlatformString::Strlen(NewBuffer);

		if (str != nullptr)
		{
			delete[] str;
		}

		str = NewBuffer;
	}


	/*
		NOTE: It will delete stored string.
	*/
	FORCEINLINE void Reset()
	{
		if (str != nullptr) delete[] str;

		Clear();
	}
	/*
		NOTE: It will not delete stored string.
	*/
	FORCEINLINE void Clear()
	{
		str = nullptr;
		StringLength = -1;
		BufferSize = -1;
	}



	FORCEINLINE TCHAR* GetBuffer() const noexcept { return str; }
	FORCEINLINE uint32 GetLength() const noexcept {	return StringLength; }
	FORCEINLINE uint32 GetBufferSize() const noexcept { return BufferSize; }



private:

	TCHAR* str = nullptr;
	int32 StringLength = -1;
	int32 BufferSize = -1;

};