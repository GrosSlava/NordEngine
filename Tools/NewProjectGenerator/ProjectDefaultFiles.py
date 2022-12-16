




def GetDefault_Gitignore_FileText():
	return \
	r"""
//..................Intermediate folders...................//

# Engine intermediate folders
**Build
**Intermediate

# Python intermediate folders
**.env
**.venv
**env
**venv
**ENV
**env.bak
**venv.bak
**__pycache__

**.vs
**.Trash-*

//.........................................................//

//..................Intermediate files.....................//

# Generated files
*.user
*.generated*
*.gen
*.lnk

# Generated batch/shell scripts in root
/BuildProjectWindows.bat
/ClearIntermediate.bat
/GenerateWindowsProject.bat
/GenerateDocs.bat

# Compiled Object files
*.slo
*.lo
*.o
*.ko
*.obj
*.elf

# Precompiled Headers
*.gch
*.pch

# Executables
*.exe
*.out
*.app
*.ipa

# Linker output
*.ilk
*.map
*.exp

//.........................................................//

//...................Visual Studio......................//

*.vscode
*.code-workspace
*.vs
*.vcxproj
*.sln
*.idea
*.VC.db
*.opensdf
*.opendb
*.sdf
*.suo
*.xcodeproj
*.xcworkspace

//......................................................//
	"""
#------------------------------------------------------#

def GetDefault_Gitattributes_FileText():
	return \
	r"""
#Auto detect text files and perform LF normalization
* text=auto

#Image
*.jpg filter=lfs diff=lfs merge=lfs -text
*.jpeg filter=lfs diff=lfs merge=lfs -text
*.png filter=lfs diff=lfs merge=lfs -text
*.gif filter=lfs diff=lfs merge=lfs -text
*.psd filter=lfs diff=lfs merge=lfs -text
*.ai filter=lfs diff=lfs merge=lfs -text
*.tif filter=lfs diff=lfs merge=lfs -text
*.hdr filter=lfs diff=lfs merge=lfs -text
*.tga filter=lfs diff=lfs merge=lfs -text

#Audio
*.mp3 filter=lfs diff=lfs merge=lfs -text
*.wav filter=lfs diff=lfs merge=lfs -text
*.ogg filter=lfs diff=lfs merge=lfs -text

#Video
*.mp4 filter=lfs diff=lfs merge=lfs -text
*.mov filter=lfs diff=lfs merge=lfs -text

#3D Object
*.FBX filter=lfs diff=lfs merge=lfs -text
*.fbx filter=lfs diff=lfs merge=lfs -text
*.blend filter=lfs diff=lfs merge=lfs -text
*.obj filter=lfs diff=lfs merge=lfs -text
*.3ds filter=lfs diff=lfs merge=lfs -text

#ETC
*.duf filter=lfs diff=lfs merge=lfs -text
*.bin.fbx filter=lfs diff=lfs merge=lfs -text
*.exr filter=lfs diff=lfs merge=lfs -text
*.mb filter=lfs diff=lfs merge=lfs -text
*.cubemap filter=lfs diff=lfs merge=lfs -text
*.xcf filter=lfs diff=lfs merge=lfs -text
	"""
#------------------------------------------------------#

def GetDefault_ClangFormat_FileText():
	return \
	r"""
BasedOnStyle: Microsoft
AccessModifierOffset: -4
IndentWidth: 4
UseTab: Always
TabWidth: 4
ColumnLimit: 220
MaxEmptyLinesToKeep: 7
#------------------------------------------------------#
SortIncludes: false
ReflowComments: false
EmptyLineAfterAccessModifier: Always
KeepEmptyLinesAtTheStartOfBlocks: false
CompactNamespaces: false
FixNamespaceComments: true
#------------------------------------------------------#
BreakBeforeBraces: Allman
BreakBeforeTernaryOperators: false
BreakConstructorInitializers: AfterColon
BreakInheritanceList: AfterColon
BreakStringLiterals: false
IndentCaseLabels: false
AlwaysBreakTemplateDeclarations: Yes
#------------------------------------------------------#
AlignAfterOpenBracket: DontAlign
AlignEscapedNewlines: Left
AlignTrailingComments: true
AlignOperands: true
PointerAlignment: Left
ReferenceAlignment: Left
#QualifierAlignment: Leave
#------------------------------------------------------#
AllowShortBlocksOnASingleLine: Never
AllowShortFunctionsOnASingleLine: InlineOnly
AllowShortCaseLabelsOnASingleLine: true
AllowShortIfStatementsOnASingleLine: WithoutElse
AllowAllArgumentsOnNextLine: true
AllowAllConstructorInitializersOnNextLine: true
AllowAllParametersOfDeclarationOnNextLine: true
#------------------------------------------------------#
SpaceBeforeParens: Never
SpaceBeforeAssignmentOperators: true
SpaceBeforeCpp11BracedList: true
SpaceBeforeInheritanceColon: true
SpaceBeforeRangeBasedForLoopColon: true
SpacesBeforeTrailingComments: 1
SpaceBeforeSquareBrackets: false
SpaceAfterCStyleCast: false
SpaceAfterLogicalNot: false
SpaceAfterTemplateKeyword: false
SpacesInAngles: false
SpacesInCStyleCastParentheses: false
SpaceInEmptyBlock: true
SpacesInConditionalStatement: true
SpaceAfterTemplateKeyword: false
#------------------------------------------------------#
StatementMacros : [
					"StaticCheck", "StaticCheckTrue", "StaticCheckEqual", 
					"StaticCheckNotEqual", "StaticCheckFloatEqual", "StaticCheckFloatNotEqual",
					"StaticCheckSameSize", "StaticCheckAlign", "StaticCheckSize",
					"Assert", "AssertTrue", "AssertEqual", "AssertNotEqual", "AssertFloatEqual", "AssertFloatNotEqual",
					"checkCode", "verify", "check",	"verifyf", "checkf",
					"checkNoEntry",	"checkNoReentry", "checkNoRecursion", "unimplemented",
					"ensure", "ensureMsgf", "ensureAlways","ensureAlwaysMsgf",
				  ]
	"""
