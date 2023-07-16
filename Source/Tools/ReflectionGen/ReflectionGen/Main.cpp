
#include "HelpFunctions.h"
#include "Options.h"
#include "Generator.h"

#include "clang-c/Index.h"





int main(int Argc, char** Argv)
{
	FReflectionGenOptions LOptions;
	ParseOptions(Argc, Argv, LOptions);

	if( LOptions.OutputDir.empty() )
	{
		RaiseError("No output dir!");
	}
	if( LOptions.InputFiles.empty() )
	{
		RaiseError("No input file!");
	}

	const char** LCCommandLine = static_cast<const char**>(malloc(sizeof(char*) * LOptions.ParserCommandLine.size()));
	for( size_t i = 0; i < LOptions.ParserCommandLine.size(); ++i )
	{
		LCCommandLine[i] = LOptions.ParserCommandLine[i].c_str();
	}

	CXIndex LFileIndex = clang_createIndex(0, 0);
	for( const std::string& InFile : LOptions.InputFiles )
	{
		// clang-format off
		CXTranslationUnit LFileUnit = clang_parseTranslationUnit(LFileIndex, InFile.c_str(), 
			LCCommandLine, static_cast<int>(LOptions.ParserCommandLine.size()), 
			nullptr, 0, 
			CXTranslationUnit_Incomplete | CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_KeepGoing);
		// clang-format on

		if( !LFileUnit )
		{
			RaiseWarning("'" + InFile + "' --- can't parse!");
			continue;
		}


		FReflectionGenInfo LGenInfo;
		FillGenInfo(LGenInfo, LFileUnit);

		clang_disposeTranslationUnit(LFileUnit);

		const std::string LFileNameOnly = GetFileNameOnly(InFile);
		const std::string OutHFile = CatPaths(LOptions.OutputDir, LFileNameOnly + ".generated.h");
		const std::string OutCppFile = CatPaths(LOptions.OutputDir, LFileNameOnly + ".generated.cpp");
		GenerateReflection(LGenInfo, InFile, OutHFile, OutCppFile);
	}

	clang_disposeIndex(LFileIndex);
	free(LCCommandLine);

	return EXIT_SUCCESS;
}