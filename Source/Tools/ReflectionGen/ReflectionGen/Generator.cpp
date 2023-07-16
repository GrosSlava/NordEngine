
#include "Generator.h"
#include "HelpFunctions.h"

#include <fstream>





std::string MakeEnumRowName(const FEnumInfo& EnumInfo, const FEnumRowInfo& RowInfo)
{
	if( EnumInfo.IsScoped )
	{
		return EnumInfo.EnumName + "::" + RowInfo.Name;
	}
	else
	{
		return RowInfo.Name;
	}
}




void PrintClassGeneratedBody(std::ofstream& outH, std::ofstream& outCpp, const FClassInfo& ClassInfo)
{
	outH << "#define " << StrUpper(StrCorrect(ClassInfo.ClassName)) << "_GENERATED_BODY()\\\n";

	outH << "\tprivate:\\\n";
	outH << "\t\tusing Self = " << ClassInfo.ClassName << ";\\\n";
	if( !ClassInfo.ParentName.empty() )
	{
		outH << "\t\tusing Super = " << ClassInfo.ParentName[0] << ";\\\n";
	}

	outH << "\tpublic:\\\n";
	outH << "\t\tstatic constexpr const char* StaticClassName = \"" << ClassInfo.ClassName << "\";\\\n";
	if( !ClassInfo.ParentName.empty() )
	{
		outH << "\t\tvirtual const char* GetClassName() const override { return StaticClassName; }\\\n";
	}
	else
	{
		outH << "\t\tvirtual const char* GetClassName() const { return StaticClassName; }\\\n";
	}
	outH << "\t\tstatic TClass<Self> StaticClass;\\\n";

	if( !ClassInfo.ParentName.empty() )
	{
		outH << "\t\tvirtual PropertyHandler GetField(const char* Name) const override;\\\n";
	}
	else
	{
		outH << "\t\tvirtual PropertyHandler GetField(const char* Name) const;\\\n";
	}
	outH << "\t\tbool HasField(const char* Name) const { return GetField(Name).IsValid(); };\\\n";

	if( !ClassInfo.ParentName.empty() )
	{
		outH << "\t\tvirtual bool CallMethod(const char* Name, TFunction& Function) override;\\\n";
	}
	else
	{
		outH << "\t\tvirtual bool CallMethod(const char* Name, TFunction& Function);\\\n";
	}

	outH << "\tprivate:\\\n";
	outH << "\tusing __FakeAlias = void\n"; // force to have ';' at the end of macro usage



	outCpp << "PropertyHandler " << ClassInfo.ClassName << "::GetField(const char* Name) const\n";
	outCpp << "{\n";
	if( !ClassInfo.ParentName.empty() )
	{
		outCpp << "\tPropertyHandler LParentResult = Super::GetField(Name);\n";
		outCpp << "\tif( LParentResult.IsValid() ) return LParentResult;\n";
	}
	if( !ClassInfo.Fields.empty() )
	{
		outCpp << "\tunsigned long long int LHashValue = StrNameHash(Name);\n";
		outCpp << "\tswitch(LHashValue)\n";
		outCpp << "\t{\n";
		for( const FFieldInfo& LFiledInfo : ClassInfo.Fields )
		{
			outCpp << "\tcase " << StrNameHash(LFiledInfo.Name) << ": { return PropertyHandler(&" << LFiledInfo.Name << "); }\n";
		}
		outCpp << "\t}\n";
	}
	outCpp << "\treturn PropertyHandler();\n";
	outCpp << "}\n";

	outCpp << "bool " << ClassInfo.ClassName << "::CallMethod(const char* Name, TFunction& Function)\n";
	outCpp << "{\n";
	if( !ClassInfo.ParentName.empty() )
	{
		outCpp << "\tbool ParentSuccess = Super::CallMethod(Name, Function);\n";
		outCpp << "\tif( ParentSuccess ) return true;\n";
	}
	if( !ClassInfo.Methods.empty() )
	{
		outCpp << "\tconst unsigned long long int LHashValue = StrNameHash(Name);\n";
		outCpp << "\tswitch(LHashValue)\n";
		outCpp << "\t{\n";
		for( const FMethodInfo& LMethodInfo : ClassInfo.Methods )
		{
			outCpp << "\tcase " << StrNameHash(LMethodInfo.Name) << ":\n";
			outCpp << "\t{\n";
			outCpp << "\t\tif( !Function.CheckSignature<";
			outCpp << LMethodInfo.ResultTypeName;
			for( const std::string& LArgTypeName : LMethodInfo.ArgsTypesName )
			{
				outCpp << ", " << LArgTypeName;
			}
			outCpp << ">() ) return false;\n";
			if( LMethodInfo.ResultTypeName != "void" )
			{
				outCpp << "\t\t*(Function.GetReturn()->GetAsPtr<" << LMethodInfo.ResultTypeName << ">()) = " << LMethodInfo.Name << "(";
			}
			else
			{
				outCpp << "\t\t" << LMethodInfo.Name << "(";
			}
			for( size_t i = 0; i < LMethodInfo.ArgsTypesName.size(); ++i )
			{
				outCpp << "Function.GetArg(" << i << ")->GetAs<" << LMethodInfo.ArgsTypesName[i] << ">()";
				if( i + 1 < LMethodInfo.ArgsTypesName.size() )
				{
					outCpp << ", ";
				}
			}
			outCpp << ");\n";
			outCpp << "\t\treturn true;\n";
			outCpp << "\t}\n";
		}
		outCpp << "\t}\n";
	}
	outCpp << "\treturn false;\n";
	outCpp << "}\n";
}

