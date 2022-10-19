
import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))


import Common.ToolsFunctionLibrary
import Common.Logger
import Common.BuildInfo

import shutil
import pathlib
import glob





'''
	Method to call before project rebuild.
	@param SolutionDir - absolute path to project root.
	@param EngineDir - absolute path to engine root.
	@param Platform - name of platform to compile. e,g ["Windows", "Linux"...]
'''
def PreProjectBuild(SolutionDir: str, EngineDir: str, Platform: str):
	if not Common.ToolsFunctionLibrary.CheckAbsPath(SolutionDir):
		Common.Logger.Log("PreProjectBuild", "Invalid solution path.")
		return
	if not Common.ToolsFunctionLibrary.CheckAbsPath(EngineDir):
		Common.Logger.Log("PreProjectBuild", "Invalid engine path.")
		return
	if not Common.BuildInfo.CheckPlatform(Platform):
		Common.Logger.Log("PreProjectBuild", "Invalid platform name.")
		return


	if os.path.exists(os.path.join(SolutionDir, "Build")):
		shutil.rmtree(os.path.join(SolutionDir, "Build"))

	Common.ToolsFunctionLibrary.CreateDirIfNotExist(os.path.join(SolutionDir, "Intermediate"))
	Common.ToolsFunctionLibrary.CreateDirIfNotExist(os.path.join(SolutionDir, "Build"))
	Common.ToolsFunctionLibrary.CreateDirIfNotExist(os.path.join(SolutionDir, "Build", "lib"))
	
	if SolutionDir != EngineDir:
		if Platform == "Windows":
			Common.ToolsFunctionLibrary.CopyAllFilesWithExtinsionFromDirReplacing(os.path.join(EngineDir, "Build"), os.path.join(SolutionDir, "Build"), ".dll")
		elif Platform == "Linux":
			Common.ToolsFunctionLibrary.CopyAllFilesWithExtinsionFromDirReplacing(os.path.join(EngineDir, "Build"), os.path.join(SolutionDir, "Build"), ".so")

	if os.path.exists(os.path.join(SolutionDir, "Content")):
		shutil.copytree(os.path.join(SolutionDir, "Content"), os.path.join(SolutionDir, "Build", "Content"))
	else:
		Common.ToolsFunctionLibrary.CreateDirIfNotExist(os.path.join(SolutionDir, "Build", "Content"))

	for LFileName in glob.iglob(os.path.join(SolutionDir, "Source", "ThirdParty", "**"), recursive = False):
		if not os.path.isdir(LFileName):
			continue
		
		Common.ToolsFunctionLibrary.CopyAllFilesFromDirReplacing(os.path.join(LFileName, "bin", Platform), os.path.join(SolutionDir, "Build"))
		Common.ToolsFunctionLibrary.CopyAllFilesFromDirReplacing(os.path.join(LFileName, "lib", Platform), os.path.join(SolutionDir, "Build", "lib"))
#------------------------------------------------------#





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	EngineDir = sys.argv[2]
	Platform = sys.argv[3]
	
	PreProjectBuild(SolutionDir, EngineDir, Platform)