
import ProjectGeneratorBase

import os
import sys
import uuid
import pathlib




#.................................................Solution default files....................................................#

def GetSolutionFileText(Submodules: list):
	LProjectUUID = str(uuid.uuid4()).upper()

	LProjectsSection = ""
	LProjectsConfigurationSection = ""

	for LSubmodule in Submodules:
		LProjectsSection += """Project("{{{ProjectUUID}}}") = "{ModuleName}", "{Path}\{ModuleName}.vcxproj", "{{{GameModuleUUID}}}" EndProject\n""".format \
		( \
			ProjectUUID = LProjectUUID, ModuleName = LSubmodule.Name, Path = LSubmodule.ModulePath, GameModuleUUID = LSubmodule.GameModuleUUID \
		)

		LProjectsConfigurationSection += "\t\t{{{GameModuleUUID}}}.Debug|x86-64.ActiveCfg = Debug|x64\n".format(GameModuleUUID = LSubmodule.GameModuleUUID)
		LProjectsConfigurationSection += "\t\t{{{GameModuleUUID}}}.Debug|x86-64.Build.0 = Debug|x64\n".format(GameModuleUUID = LSubmodule.GameModuleUUID)
		LProjectsConfigurationSection += "\t\t{{{GameModuleUUID}}}.Release|x86-64.ActiveCfg = Release|x64\n".format(GameModuleUUID = LSubmodule.GameModuleUUID)
		LProjectsConfigurationSection += "\t\t{{{GameModuleUUID}}}.Release|x86-64.Build.0 = Release|x64\n".format(GameModuleUUID = LSubmodule.GameModuleUUID)

	return \
	"""Microsoft Visual Studio Solution File, Format Version 12.00
# Visual Studio Version 17
VisualStudioVersion = 17
MinimumVisualStudioVersion = 10.0.40219.1
{ProjectsSection}
Global
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
		Debug|x86-64 = Debug|x86-64
		Release|x86-64 = Release|x86-64
	EndGlobalSection
	GlobalSection(ProjectConfigurationPlatforms) = postSolution
{ProjectsConfigurationSection}
	EndGlobalSection
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
	GlobalSection(ExtensibilityGlobals) = postSolution
		SolutionGuid = {{{SolutionGuid}}}
	EndGlobalSection
EndGlobal
	""".format(ProjectsSection = LProjectsSection, ProjectsConfigurationSection = LProjectsConfigurationSection, SolutionGuid = str(uuid.uuid4()).upper())
#------------------------------------------------------#


