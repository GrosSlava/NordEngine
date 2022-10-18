
import ProjectGeneratorHelpers
import ProjectDefaultFiles

import os
import sys
import shutil
import glob
import pathlib
import uuid




'''
	Paths for c++ easy includs.
	Relative to engine root dir.
'''
ENGINE_INCLUDE_PATHS = [ \
os.path.join("Source", "ThirdParty", "SFML", "include"), \
os.path.join("Source", "Engine", "Core", "Containers", "Public"), \
os.path.join("Source", "Engine", "Core", "Delegate", "Public"), \
os.path.join("Source", "Engine", "Core", "Files", "Public"), \
os.path.join("Source", "Engine", "Core", "GenericPlatform", "Public"), \
os.path.join("Source", "Engine", "Core", "Macros"), \
os.path.join("Source", "Engine", "Core", "Math", "Public"), \
os.path.join("Source", "Engine", "Core", "Memory", "Public"), \
os.path.join("Source", "Engine", "Core", "Misc"), \
os.path.join("Source", "Engine", "Core", "Path", "Public"), \
os.path.join("Source", "Engine", "Core", "Templates"), \
os.path.join("Source", "Engine", "Core", "Time", "Public"), \
os.path.join("Source", "Engine", "Engine", "CoreGame", "Public"), \
os.path.join("Source", "Engine", "Engine", "Platforms", "Public"), \
os.path.join("Source", "Engine", "Engine", "SubEngines", "Public"), \
os.path.join("Source", "Engine", "Engine", "UObject", "Public") \
]

'''
	Libs used by engine.
	Append with platform specific suffix like '.lib' or '.a', '.so' etc...
'''
ENGINE_USING_LIBS = [ \
"Engine", \
"openal32", \
"sfml-graphics", \
"sfml-window", \
"sfml-system", \
"sfml-audio", \
"sfml-network" \
]

'''
	Helper function to get list of used libs.
'''
def GetUsingLibs(IsEngineModule: bool):
	if IsEngineModule:
		return ENGINE_USING_LIBS[1:]
	else:
		return ENGINE_USING_LIBS
#------------------------------------------------------#

'''
	Reserved folders names.
'''
RESERVED_PROJECT_FOLDER_NAME = [ \
"Intermediate", \
"Build", \
"build", \
"Source", \
"source", \
"Src", \
"src", \
"SRC", \
"Content", \
"content", \
"Docs", \
"docs", \
"doc", \
"Tools", \
"tools", \
"Private", \
"private", \
"Public", \
"public", \
"Plugins", \
"plugins", \
"ThirdParty" \
]


#..................Reserved modules names.................#

ENGINE_MODULE_NAME = "Engine"

#.........................................................#




#.......................................................Project config......................................................#

'''
	Base information about project.
'''
class FProjectConfig:
	def __init__(self, ProjectPath: str):
		self.ProjectPath = ProjectPath		# absolute path to project root
		self.PathToEngine = ""				# absolute or relative path to engine root folder.
		self.IsEnginePathRelative = False	# indicate that PathToEngine set as relative path
		self.IsEngine = False				# mark that we are building engine project
		self.ProjectName = ""				# name of project without '"'	
		self.ShowConsole = False			# project will have platform console
	
	def GetAbsolutePathToEngine(self):
		if self.IsEnginePathRelative:
			return os.path.join(self.ProjectPath, self.PathToEngine)
		else:
			return self.PathToEngine
	#------------------------------------------------------#


'''
	Read project generation config form "ProjectConfig.txt" file.
	@param ProjectPath - absolute path to project root.
	@return FProjectConfig.
'''
def ScanProjectConfig(ProjectPath: str):
	if not ProjectGeneratorHelpers.CheckAbsPath(ProjectPath):
		print("[ScanProjectConfig] --- Invalid project path.")
		return

	
	LProjectConfig = FProjectConfig(ProjectPath)
	
	if not os.path.exists(os.path.join(ProjectPath, "ProjectConfig.txt")):
		print("Can't find 'ProjectConfig.txt' file!")
		sys.exit(0)
	
	with open(os.path.join(ProjectPath, "ProjectConfig.txt"), "r", encoding = 'utf-8') as ProjectConfigFile:
		for LLine in ProjectConfigFile:
			S = LLine.split("=")
			if len(S) != 2:
				continue

			if S[0].strip() == "PathToEngine":
				LProjectConfig.PathToEngine = S[1].strip()
				if LProjectConfig.PathToEngine == ".":
					LProjectConfig.IsEngine = True
					LProjectConfig.IsEnginePathRelative = True
				elif LProjectConfig.PathToEngine[0] == ".":
					LProjectConfig.IsEnginePathRelative = True
			elif S[0].strip() == "ProjectName":
				LProjectConfig.ProjectName = S[1].strip()
			elif S[0].strip() == "ShowConsole":
				LProjectConfig.ShowConsole = ProjectGeneratorHelpers.StrToBool(S[1].strip())

	if LProjectConfig.PathToEngine.strip() == "":
		print("PathToEngine not set!")
		sys.exit(0)				
	if LProjectConfig.ProjectName.strip() == "":
		print("ProjectName not set!")
		sys.exit(0)	

	return LProjectConfig
#------------------------------------------------------#
	
#...........................................................................................................................#



#.....................................................Submodules............................................................#