#------------------------------------------------------#

def Fixup_Doxyfile(FilePath: str, ProjectName: str):
	NewFileS = ""
	with open(FilePath, 'r') as f:
		for LLine in f:
			if len(LLine) == 0 or LLine[0] == '#':
				NewFileS += LLine
				continue
			S = LLine.split("=")
			if len(S) != 2:
				NewFileS += LLine
				continue
			LKey = S[0].strip()
			if LKey == "PROJECT_NAME":
				NewFileS += S[0] + "= \"" + ProjectName + "\"\n"
			elif LKey == "OUTPUT_DIRECTORY":
				NewFileS += S[0] + "= Docs\n"
			elif LKey == "INPUT":
				NewFileS += S[0] + "= Source README.md\n"
			elif LKey == "RECURSIVE":
				NewFileS += S[0] + "= YES\n"
			elif LKey == "EXCLUDE_PATTERNS":
				NewFileS += S[0] + "= */Private/* */ThirdParty/*\n"
			elif LKey == "USE_MDFILE_AS_MAINPAGE":
				NewFileS += S[0] + "= README.md\n"
			elif LKey == "GENERATE_LATEX":
				NewFileS += S[0] + "= NO\n"
			elif LKey == "BUILTIN_STL_SUPPORT":
				NewFileS += S[0] + "= YES\n"
			elif LKey == "EXTRACT_STATIC":
				NewFileS += S[0] + "= YES\n"
			elif LKey == "DISABLE_INDEX":
				NewFileS += S[0] + "= Yes\n"
			elif LKey == "GENERATE_TREEVIEW":
				NewFileS += S[0] + "= Yes\n"
			elif LKey == "FULL_SIDEBAR":
				NewFileS += S[0] + "= NO\n"
			else:
				NewFileS += LLine
	with open(FilePath, 'w') as f:
		f.write(NewFileS)
#------------------------------------------------------#




def GetDefault_ProjectConfig_FileText(PathToEngine: str, ProjectName: str):
	return \
	r"""
PathToEngine = {PathToEngine}
ProjectName = {ProjectName}
ShowConsole = false
Modules = {ProjectName}
	""".format(PathToEngine = PathToEngine, ProjectName = ProjectName)
#------------------------------------------------------#

def GetDefault_GenerateWindowsProject_BatFileText(PathToEngine: str):
	return \
	r"""
@echo off


rem Set absolute path to engine
set EngineRoot={PathToEngine}



rem ....................................................................................... rem
rem ....................................................................................... rem
rem ....................................................................................... rem


python "%EngineRoot%\Tools\NewProjectGenerator\WindowsProjectGenerator.py" %~dp0
pause
	""".format(PathToEngine = PathToEngine)
#------------------------------------------------------#

def GetDefault_ClearIntermediate_BatFileText(PathToEngine: str):
	return \
	r"""
@echo off


rem Set absolute path to engine
set EngineRoot={PathToEngine}



rem ....................................................................................... rem
rem ....................................................................................... rem
rem ....................................................................................... rem


python "%EngineRoot%\Tools\ClearIntermediateFiles\ClearIntermediate.py" %~dp0
pause
	""".format(PathToEngine = PathToEngine)
#------------------------------------------------------#

def GetDefault_BuildProjectWindows_BatFileText(PathToEngine: str):
	return \
	r"""
@echo off


rem Set absolute path to engine
set EngineRoot={PathToEngine}



rem ....................................................................................... rem
rem ....................................................................................... rem
rem ....................................................................................... rem


call "%EngineRoot%\Tools\BuildEngine\GetMSBuildPath.bat"
if errorlevel 1 goto Error_NoVisualStudioEnvironment

python "%EngineRoot%\Tools\BuildEngine\ProjectBuildToolWindows.py" %~dp0 %MSBUILD_PATH%
pause
goto Exit



:Error_NoVisualStudioEnvironment
echo ERROR: Missing Visual Studio 2015 or newer.
pause
goto Exit

:Exit
exit /B 1
	""".format(PathToEngine = PathToEngine)
#------------------------------------------------------#

def GetDefault_GenerateDocs_BatFileText():
	return \
	r"""
@echo off


doxygen "Doxyfile"
pause
	"""
#------------------------------------------------------#
