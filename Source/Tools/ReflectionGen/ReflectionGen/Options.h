
#pragma once

#include "ReflectionGenCore.h"




/**
	Generator options values.
*/
struct FReflectionGenOptions
{
	std::vector<std::string> InputFiles;
	std::string OutputDir;
	std::vector<std::string> ParserCommandLine;
};

/**
	Main function to parse command line.

	Options should have special format:
		OutputDir InputFiles... ! <Third party parser options>...
*/
void ParseOptions(int Argc, char** Argv, FReflectionGenOptions& OutOptions);