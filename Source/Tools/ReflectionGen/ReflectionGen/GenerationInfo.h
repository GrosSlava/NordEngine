
#pragma once

#include "ReflectionGenCore.h"

#include "clang-c/Index.h"




/**
	C++ access cpecifier.
*/
enum class EAccessSpecifier
{
	InvalidAccessSpecifier = 0,
	Public,
	Protected,
	Private
};
/**
	Class/struct field info.
*/
struct FFieldInfo
{
	/**
		Field name.
	*/
	std::string Name;
	/**
		Field access.
	*/
	EAccessSpecifier AccessSpecifier = EAccessSpecifier::InvalidAccessSpecifier;
};
/**
	Enum row info.
*/
struct FEnumRowInfo
{
	/**
		Enum row name.
	*/
	std::string Name;
	/**
		Enum row value.
	*/
	long long int Value = 0;
};
/**
	Class/strcut method info.
*/
struct FMethodInfo
{
	/**
		Method name.
	*/
	std::string Name;
	/*
		Method access.
	*/
	EAccessSpecifier AccessSpecifier = EAccessSpecifier::InvalidAccessSpecifier;
	/**
		Method return type name.
	*/
	std::string ResultTypeName;
	/**
		Array of method arguments names. Order is important.
	*/
	std::vector<std::string> ArgsTypesName;
};



/**
	Info about class and its members.
*/
struct FClassInfo
{
public:

	/**
		@return parent name or empty string.
	*/
	inline std::string GetParentName() const noexcept { return ParentName.empty() ? "" : ParentName[0]; }

public:

	/**
		Fix info structure.
	*/
	void Fixup();



public:

	/**
		Class name with namespace name.
	*/
	std::string ClassName;
	/**
		Parent names. First one is used as parent.
	*/
	std::vector<std::string> ParentName;
	/**
		All implemented interfaces (abstract classes).
	*/
	std::vector<std::string> Interfaces;
	/**
		All class fields.
	*/
	std::vector<FFieldInfo> Fields;
	/**
		All class methods.
	*/
	std::vector<FMethodInfo> Methods;
};
/**
	Info about struct and its members.
*/
struct FStructInfo
{
public:

	/**
		Fix info structure.
	*/
	void Fixup();



public:

	/**
		Struct name with namespace name.
	*/
	std::string StructName;
	/**
		All struct fields.
	*/
	std::vector<FFieldInfo> Fields;
};
/**
	Info about enum and its rows.
*/
struct FEnumInfo
{
public:

	/**
		Fix info structure.
	*/
	void Fixup();

public:

	inline size_t GetMinValueIndex() const noexcept { return MinValueIndex; }
	inline size_t GetMaxValueIndex() const noexcept { return MaxValueIndex; }



public:

	/**
		Enum name with namespace name.
	*/
	std::string EnumName;
	/**
		Enum real type name.
	*/
	std::string TypeName;
	/**
		Is scoped(enum class).
	*/
	bool IsScoped = false;
	/**
		All enum rows.
	*/
	std::vector<FEnumRowInfo> Rows;

private:

	/**
		Enum row min value index.
	*/
	size_t MinValueIndex = -1;
	/**
		Enum row max value index.
	*/
	size_t MaxValueIndex = -1;
};


/**
	Structured info from parsed files.
*/
struct FReflectionGenInfo
{
	/**
		Info about classes only from parsed source file.
	*/
	std::unordered_map<std::string, FClassInfo> ClassesInfo;
	/**
		Info about structs only from parsed source file.
	*/
	std::unordered_map<std::string, FStructInfo> StructsInfo;
	/**
		Info about enums only from parsed source file.
	*/
	std::unordered_map<std::string, FEnumInfo> EnumsInfo;
};
/**
	Main function for forming stuctured info from AST.
*/
void FillGenInfo(FReflectionGenInfo& GenInfo, const CXTranslationUnit& TranslationUnit);