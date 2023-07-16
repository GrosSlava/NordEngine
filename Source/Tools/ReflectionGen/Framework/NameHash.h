// CppReflectionGen framework.
#pragma once




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