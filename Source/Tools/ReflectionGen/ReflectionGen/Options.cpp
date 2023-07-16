
#include "Options.h"





void ParseOptions(int Argc, char** Argv, FReflectionGenOptions& OutOptions)
{
	if( Argc > 1 )
	{
		OutOptions.OutputDir = Argv[1];
	}
	OutOptions.ParserCommandLine.push_back("-x");
	OutOptions.ParserCommandLine.push_back("c++");

	bool CommandLineHeighPart = false;
	for( int i = 2; i < Argc; ++i )
	{
		std::string LArg = std::string(Argv[i]);

		if( LArg == "!" )
		{
			CommandLineHeighPart = true;
			continue;
		}

		if( CommandLineHeighPart )
		{
			OutOptions.ParserCommandLine.push_back(std::move(LArg));
		}
		else
		{
			OutOptions.InputFiles.push_back(std::move(LArg));
		}
	}
}