def GetVCXFileText(ProjectConfig: ProjectGeneratorBase.FProjectConfig, ModuleSolution : ProjectGeneratorBase.FSubmoduleInfo, EngineIncludePaths: str, LibsDir: str, UsingLibs: str):
	LPublicFilesSection = ""
	for LPublicFilePath in ModuleSolution.PublicFilesPath:
		LPublicFilesSection += '\t\t<ClInclude Include="{FilePath}" />\n'.format(FilePath = os.path.relpath(LPublicFilePath, ModuleSolution.ModulePath))
	
	LPrivateFilesSection = ""
	for LPrivateFilePath in ModuleSolution.PrivateFilesPath:
		LPrivateFilesSection += '\t\t<ClCompile Include="{FilePath}" />\n'.format(FilePath = os.path.relpath(LPrivateFilePath, ModuleSolution.ModulePath))


	LPreprocessorDebugDefinitions = "_DEBUG;_WINDOWS;WIN32;WIN64;"
	LPreprocessorReleaseDefinitions = "NDEBUG;_WINDOWS;WIN32;WIN64;"
	if ModuleSolution.IsEngineModule:
		LPreprocessorDebugDefinitions += "ENGINE;"
		LPreprocessorReleaseDefinitions += "ENGINE;"
	

	LConfigurationType = ""
	if ModuleSolution.IsEngineModule or ModuleSolution.IsPlugin:
		LConfigurationType = "DynamicLibrary"
	else:
		LConfigurationType = "Application"


	LPostBuildEvent = ""
	if ProjectConfig.IsEngine:
		LPostBuildEvent = "\t\tpython \"$(SolutionDir){ScriptPath}\" $(SolutionDir)".format(ScriptPath = os.path.join("Tools", "BuildEngine", "PostModuleBuild.py")) 
	else:
		LPostBuildEvent = "\t\tpython \"{ScriptPath}\" $(SolutionDir)".format(ScriptPath = os.path.join(ProjectConfig.GetAbsolutePathToEngine(), "Tools", "BuildEngine", "PostModuleBuild.py")) 

	return \
	"""<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
	<ItemGroup Label="ProjectConfigurations">
		<ProjectConfiguration Include="Debug|x64">
			<Configuration>Debug</Configuration>
			<Platform>x64</Platform>
		</ProjectConfiguration>
		<ProjectConfiguration Include="Release|x64">
			<Configuration>Release</Configuration>
			<Platform>x64</Platform>
		</ProjectConfiguration>
	</ItemGroup>
	<PropertyGroup Label="Globals">
		<VCProjectVersion>16.0</VCProjectVersion>
		<Keyword>Win32Proj</Keyword>
		<ProjectGuid>{GameModuleUUID}</ProjectGuid>
		<RootNamespace>Source</RootNamespace>
		<WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>
	</PropertyGroup>
	<Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
	<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'" Label="Configuration">
		<ConfigurationType>{ConfigurationType}</ConfigurationType>
		<UseDebugLibraries>true</UseDebugLibraries>
		<PlatformToolset>v143</PlatformToolset>
		<WholeProgramOptimization>false</WholeProgramOptimization>
		<CharacterSet>Unicode</CharacterSet>
		<PreferredToolArchitecture>x64</PreferredToolArchitecture>
	</PropertyGroup>
	<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'" Label="Configuration">
		<ConfigurationType>{ConfigurationType}</ConfigurationType>
		<UseDebugLibraries>false</UseDebugLibraries>
		<PlatformToolset>v143</PlatformToolset>
		<WholeProgramOptimization>true</WholeProgramOptimization>
		<CharacterSet>Unicode</CharacterSet>
		<PreferredToolArchitecture>x64</PreferredToolArchitecture>
	</PropertyGroup>
	<Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
	<ImportGroup Label="ExtensionSettings">
	</ImportGroup>
	<ImportGroup Label="Shared">
	</ImportGroup>
	<ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
		<Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
	</ImportGroup>
	<ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
		<Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
	</ImportGroup>
	<PropertyGroup Label="UserMacros" />
	<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
		<OutDir>$(SolutionDir)Build\</OutDir>
		<IntDir>$(SolutionDir)Intermediate\$(Platform)\$(Configuration)\</IntDir>
	</PropertyGroup>
	<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
		<OutDir>$(SolutionDir)Build\</OutDir>
		<IntDir>$(SolutionDir)Intermediate\$(Platform)\$(Configuration)\</IntDir>
	</PropertyGroup>
	<ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
		<ClCompile>
			<WarningLevel>Level3</WarningLevel>
			<SDLCheck>true</SDLCheck>
			<PreprocessorDefinitions>{PreprocessorDebugDefinitions}%(PreprocessorDefinitions)</PreprocessorDefinitions>
			<ConformanceMode>true</ConformanceMode>
			<PrecompiledHeader>NotUsing</PrecompiledHeader>
			<LanguageStandard>stdcpp17</LanguageStandard>
			<LanguageStandard_C>stdc17</LanguageStandard_C>
			<AdditionalIncludeDirectories>Public;{EngineIncludePaths}%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
			<MultiProcessorCompilation>true</MultiProcessorCompilation>
		</ClCompile>
		<Link>
			<SubSystem>{SubSystem}</SubSystem>
			<GenerateDebugInformation>true</GenerateDebugInformation>
			<AdditionalLibraryDirectories>{LibsDir}%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
			<AdditionalDependencies>{UsingLibs}%(AdditionalDependencies)</AdditionalDependencies>
		</Link>
		<PostBuildEvent>
		<Command>
{PostBuildEvent}
		exit 0
		</Command>
		</PostBuildEvent>
	</ItemDefinitionGroup>
	<ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
		<ClCompile>
			<WarningLevel>Level3</WarningLevel>
			<FunctionLevelLinking>true</FunctionLevelLinking>
			<IntrinsicFunctions>true</IntrinsicFunctions>
			<SDLCheck>true</SDLCheck>
			<PreprocessorDefinitions>{PreprocessorReleaseDefinitions}%(PreprocessorDefinitions)</PreprocessorDefinitions>
			<ConformanceMode>true</ConformanceMode>
			<PrecompiledHeader>NotUsing</PrecompiledHeader>
			<LanguageStandard>stdcpp17</LanguageStandard>
			<LanguageStandard_C>stdc17</LanguageStandard_C>
			<AdditionalIncludeDirectories>Public;{EngineIncludePaths}%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
			<MultiProcessorCompilation>true</MultiProcessorCompilation>
		</ClCompile>
		<Link>
			<SubSystem>{SubSystem}</SubSystem>
			<EnableCOMDATFolding>true</EnableCOMDATFolding>
			<OptimizeReferences>true</OptimizeReferences>
			<GenerateDebugInformation>false</GenerateDebugInformation>
			<AdditionalLibraryDirectories>{LibsDir}%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
			<AdditionalDependencies>{UsingLibs}%(AdditionalDependencies)</AdditionalDependencies>
		</Link>
		<PostBuildEvent>
		<Command>
{PostBuildEvent}
		exit 0
		</Command>
		</PostBuildEvent>
	</ItemDefinitionGroup>
	<ItemGroup>
{PublicFilesSection}
	</ItemGroup>
	<ItemGroup>
{PrivateFilesSection}
	</ItemGroup>
	<Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
	<ImportGroup Label="ExtensionTargets">
	</ImportGroup>
</Project>
	""".format( \
				GameModuleUUID = ModuleSolution.GameModuleUUID, \
				ConfigurationType = LConfigurationType, \
				SubSystem = "Console" if ProjectConfig.ShowConsole else "Windows", \
				EngineIncludePaths = EngineIncludePaths, \
				LibsDir = LibsDir, \
				UsingLibs = UsingLibs, \
				PreprocessorDebugDefinitions = LPreprocessorDebugDefinitions, \
				PreprocessorReleaseDefinitions = LPreprocessorReleaseDefinitions, \
				PublicFilesSection = LPublicFilesSection, \
				PrivateFilesSection = LPrivateFilesSection, \
				PostBuildEvent = LPostBuildEvent
				)
