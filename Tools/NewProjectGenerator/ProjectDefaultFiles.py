



def GetDefaultGitignoreFileText():
	return \
	"""
//..................Intermediate folders...................//

**Build
**Intermediate
**.vs
**__pycache__

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

//..................Intermediate files.....................//

*.user
*.generated*
*.gen
*.obj
*.o

//.........................................................//
	"""

def GetDefaultGitattributesFileText():
	return \
	"""
# Auto detect text files and perform LF normalization
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
	
def GetDefaultClangFormatFileText():
	return \
	"""
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