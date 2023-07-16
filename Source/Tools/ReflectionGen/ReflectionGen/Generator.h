
#pragma once

#include "ReflectionGenCore.h"
#include "GenerationInfo.h"




/**
	Main function for generationg C++ files.
*/
void GenerateReflection(const FReflectionGenInfo& Info, const std::string& SourceFile, const std::string& OutHFilePath, const std::string& OutCppFilePath);