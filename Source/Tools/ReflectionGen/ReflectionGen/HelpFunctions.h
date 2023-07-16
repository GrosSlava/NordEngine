
#pragma once

#include "ReflectionGenCore.h"
#include <iostream>
#include <algorithm>




//.........................................................Errors...............................................................//

/**
	Print error message and exit program.
*/
inline void RaiseError(const std::string& Msg)
{
	std::cerr << "CppReflectionGen ERROR: " << Msg << std::endl;
	exit(EXIT_FAILURE);
}
/**
	Print warning message and continue program.
*/
inline void RaiseWarning(const std::string& Msg)
{
	std::cerr << "CppReflectionGen WARNING: " << Msg << std::endl;
}

//..............................................................................................................................//


//.........................................................String...............................................................//

/**
	Make new copy of string with upper case chars.
*/
inline std::string StrUpper(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
	return s;
}
/**
	Make new copy of string with C-correct chars.
*/
inline std::string StrCorrect(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return (c == ':' || c == '.' || c == '-') ? '_' : c; });
	return s;
}

//..............................................................................................................................//

//...........................................................Vector.............................................................//

/**
	Append vector B to vector A.
*/
template<typename T>
inline void AppendVector(std::vector<T>& A, const std::vector<T>& B)
{
	A.reserve(A.size() + B.size());
	A.insert(A.end(), B.begin(), B.end());
}

//..............................................................................................................................//

//.............................................................Path.............................................................//

#if _WIN32 || _WIN64
#define PREFERED_SEPARATOR '\\'
#elif __linux__ || linux
#define PREFERED_SEPARATOR '/'
#endif

/**
	Concat paths.
	@return new string path.
*/
inline std::string CatPaths(const std::string& Lhs, const std::string& Rhs, char Separator = PREFERED_SEPARATOR)
{
	if( Lhs.empty() ) return Rhs;
	if( Rhs.empty() ) return Lhs;

	std::string LResult = Lhs;
	if( LResult.back() != Separator && Rhs.at(0) != Separator )
	{
		LResult += Separator;
	}
	else if( LResult.back() == Separator && Rhs.at(0) == Separator )
	{
		LResult.resize(LResult.size() - 1);
	}
	LResult += Rhs;

	return LResult;
}
/**
	Spilt file path to its components.
*/
inline void SplitFilePath(const std::string& FilePath, std::string& OutPathOnly, std::string& OutNameOnly, std::string& OutExtensionOnly)
{
	size_t PathEnd = std::string::npos;
	size_t NameEnd = std::string::npos;

	for( size_t i = 0; i < FilePath.size(); ++i )
	{
		const char C = FilePath[i];
		if( C == ':' || C == '/' || C == '\\' )
		{
			PathEnd = i;
			NameEnd = std::string::npos;
		}
		else if( C == '.' )
		{
			NameEnd = i;
		}
	}
	if( PathEnd != std::string::npos )
	{
		OutPathOnly = FilePath.substr(0, PathEnd); // include last separator
	}
	if( NameEnd != std::string::npos )
	{
		OutNameOnly = FilePath.substr(PathEnd + 1, NameEnd - PathEnd - 1);
		OutExtensionOnly = FilePath.substr(NameEnd + 1);
	}
}
/**
	Extract file name with extension from path.
*/
inline std::string GetFileNameWithExtension(const std::string& FilePath)
{
	size_t PathEnd = std::string::npos;

	for( size_t i = 0; i < FilePath.size(); ++i )
	{
		const char C = FilePath[i];
		if( C == ':' || C == '/' || C == '\\' )
		{
			PathEnd = i;
		}
	}

	if( PathEnd != std::string::npos )
	{
		return FilePath.substr(PathEnd + 1);
	}
	else
	{
		return "";
	}
}
/**
	Extract only file name from path.
*/
inline std::string GetFileNameOnly(const std::string& FilePath)
{
	size_t PathEnd = std::string::npos;
	size_t NameEnd = std::string::npos;

	for( size_t i = 0; i < FilePath.size(); ++i )
	{
		const char C = FilePath[i];
		if( C == ':' || C == '/' || C == '\\' )
		{
			PathEnd = i;
			NameEnd = std::string::npos;
		}
		else if( C == '.' )
		{
			NameEnd = i;
		}
	}
	if( NameEnd != std::string::npos )
	{
		return FilePath.substr(PathEnd + 1, NameEnd - PathEnd - 1);
	}
	else
	{
		return "";
	}
}

//..............................................................................................................................//

//..............................................................Hash............................................................//

/**
	Polinimial string hash.
	Should be same with Framework/NameHash.h
*/
inline unsigned long long int StrNameHash(const char* S)
{
	unsigned long long int LResult = 0;
	unsigned long long int P = 1;

	while( *S != '\0' )
	{
		LResult += *S * P;
		P *= 53;
		++S;
	}

	return LResult;
}
inline unsigned long long int StrNameHash(const std::string& S)
{
	return StrNameHash(S.c_str());
}

//..............................................................................................................................//