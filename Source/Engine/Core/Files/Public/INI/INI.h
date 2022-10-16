
#pragma once

#include "GenericPlatform.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>




/*
	Read an INI file into easy-to-access name/value pairs.
*/
struct ENGINE_API FINIFile
{
public:

	FINIFile() {};
	FINIFile(const std::string& FileName);
	FINIFile(FILE* File);



public:

	FORCEINLINE std::vector<std::string> GetSections() const 
	{
		std::vector<std::string> LResult;
		LResult.reserve(Values.size());

		for( const auto& element : Values )
		{
			LResult.push_back(element.first);
		}

		return LResult;
	}
	FORCEINLINE std::unordered_map<std::string, std::string> GetSection(const std::string& Section) const
	{
		const auto LSection = Values.find(Section);

		if( LSection == Values.end() )
		{
			return std::unordered_map<std::string, std::string>();
		}

		return LSection->second;
	}
	FORCEINLINE std::vector<std::string> GetKeys(const std::string& Section) const 
	{
		const std::unordered_map<std::string, std::string> LSection = GetSection(Section);

		std::vector<std::string> LResult;
		LResult.reserve(LSection.size());

		for( const auto& element : LSection )
		{
			LResult.push_back(element.first);
		}

		return LResult;
	}


	template<typename T>
	FORCEINLINE T Get(const std::string& Section, const std::string& Name, const T& DefaultValue = T()) const
	{
		const auto LSection = GetSection(Section);
		const auto LValue = LSection.find(Name);

		if( LValue == LSection.end() )
		{
			return DefaultValue;
		}

		return Value2T<T>(LValue->second);
	}
	template<typename T>
	FORCEINLINE std::vector<T> GetVector(const std::string& Section, const std::string& Name) const
	{
		std::string LValue = Get<std::string>(Section, Name);
		if( LValue.empty() ) return std::vector<T>();


		std::istringstream out(LValue);
		const std::vector<std::string> strs {std::istream_iterator<std::string>(out)};

		std::vector<T> LResult;
		for( const std::string& s : strs )
		{
			LResult.emplace_back(Value2T<T>(s));
		}

		return LResult;
	}

	template<typename T>
	FORCEINLINE void Set(const std::string& Section, const std::string& Name, const T& Value)
	{
		Values[Section][Name] = Value2String(Value);
	}
	template<typename T>
	FORCEINLINE void Set(const std::string& Section, const std::string& Name, const std::vector<T>& Value)
	{
		Values[Section][Name] = Vector2String(Value);
	}

protected:

	int ParseIni(const std::string& FileName);
	int ParseIniFile(FILE* File);


	template<typename T>
	FORCEINLINE std::string Value2String(const T& V) const
	{
		std::stringstream SS;
		SS << V;
		return SS.str();
	}
	template<typename T>
	FORCEINLINE std::string Vector2String(const std::vector<T>& V) const
	{
		if( V.empty() ) return "";

		std::ostringstream oss;
		std::copy(V.begin(), V.end() - 1, std::ostream_iterator<T>(oss, " "));
		oss << V.back();

		return oss.str();
	}

	template<typename T>
	FORCEINLINE T Value2T(std::string S) const
	{
		T LResult;

		std::istringstream _ {S};
		_.exceptions(std::ios::failbit);
		_ >> LResult;

		return LResult;
	}
	template<>
	FORCEINLINE std::string Value2T(std::string S) const
	{
		return S;
	}
	template<>
	FORCEINLINE bool Value2T<bool>(std::string S) const
	{
		std::transform(S.begin(), S.end(), S.begin(), [](unsigned char c) { return std::tolower(c); } );
		
		static const std::unordered_map<std::string, bool> s2b 
		{
			{"1", true},
			{"true", true},
			{"yes", true},
			{"on", true},
			{"0", false},
			{"false", false},
			{"no", false},
			{"off", false},
		};

		const auto LValue = s2b.find(S);
		if( LValue == s2b.end() )
		{
			return false;
		}

		return LValue->second;
	}



protected:

	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> Values;
};



struct ENGINE_API FINIWriter
{
public:

	FINIWriter() { }



public:

	FORCEINLINE static void Write(const std::string& FilePath, const FINIFile& IniFile)
	{
		std::ofstream out;
		out.open(FilePath);
		if( !out.is_open() )
		{
			return;
		}

		for( const auto& LSection : IniFile.GetSections() )
		{
			out << "[" << LSection << "]\n";

			for( const auto& LKey : IniFile.GetKeys(LSection) )
			{
				out << LKey << "=" << IniFile.Get<std::string>(LSection, LKey) << "\n";
			}
		}

		out.close();
	}
};