void PrintClassGeneratedTables(std::ofstream& outH, std::ofstream& outCpp, const FClassInfo& ClassInfo)
{
	outH << "#define " << StrUpper(StrCorrect(ClassInfo.ClassName)) << "_GENERATED_TABLES()\\\n";

	outH << "\ttemplate<>\\\n";
	if( ClassInfo.ParentName.empty() )
	{
		outH << "\tclass TClassBuilder<" << ClassInfo.ClassName << ">\\\n";
	}
	else
	{
		outH << "\tclass TClassBuilder<" << ClassInfo.ClassName << "> : public TClassBuilder<" << ClassInfo.ParentName[0] << ">\\\n";
	}
	outH << "\t{\\\n";
	outH << "\t\tCLASS_BUILDER_BODY(TClassBuilder<" << ClassInfo.ClassName << ">)\\\n";
	outH << "\tpublic:\\\n";
	if( ClassInfo.ParentName.empty() )
	{
		outH << "\t\tvirtual void* Create() const { return new " << ClassInfo.ClassName << "(); }\\\n";
		outH << "\t\tvirtual TPtr<void> CreatePtr() const { return TPtr<" << ClassInfo.ClassName << ">().Create(); }\\\n";
	}
	else
	{
		outH << "\t\tvirtual void* Create() const override { return new " << ClassInfo.ClassName << "(); }\\\n";
		outH << "\t\tvirtual TPtr<void> CreatePtr() const override { return TPtr<" << ClassInfo.ClassName << ">().Create(); }\\\n";
	}
	outH << "\t};\n\n";
}

void PrintStructGeneratedBody(std::ofstream& outH, std::ofstream& outCpp, const FStructInfo& StructInfo)
{
	outH << "#define " << StrUpper(StrCorrect(StructInfo.StructName)) << "_GENERATED_BODY()\\\n";

	outH << "\tprivate:\\\n";
	outH << "\t\tusing Self = " << StructInfo.StructName << ";\\\n";

	outH << "\tpublic:\\\n";
	outH << "\t\tstatic constexpr const char* StaticStructName = \"" << StructInfo.StructName << "\";\\\n";

	outH << "\t\tPropertyHandler GetField(const char* Name) const;\\\n";
	outH << "\t\tbool HasField(const char* Name) const { return GetField(Name).IsValid(); };\\\n";

	outH << "\tpublic:\\\n";
	outH << "\tusing __FakeAlias = void\n"; // force to have ';' at the end of macro usage



	outCpp << "PropertyHandler " << StructInfo.StructName << "::GetField(const char* Name) const\n";
	outCpp << "{\n";
	if( !StructInfo.Fields.empty() )
	{
		outCpp << "\tconst unsigned long long int LHashValue = StrNameHash(Name);\n";
		outCpp << "\tswitch(LHashValue)\n";
		outCpp << "\t{\n";
		for( const FFieldInfo& LFiledInfo : StructInfo.Fields )
		{
			outCpp << "\tcase " << StrNameHash(LFiledInfo.Name) << ": { return PropertyHandler(&" << LFiledInfo.Name << "); }\n";
		}
		outCpp << "\t}\n";
	}
	outCpp << "\treturn PropertyHandler();\n";
	outCpp << "}\n";
}

