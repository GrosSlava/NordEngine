
import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))


import ToolsConfig
import Common.ToolsFunctionLibrary
import Common.Logger
import Common.ProjectConfig
import ProjectDefaultFiles

import shutil
import glob
import pathlib
import uuid





'''
	Helper function to get list of used libs.
'''
def GetUsingLibs(IsEngineModule: bool):
	if IsEngineModule:
		return ToolsConfig.ENGINE_USING_LIBS[1:]
	else:
		return ToolsConfig.ENGINE_USING_LIBS
#------------------------------------------------------#



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
		self.IsEngineModule = self.Name == ToolsConfig.ENGINE_MODULE_NAME
		self.IsPlugin = False							# indicate that this module is Plugin


'''
	Fill FSubmoduleInfo structure.
	@param ModulePath - absolute path to module root.
	@return FSubmoduleInfo.
'''
def FillSubmodule(ModulePath: str):
	if not Common.ToolsFunctionLibrary.CheckAbsPath(ModulePath):
		Common.Logger.Log("FillSubmodule", "Invalid module path.")
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
	if not Common.ToolsFunctionLibrary.CheckAbsPath(ProjectPath):
		Common.Logger.Log("FindAllProjectSubmodules", "Invalid project path.")
		return


	LSubmodules = []
	for LFileName in glob.iglob(os.path.join(ProjectPath, "Source", "**"), recursive = False):
		if (not os.path.isdir(LFileName)) or (pathlib.Path(LFileName).parts[-1] in ToolsConfig.RESERVED_PROJECT_FOLDER_NAME):
			continue
		LModule = FillSubmodule(LFileName)
		LSubmodules.append(LModule)

	for LFileName in glob.iglob(os.path.join(ProjectPath, "Source", "Plugins", "**"), recursive = False):
		if (not os.path.isdir(LFileName)) or (pathlib.Path(LFileName).parts[-1] in ToolsConfig.RESERVED_PROJECT_FOLDER_NAME):
			continue
		LModule = FillSubmodule(LFileName)
		LModule.IsPlugin = True
		LSubmodules.append(LModule)

	return LSubmodules
#------------------------------------------------------#

#...........................................................................................................................#




'''
	Generate project structure.
'''
def GenerateBaseProjectStructure(ProjectConfig: Common.ProjectConfig.FProjectConfig):
	Common.ToolsFunctionLibrary.CreateDirIfNotExist(os.path.join(ProjectConfig.ProjectPath, "Source"))
	Common.ToolsFunctionLibrary.CreateDirIfNotExist(os.path.join(ProjectConfig.ProjectPath, "Content"))	
	Common.ToolsFunctionLibrary.CreateDirIfNotExist(os.path.join(ProjectConfig.ProjectPath, "Docs"))
	Common.ToolsFunctionLibrary.CreateDirIfNotExist(os.path.join(ProjectConfig.ProjectPath, "Tools"))

	if not ProjectConfig.IsEngine:
		Common.ToolsFunctionLibrary.CreateDirIfNotExist(os.path.join(ProjectConfig.ProjectPath, "Source", ProjectConfig.ProjectName))
		Common.ToolsFunctionLibrary.CreateDirIfNotExist(os.path.join(ProjectConfig.ProjectPath, "Source", ProjectConfig.ProjectName, "Public"))
		Common.ToolsFunctionLibrary.CreateDirIfNotExist(os.path.join(ProjectConfig.ProjectPath, "Source", ProjectConfig.ProjectName, "Private"))
	Common.ToolsFunctionLibrary.CreateDirIfNotExist(os.path.join(ProjectConfig.ProjectPath, "Source", "Plugins"))
	Common.ToolsFunctionLibrary.CreateDirIfNotExist(os.path.join(ProjectConfig.ProjectPath, "Source", "ThirdParty"))


	LFilePath = os.path.join(ProjectConfig.ProjectPath, ".gitignore")
	if not os.path.exists(LFilePath):
		with open(LFilePath, 'w') as f:
			f.write(ProjectDefaultFiles.GetDefault_Gitignore_FileText())

	LFilePath = os.path.join(ProjectConfig.ProjectPath, ".gitattributes")
	if not os.path.exists(LFilePath):
		with open(LFilePath, 'w') as f:
			f.write(ProjectDefaultFiles.GetDefault_Gitattributes_FileText())

	LFilePath = os.path.join(ProjectConfig.ProjectPath, ".clang-format")
	if not os.path.exists(LFilePath):
		with open(LFilePath, 'w') as f:
			f.write(ProjectDefaultFiles.GetDefault_ClangFormat_FileText())

	LFilePath = os.path.join(ProjectConfig.ProjectPath, "LICENSE.md")
	if not os.path.exists(LFilePath):
		with open(LFilePath, 'w') as f:
			f.write('//TODO')

	LFilePath = os.path.join(ProjectConfig.ProjectPath, "README.md")
	if not os.path.exists(LFilePath):
		with open(LFilePath, 'w') as f:
			f.write('//TODO')
#------------------------------------------------------#