#------------------------------------------------------#


def GetUserFileText():
	return \
	"""<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="Current" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
	<PropertyGroup>
		<ShowAllFiles>true</ShowAllFiles>
	</PropertyGroup>
</Project>	
	"""
#------------------------------------------------------#

#...........................................................................................................................#





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	
	LProjectConfig = ProjectGeneratorBase.ScanProjectConfig(SolutionDir)
	ProjectGeneratorBase.GenerateBaseProjectStructure(SolutionDir, LProjectConfig)
	
	
	EngineIncludePaths = ""
	if LProjectConfig.IsEngine:
		EngineIncludePaths = ";".join(list(map(lambda x : "$(SolutionDir)" + x, ProjectGeneratorBase.ENGINE_INCLUDE_PATHS))) + ";"
	else:
		EngineIncludePaths = ";".join(list(map(lambda x : os.path.join(LProjectConfig.GetAbsolutePathToEngine(), x), ProjectGeneratorBase.ENGINE_INCLUDE_PATHS))) + ";"
	
	LibsDir = ""
	if LProjectConfig.IsEngine:
		LibsDir = "$(SolutionDir)Source\ThirdParty\SFML\lib\Windows;"
		LibsDir += "$(SolutionDir)Build\lib;"
	else:
		LibsDir = os.path.join(LProjectConfig.GetAbsolutePathToEngine(), "Build", "lib;")
	
	
	LSubmodules = ProjectGeneratorBase.FindAllProjectSubmodules(SolutionDir)
	
	with open(os.path.join(SolutionDir, "{Name}.sln".format(Name = LProjectConfig.ProjectName)), 'w') as f:
		f.write(GetSolutionFileText(LSubmodules))
	
	for LSubmodule in LSubmodules:
		UsingLibs = ";".join(list(map(lambda x : x + ".lib", ProjectGeneratorBase.GetUsingLibs(LSubmodule.IsEngineModule)))) + ";"
		
		with open(os.path.join(LSubmodule.ModulePath, "{Name}.vcxproj".format(Name = LSubmodule.Name)), 'w') as f:
			f.write(GetVCXFileText(LProjectConfig, LSubmodule, EngineIncludePaths, LibsDir, UsingLibs))
		with open(os.path.join(LSubmodule.ModulePath, "{Name}.vcxproj.user".format(Name = LSubmodule.Name)), 'w') as f:
			f.write(GetUserFileText())