void PrintEnumGeneratedTables(std::ofstream& outH, std::ofstream& outCpp, const FEnumInfo& EnumInfo)
{
	outH << "#define " << StrUpper(StrCorrect(EnumInfo.EnumName)) << "_GENERATED_TABLES()\\\n";

	outH << "\tnamespace " << EnumInfo.EnumName << "_Context\\\n";
	outH << "\t{\\\n";
	outH << "\tconst char* GetValueName(" << EnumInfo.EnumName << " V);\\\n";
	outH << "\t" << EnumInfo.EnumName << " GetValueByName(const char* Name);\\\n";
	outH << "\tconst " << EnumInfo.TypeName << "* GetValuesArray();\\\n";
	outH << "\tinline unsigned int ElemsCount() { return " << EnumInfo.Rows.size() << "; }\\\n";
	outH << "\tinline " << EnumInfo.TypeName << " AsInt(" << EnumInfo.EnumName << " V) { return static_cast<" << EnumInfo.TypeName << ">(V); }\\\n";
	if( EnumInfo.GetMaxValueIndex() != -1 )
	{
		outH << "\tinline " << EnumInfo.EnumName << " MaxValue() { return " << MakeEnumRowName(EnumInfo, EnumInfo.Rows[EnumInfo.GetMaxValueIndex()])
			 << "; }\\\n";
	}
	if( EnumInfo.GetMinValueIndex() != -1 )
	{
		outH << "\tinline " << EnumInfo.EnumName << " MinValue() { return " << MakeEnumRowName(EnumInfo, EnumInfo.Rows[EnumInfo.GetMinValueIndex()])
			 << "; }\\\n";
	}
	outH << "\t}\n";



	outCpp << "const char* " << EnumInfo.EnumName << "_Context::GetValueName(" << EnumInfo.EnumName << " V)\n";
	outCpp << "{\n";
	if( !EnumInfo.Rows.empty() )
	{
		outCpp << "\tswitch(V)\n";
		outCpp << "\t{\n";
		for( const FEnumRowInfo& LRow : EnumInfo.Rows )
		{
			outCpp << "\tcase " << MakeEnumRowName(EnumInfo, LRow) << ": { return \"" << LRow.Name << "\"; }\n";
		}
		outCpp << "\t}\n";
	}
	outCpp << "\treturn nullptr;\n";
	outCpp << "}\n";

	outCpp << EnumInfo.EnumName << " " << EnumInfo.EnumName << "_Context::GetValueByName(const char* Name)\n";
	outCpp << "{\n";
	if( !EnumInfo.Rows.empty() )
	{
		outCpp << "\tconst unsigned long long int LHashValue = StrNameHash(Name);\n";
		outCpp << "\tswitch(LHashValue)\n";
		outCpp << "\t{\n";
		for( const FEnumRowInfo& LRow : EnumInfo.Rows )
		{
			outCpp << "\tcase " << StrNameHash(LRow.Name) << ": { return " << MakeEnumRowName(EnumInfo, LRow) << "; }\n";
		}
		outCpp << "\t}\n";
	}
	outCpp << "\treturn " << EnumInfo.EnumName << "();\n";
	outCpp << "}\n";

	outCpp << "const " << EnumInfo.TypeName << "* " << EnumInfo.EnumName << "_Context::GetValuesArray()\n";
	outCpp << "{\n";
	if( !EnumInfo.Rows.empty() )
	{
		outCpp << "\tstatic const " << EnumInfo.TypeName << " EnumLinearTable[] = { ";
		std::string Sep = "";
		for( const FEnumRowInfo& LRow : EnumInfo.Rows )
		{
			outCpp << Sep << LRow.Value;
			Sep = ", ";
		}
		outCpp << " };\n";
		outCpp << "\treturn EnumLinearTable;\n";
	}
	else
	{
		outCpp << "\treturn nullptr;\n";
	}
	outCpp << "}\n";
}



void GenerateReflection(const FReflectionGenInfo& Info, const std::string& SourceFile, const std::string& OutHFilePath, const std::string& OutCppFilePath)
{
	std::ofstream OutHFile(OutHFilePath);
	if( !OutHFile.is_open() )
	{
		RaiseError("Can't create file '" + OutHFilePath + "'");
	}

	std::ofstream OutCppFile(OutCppFilePath);
	if( !OutCppFile.is_open() )
	{
		RaiseError("Can't create file '" + OutCppFilePath + "'");
	}

	const std::string LGuardName = StrUpper(StrCorrect(GetFileNameOnly(OutHFilePath))) + "_H";
	OutHFile << "// Generated by CppReflectionGen. Do not edit!\n";
	OutHFile << "#ifdef " << LGuardName << "\n";
	OutHFile << "#error \"Detected CppReflectionGen file double include: " << GetFileNameWithExtension(OutHFilePath) << "\"\n";
	OutHFile << "#endif\n";
	OutHFile << "#define " << LGuardName << "\n";
	OutHFile << "\n";

	OutHFile << "#include \"Type.h\"\n";
	OutHFile << "#include \"Ptr.h\"\n";
	OutHFile << "#include \"Class.h\"\n";
	OutHFile << "#include \"Property.h\"\n";
	OutHFile << "#include \"Function.h\"\n";
	OutHFile << "\n\n\n";

	OutCppFile << "// Generated by CppReflectionGen. Do not edit!\n";
	OutCppFile << "#include \"" << SourceFile << "\"\n";
	OutCppFile << "#include \"NameHash.h\"\n";
	OutCppFile << "#ifdef " << LGuardName << "\n";
	OutCppFile << "\n\n\n\n";

	for( const auto& LClassInfoPair : Info.ClassesInfo )
	{
		PrintClassGeneratedBody(OutHFile, OutCppFile, LClassInfoPair.second);
		PrintClassGeneratedTables(OutHFile, OutCppFile, LClassInfoPair.second);
	}
	for( const auto& LStructInfoPair : Info.StructsInfo )
	{
		PrintStructGeneratedBody(OutHFile, OutCppFile, LStructInfoPair.second);
	}
	for( const auto& LEnumInfoPair : Info.EnumsInfo )
	{
		PrintEnumGeneratedTables(OutHFile, OutCppFile, LEnumInfoPair.second);
	}

	OutCppFile << "#endif\n";
}