'''
	Base information about project submodule.
'''
class FSubmoduleInfo:
	def __init__(self, Name: str, ModulePath: str, GameModuleUUID: str):
		self.Name = Name								# submodule name
		self.ModulePath = ModulePath					# absolute path to submodule root
		self.GameModuleUUID = GameModuleUUID.upper()	# submodule UUID
		self.PublicFilesPath = []						# array of module header file absolute paths
		self.PrivateFilesPath = []						# array of module source file absolute paths
		self.IsEngineModule = self.Name == ENGINE_MODULE_NAME
		self.IsPlugin = False							# indicate that this module is Plugin


'''
	Fill FSubmoduleInfo structure.
	@param ModulePath - absolute path to module root.
	@return FSubmoduleInfo.
'''
def FillSubmodule(ModulePath: str):
	if not ProjectGeneratorHelpers.CheckAbsPath(ModulePath):
		print("[FillSubmodule] --- Invalid module path.")
		return


	LModuleName = pathlib.Path(ModulePath).parts[-1]
	LModule = FSubmoduleInfo(Name = LModuleName, ModulePath = ModulePath, GameModuleUUID = str(uuid.uuid4()))
	
	for LFileName in glob.iglob(os.path.join(ModulePath, "**"), recursive = True):
		if not os.path.isfile(LFileName):
			continue
		if pathlib.Path(LFileName).suffix == ".h":
			LModule.PublicFilesPath.append(LFileName)
		elif pathlib.Path(LFileName).suffix == ".cpp" or pathlib.Path(LFileName).suffix == ".c":
			LModule.PrivateFilesPath.append(LFileName)

	return LModule
#------------------------------------------------------#

'''
	Find all modules in project and return it's array.
	@param ProjectPath - absolute path to project root.
	@return list of FSubmoduleInfo.
'''
def FindAllProjectSubmodules(ProjectPath: str):
	if not ProjectGeneratorHelpers.CheckAbsPath(ProjectPath):
		print("[FindAllProjectSubmodules] --- Invalid project path.")
		return

	
	LSubmodules = []
	for LFileName in glob.iglob(os.path.join(ProjectPath, "Source", "**"), recursive = False):
		if (not os.path.isdir(LFileName)) or (pathlib.Path(LFileName).parts[-1] in RESERVED_PROJECT_FOLDER_NAME):
			continue
		LModule = FillSubmodule(LFileName)
		LSubmodules.append(LModule)

	for LFileName in glob.iglob(os.path.join(ProjectPath, "Source", "Plugins", "**"), recursive = False):
		if (not os.path.isdir(LFileName)) or (pathlib.Path(LFileName).parts[-1] in RESERVED_PROJECT_FOLDER_NAME):
			continue
		LModule = FillSubmodule(LFileName)
		LModule.IsPlugin = True
		LSubmodules.append(LModule)

	return LSubmodules
#------------------------------------------------------#

#...........................................................................................................................#




'''
	Generate project structure.
	@param ProjectPath - absolute path to project root.
'''
def GenerateBaseProjectStructure(ProjectPath: str, ProjectConfig: FProjectConfig):
	if not ProjectGeneratorHelpers.CheckAbsPath(ProjectPath):
		print("[GenerateBaseProjectStructure] --- Invalid project path.")
		return
	

	ProjectGeneratorHelpers.CreateDirIfNotExist(os.path.join(ProjectPath, "Source"))
	ProjectGeneratorHelpers.CreateDirIfNotExist(os.path.join(ProjectPath, "Content"))	
	ProjectGeneratorHelpers.CreateDirIfNotExist(os.path.join(ProjectPath, "Docs"))
	ProjectGeneratorHelpers.CreateDirIfNotExist(os.path.join(ProjectPath, "Tools"))
	
	if not ProjectConfig.IsEngine:
		ProjectGeneratorHelpers.CreateDirIfNotExist(os.path.join(ProjectPath, "Source", ProjectConfig.ProjectName))
		ProjectGeneratorHelpers.CreateDirIfNotExist(os.path.join(ProjectPath, "Source", ProjectConfig.ProjectName, "Public"))
		ProjectGeneratorHelpers.CreateDirIfNotExist(os.path.join(ProjectPath, "Source", ProjectConfig.ProjectName, "Private"))
	ProjectGeneratorHelpers.CreateDirIfNotExist(os.path.join(ProjectPath, "Source", "Plugins"))
	ProjectGeneratorHelpers.CreateDirIfNotExist(os.path.join(ProjectPath, "Source", "ThirdParty"))
	
	
	LFilePath = os.path.join(ProjectPath, ".gitignore")
	if not os.path.exists(LFilePath):
		with open(LFilePath, 'w') as f:
			f.write(ProjectDefaultFiles.GetDefault_Gitignore_FileText())

	LFilePath = os.path.join(ProjectPath, ".gitattributes")
	if not os.path.exists(LFilePath):
		with open(LFilePath, 'w') as f:
			f.write(ProjectDefaultFiles.GetDefault_Gitattributes_FileText())

	LFilePath = os.path.join(ProjectPath, ".clang-format")
	if not os.path.exists(LFilePath):
		with open(LFilePath, 'w') as f:
			f.write(ProjectDefaultFiles.GetDefault_ClangFormat_FileText())
			
	LFilePath = os.path.join(ProjectPath, "LICENSE.md")
	if not os.path.exists(LFilePath):
		with open(LFilePath, 'w') as f:
			f.write('//TODO')

	LFilePath = os.path.join(ProjectPath, "README.md")
	if not os.path.exists(LFilePath):
		with open(LFilePath, 'w') as f:
			f.write('//TODO')
#------------------------------------------------------#