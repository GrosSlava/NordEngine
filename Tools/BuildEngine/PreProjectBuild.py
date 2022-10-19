
import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))


import ToolsConfig
import Common.ProjectConfig
import Common.ToolsFunctionLibrary
import Common.Logger
import Common.BuildInfo

import shutil
import pathlib
import glob





'''
	Method to call before project rebuild.
	@param ProjectConfig - scanned project config.
	@param Platform - name of platform to compile. e,g ["Windows", "Linux"...]
'''
def PreProjectBuild(ProjectConfig: Common.ProjectConfig.FProjectConfig, Platform: str):
	if not Common.BuildInfo.CheckPlatform(Platform):
		Common.Logger.Log("PreProjectBuild", "Invalid platform name.")
		return


	LEngineBuildDir = os.path.join(ProjectConfig.GetAbsolutePathToEngine(), "Build")

	LProjectBuildDir = os.path.join(ProjectConfig.ProjectPath, "Build")
	LProjectBuildContentDir = os.path.join(LProjectBuildDir, "Content")
	LProjectBuildLibDir = os.path.join(LProjectBuildDir, "lib")
	
	LProjectIntermediateDir = os.path.join(ProjectConfig.ProjectPath, "Intermediate")
	LProjectContentDir = os.path.join(ProjectConfig.ProjectPath, "Content")
	LProjectSourceDir = os.path.join(ProjectConfig.ProjectPath, "Source")


	if os.path.exists(LProjectBuildDir):
		shutil.rmtree(LProjectBuildDir)

	Common.ToolsFunctionLibrary.CreateDirIfNotExist(LProjectIntermediateDir)
	Common.ToolsFunctionLibrary.CreateDirIfNotExist(LProjectBuildDir)
	Common.ToolsFunctionLibrary.CreateDirIfNotExist(LProjectBuildLibDir)
	
	if not ProjectConfig.IsEngine:
		if Platform == "Windows":
			Common.ToolsFunctionLibrary.CopyAllFilesWithExtinsionFromDirReplacing(LEngineBuildDir, LProjectBuildDir, ".dll")
		elif Platform == "Linux":
			Common.ToolsFunctionLibrary.CopyAllFilesWithExtinsionFromDirReplacing(LEngineBuildDir, LProjectBuildDir, ".so")

	if os.path.exists(LProjectContentDir):
		shutil.copytree(LProjectContentDir, LProjectBuildContentDir)
	else:
		Common.ToolsFunctionLibrary.CreateDirIfNotExist(LProjectBuildContentDir)

	for LFileName in glob.iglob(os.path.join(LProjectSourceDir, "ThirdParty", "**"), recursive = False):
		if not os.path.isdir(LFileName):
			continue

		Common.ToolsFunctionLibrary.CopyAllFilesFromDirReplacing(os.path.join(LFileName, "bin", Platform), LProjectBuildDir)
		Common.ToolsFunctionLibrary.CopyAllFilesFromDirReplacing(os.path.join(LFileName, "lib", Platform), LProjectBuildLibDir)
#------------------------------------------------------#





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	Platform = sys.argv[2]
	
	LProjectConfig = Common.ProjectConfig.ScanProjectConfig(SolutionDir)
	
	PreProjectBuild(LProjectConfig, Platform)